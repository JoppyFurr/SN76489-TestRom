/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "SMSlib.h"

#define PORT_A_DPAD_MASK    (PORT_A_KEY_UP | PORT_A_KEY_DOWN | PORT_A_KEY_LEFT | PORT_A_KEY_RIGHT)
#define PORT_A_KEY_MASK     (PORT_A_KEY_1 | PORT_A_KEY_2)

#include "../tile_data/palette.h"
#include "../tile_data/pattern.h"
#include "../tile_data/pattern_index.h"
#include "cursor.h"
#include "draw.h"
#include "register.h"
#include "key_interface.h"
#include "gui_elements.h"


typedef struct gui_state_s {

    const gui_element_t *gui;
    element_id_t current_element;
    uint8_t keyboard_key;

    bool cursor_update;
    bool keyboard_update;
    bool element_update;

    uint16_t element_values [ELEMENT_COUNT];

} gui_state_t;

static const uint16_t value_defaults [ELEMENT_COUNT] = {
    [ELEMENT_CH0_VOLUME] = 0,
    [ELEMENT_CH0_MODE_KEYBOARD] = 1,
    [ELEMENT_CH0_MODE_CONSTANT] = 0,
    [ELEMENT_CH0_FREQUENCY] = 428,
    [ELEMENT_CH0_BUTTON] = 0,
    [ELEMENT_CH1_VOLUME] = 0,
    [ELEMENT_CH1_MODE_KEYBOARD] = 0,
    [ELEMENT_CH1_MODE_CONSTANT] = 0,
    [ELEMENT_CH1_FREQUENCY] = 339,
    [ELEMENT_CH1_BUTTON] = 0,
    [ELEMENT_CH2_VOLUME] = 0,
    [ELEMENT_CH2_MODE_KEYBOARD] = 0,
    [ELEMENT_CH2_MODE_CONSTANT] = 0,
    [ELEMENT_CH2_FREQUENCY] = 285,
    [ELEMENT_CH2_BUTTON] = 0,
    [ELEMENT_NOISE_VOLUME] = 0,
    [ELEMENT_NOISE_MODE_KEYBOARD] = 0,
    [ELEMENT_NOISE_MODE_CONSTANT] = 0,
    [ELEMENT_NOISE_CONTROL] = 4,
    [ELEMENT_NOISE_BUTTON] = 0,
};

uint16_t notes [29] = {
    856, 807, 762, 719, 679, 641, 605, 571,
    539, 508, 480, 453, 428, 404, 381, 360,
    339, 320, 302, 285, 269, 254, 240, 226,
    214, 202, 190, 180, 170
};


/*
 * Push the in-ram copy of an element's value to the GUI and run its callback
 * to push the value to the 76489.
 */
static void element_update (const gui_element_t *element, uint16_t value)
{
    if (element->type == TYPE_VALUE)
    {
        draw_value (element->x, element->y, value);
    }
    else if (element->type == TYPE_VALUE_WIDE)
    {
        draw_value_wide (element->x, element->y, value);
    }
    else if (element->type == TYPE_LED)
    {
        draw_led (element->x, element->y, value);
    }
    else if (element->type == TYPE_BUTTON)
    {
        draw_button (element->x, element->y, value);
    }

    if (element->callback)
    {
        element->callback (value);
    }
}


/*
 * Move the cursor to select a different GUI element.
 */
static void element_navigate (gui_state_t *state, uint16_t key_pressed)
{
    if (state->current_element == ELEMENT_KEYBOARD)
    {
        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
                if      (state->keyboard_key < 7 ) state->current_element = ELEMENT_NOISE_VOLUME;
                else if (state->keyboard_key < 11 ) state->current_element = ELEMENT_NOISE_MODE_KEYBOARD;
                else if (state->keyboard_key < 16) state->current_element = ELEMENT_NOISE_MODE_CONSTANT;
                else if (state->keyboard_key < 21) state->current_element = ELEMENT_NOISE_CONTROL;
                else                               state->current_element = ELEMENT_NOISE_BUTTON;
                state->cursor_update = true;
                state->keyboard_key = 0;
                state->keyboard_update = true;
                break;
            case PORT_A_KEY_LEFT:
                if (state->keyboard_key > 1)
                {
                    state->keyboard_key--;
                    state->keyboard_update = true;
                }
                break;
            case PORT_A_KEY_RIGHT:
                if (state->keyboard_key < 29)
                {
                    state->keyboard_key++;
                    state->keyboard_update = true;
                }
                break;
            default:
                break;
        }
    }
    else
    {
        uint8_t element_was = state->current_element;
        const gui_element_t *element = &state->gui [state->current_element];

        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
                state->current_element = element->up;
                break;

            case PORT_A_KEY_DOWN:
                if (element->down == ELEMENT_KEYBOARD)
                {
                    /* Hide the cursor */
                    SMS_initSprites ();
                    SMS_copySpritestoSAT ();

                    /* Select a key */
                    state->keyboard_key = element->x - 1;
                    state->keyboard_update = true;
                }
                state->current_element = element->down;
                break;

            case PORT_A_KEY_LEFT:
                state->current_element = element->left;
                break;

            case PORT_A_KEY_RIGHT:
                state->current_element = element->right;
                break;

            default:
                break;
        }

        /* "BUTTON" elements turn off when unselected.
         * Note that our pointer, 'element', still points at the previous element */
        if (state->current_element != element_was)
        {
            if (element->type == TYPE_BUTTON)
            {
                state->element_values [element_was] = 0;
                element_update (element, 0);
            }
        }

        state->cursor_update = true;
    }
}


/*
 * Update the in-ram value of an element when a button is pressed or released.
 */
static void element_input (gui_state_t *state, uint16_t key_pressed, int16_t key_released)
{
    const gui_element_t *element = &state->gui [state->current_element];
    uint16_t *value = &state->element_values [state->current_element];

    if (element->type == TYPE_VALUE || element->type == TYPE_VALUE_WIDE)
    {
        if (key_pressed == PORT_A_KEY_1 && *value > 0)
        {
            *value -= 1;
            state->element_update = true;
        }
        else if (key_pressed == PORT_A_KEY_2 && *value < element->max)
        {
            *value += 1;
            state->element_update = true;
        }
    }
    else if (element->type == TYPE_LED)
    {
        if (key_pressed)
        {
            *value ^= 0x0001;
            state->element_update = true;
        }
    }
    else if (element->type == TYPE_BUTTON)
    {
        if (key_pressed)
        {
            *value = 1;
        }
        else if (key_released)
        {
            *value = 0;
        }
        state->element_update = true;
    }
}


/*
 * Frame interrupt, used to colour-cycle the cursor.
 */
static void frame_interrupt (void)
{
    static uint8_t frame = 0;
    static uint8_t hilight_index = 3;
    frame++;

    /* Simple 3-frame palette cycle to animate cursor */
    if ((frame & 0x07) == 0)
    {
        static int band = 3;

        SMS_setSpritePaletteColor (band, 2); /* Dim the previously bright band */
        band = (band == 1) ? 3 : band - 1;
        SMS_setSpritePaletteColor (band, 23); /* Brighten the new bright band */
    }

    /* Animate the cursor slide */
    cursor_tick ();
}


/*
 * Entry point.
 */
void main (void)
{
    /* Load palette */
    SMS_loadBGPalette (palette);
    SMS_loadSpritePalette (palette);
    SMS_setBackdropColor (0);

    SMS_loadTiles (patterns, 0, sizeof (patterns));
    SMS_useFirstHalfTilesforSprites (true);
    SMS_initSprites ();
    SMS_copySpritestoSAT ();

    gui_state_t gui_state = { .gui = psg_gui, .current_element = ELEMENT_CH0_VOLUME };

    draw_reset (0, 24);
    draw_title ();
    draw_footer ();

    /* Initialise value defaults */
    for (uint8_t i = ELEMENT_CH0_VOLUME; i < ELEMENT_KEYBOARD; i++)
    {
        const gui_element_t *element = &psg_gui [i];
        uint16_t value = value_defaults [i];

        gui_state.element_values [i] = value;

        if (element->callback)
        {
            element->callback (value);
        }
    }

    draw_labels ();

    /* Draw the GUI elements with their current values */
    for (uint8_t i = ELEMENT_CH0_VOLUME; i <= ELEMENT_NOISE_BUTTON; i++)
    {
        const gui_element_t *element = &gui_state.gui [i];
        uint16_t value = gui_state.element_values [i];

        if (element->type == TYPE_VALUE)
        {
            draw_value (element->x, element->y, value);
        }
        else if (element->type == TYPE_VALUE_WIDE)
        {
            draw_value_wide (element->x, element->y, value);
        }
        else if (element->type == TYPE_LED)
        {
            draw_led (element->x, element->y, value);
        }
        else if (element->type == TYPE_BUTTON)
        {
            draw_button (element->x, element->y, value);
        }
    }

    draw_keyboard ();

    cursor_target (gui_state.gui [gui_state.current_element].cursor_x,
                   gui_state.gui [gui_state.current_element].cursor_y,
                   gui_state.gui [gui_state.current_element].cursor_w,
                   gui_state.gui [gui_state.current_element].cursor_h);

    SMS_setFrameInterruptHandler (frame_interrupt);
    SMS_displayOn ();

    /* Main loop */
    while (true)
    {
        SMS_waitForVBlank ();
        uint16_t key_pressed = SMS_getKeysPressed ();
        uint16_t key_released = SMS_getKeysReleased ();

        /* Navigation */
        switch (key_pressed & PORT_A_DPAD_MASK)
        {
            case PORT_A_KEY_UP:
            case PORT_A_KEY_DOWN:
            case PORT_A_KEY_LEFT:
            case PORT_A_KEY_RIGHT:
                element_navigate (&gui_state, key_pressed);
                break;
        }

        /* Button input */
        switch ((key_pressed | key_released) & PORT_A_KEY_MASK)
        {
            case PORT_A_KEY_1:
            case PORT_A_KEY_2:
                element_input (&gui_state, key_pressed, key_released);
                break;

            default:
                break;
        }

        if (gui_state.cursor_update)
        {
            cursor_target (gui_state.gui [gui_state.current_element].cursor_x,
                           gui_state.gui [gui_state.current_element].cursor_y,
                           gui_state.gui [gui_state.current_element].cursor_w,
                           gui_state.gui [gui_state.current_element].cursor_h);
            gui_state.cursor_update = false;
        }

        if (gui_state.keyboard_update)
        {
            static uint8_t previous_key = 0;

            if (previous_key > 0)
            {
                draw_keyboard_update (previous_key - 1, false);
            }

            if (gui_state.current_element == ELEMENT_KEYBOARD)
            {
                uint16_t note = notes [gui_state.keyboard_key - 1];
                draw_keyboard_update (gui_state.keyboard_key - 1, true);

                for (int channel = 0; channel < (CHANNEL_OFFSET * 3); channel += CHANNEL_OFFSET)
                {
                    if (gui_state.element_values [ELEMENT_CH0_MODE_KEYBOARD + channel])
                    {
                        gui_state.element_values [ELEMENT_CH0_FREQUENCY + channel] = note;
                        element_update (&gui_state.gui [ELEMENT_CH0_FREQUENCY + channel], note);
                    }
                }
            }
            else
            {
                if (gui_state.element_values [ELEMENT_CH0_MODE_KEYBOARD])
                {
                    element_update (&gui_state.gui [ELEMENT_CH0_BUTTON], false);
                }
                if (gui_state.element_values [ELEMENT_CH1_MODE_KEYBOARD])
                {
                    element_update (&gui_state.gui [ELEMENT_CH1_BUTTON], false);
                }
                if (gui_state.element_values [ELEMENT_CH2_MODE_KEYBOARD])
                {
                    element_update (&gui_state.gui [ELEMENT_CH2_BUTTON], false);
                }
                if (gui_state.element_values [ELEMENT_NOISE_MODE_KEYBOARD])
                {
                    element_update (&gui_state.gui [ELEMENT_NOISE_BUTTON], false);
                }
            }

            previous_key = gui_state.keyboard_key;
            gui_state.keyboard_update = false;
        }

        /* Key-down / key-up events on the keyboard */
        if (gui_state.current_element == ELEMENT_KEYBOARD)
        {
            for (int channel = 0; channel < (CHANNEL_OFFSET * 4); channel += CHANNEL_OFFSET)
            {
                if (gui_state.element_values [ELEMENT_CH0_MODE_KEYBOARD + channel])
                {
                    if (key_pressed == PORT_A_KEY_1)
                    {
                        element_update (&gui_state.gui [ELEMENT_CH0_BUTTON + channel], true);
                    }
                    if (key_released == PORT_A_KEY_1)
                    {
                        element_update (&gui_state.gui [ELEMENT_CH0_BUTTON + channel], false);
                    }
                }
            }
        }

        if (gui_state.element_update)
        {
            const gui_element_t *element = &gui_state.gui [gui_state.current_element];
            uint16_t value = gui_state.element_values [gui_state.current_element];

            /* Special cases: Elements that affect other elements, such as changing mode. */
            if (gui_state.current_element == ELEMENT_CH0_MODE_KEYBOARD && value)
            {
                gui_state.element_values [ELEMENT_CH0_MODE_CONSTANT] = false;
                element_update (&gui_state.gui [ELEMENT_CH0_MODE_CONSTANT], false);
            }
            else if (gui_state.current_element == ELEMENT_CH0_MODE_CONSTANT && value)
            {
                gui_state.element_values [ELEMENT_CH0_MODE_KEYBOARD] = false;
                element_update (&gui_state.gui [ELEMENT_CH0_MODE_KEYBOARD], false);
            }
            else if (gui_state.current_element == ELEMENT_CH1_MODE_KEYBOARD && value)
            {
                gui_state.element_values [ELEMENT_CH1_MODE_CONSTANT] = false;
                element_update (&gui_state.gui [ELEMENT_CH1_MODE_CONSTANT], false);
            }
            else if (gui_state.current_element == ELEMENT_CH1_MODE_CONSTANT && value)
            {
                gui_state.element_values [ELEMENT_CH1_MODE_KEYBOARD] = false;
                element_update (&gui_state.gui [ELEMENT_CH1_MODE_KEYBOARD], false);
            }
            else if (gui_state.current_element == ELEMENT_CH2_MODE_KEYBOARD && value)
            {
                gui_state.element_values [ELEMENT_CH2_MODE_CONSTANT] = false;
                element_update (&gui_state.gui [ELEMENT_CH2_MODE_CONSTANT], false);
            }
            else if (gui_state.current_element == ELEMENT_CH2_MODE_CONSTANT && value)
            {
                gui_state.element_values [ELEMENT_CH2_MODE_KEYBOARD] = false;
                element_update (&gui_state.gui [ELEMENT_CH2_MODE_KEYBOARD], false);
            }
            else if (gui_state.current_element == ELEMENT_NOISE_MODE_KEYBOARD && value)
            {
                gui_state.element_values [ELEMENT_NOISE_MODE_CONSTANT] = false;
                element_update (&gui_state.gui [ELEMENT_NOISE_MODE_CONSTANT], false);
            }
            else if (gui_state.current_element == ELEMENT_NOISE_MODE_CONSTANT && value)
            {
                gui_state.element_values [ELEMENT_NOISE_MODE_KEYBOARD] = false;
                element_update (&gui_state.gui [ELEMENT_NOISE_MODE_KEYBOARD], false);
            }

            element_update (element, value);
            gui_state.element_update = false;
        }
    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
