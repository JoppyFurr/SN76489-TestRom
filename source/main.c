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
#include "register.h"
#include "gui_elements.h"
#include "draw.h"


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
    [ELEMENT_CH0_KEYBOARD_MODE] = 1,
    [ELEMENT_CH0_CONST_MODE] = 0,
    [ELEMENT_CH0_FREQUENCY] = 500,
    [ELEMENT_CH0_BUTTON] = 0,
    [ELEMENT_CH1_VOLUME] = 15,
    [ELEMENT_CH1_KEYBOARD_MODE] = 0,
    [ELEMENT_CH1_CONST_MODE] = 0,
    [ELEMENT_CH1_FREQUENCY] = 500,
    [ELEMENT_CH1_BUTTON] = 0,
    [ELEMENT_CH2_VOLUME] = 15,
    [ELEMENT_CH2_KEYBOARD_MODE] = 0,
    [ELEMENT_CH2_CONST_MODE] = 0,
    [ELEMENT_CH2_FREQUENCY] = 500,
    [ELEMENT_CH2_BUTTON] = 0,
    [ELEMENT_NOISE_VOLUME] = 15,
    [ELEMENT_NOISE_KEYBOARD_MODE] = 0,
    [ELEMENT_NOISE_CONST_MODE] = 0,
    [ELEMENT_NOISE_CONTROL] = 0,
    [ELEMENT_NOISE_BUTTON] = 0,
};

uint16_t notes [29] = {
    0 /* TODO */
};


/*
 * Update the cursor position.
 */
static void cursor_update (uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    /* Clear any previous sprites */
    SMS_initSprites ();

    /* A height of zero indicates that we do not want to draw the cursor */
    if (height > 0)
    {
        x = (x << 3) - 8;
        y = (y << 3) - 4;

        /* Top */
        SMS_addSprite (x, y, PATTERN_CURSOR + 0);
        for (uint8_t i = 1; i < width; i++)
        {
            SMS_addSprite (x + (i << 3), y, PATTERN_CURSOR + 1);
        }
        SMS_addSprite (x + (width << 3), y, PATTERN_CURSOR + 2);

        /* Sides */
        for (uint8_t i = 1; i < height; i++)
        {
            SMS_addSprite (x,                y + (i << 3), PATTERN_CURSOR + 3);
            SMS_addSprite (x + (width << 3), y + (i << 3), PATTERN_CURSOR + 5);
        }

        /* Bottom */
        SMS_addSprite (x, y + (height << 3), PATTERN_CURSOR + 6);
        for (uint8_t i = 1; i < width; i++)
        {
            SMS_addSprite (x + (i << 3), y + (height << 3), PATTERN_CURSOR + 7);
        }
        SMS_addSprite (x + (width << 3), y + (height << 3), PATTERN_CURSOR + 8);
    }

    SMS_copySpritestoSAT ();
}


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
                else if (state->keyboard_key < 11 ) state->current_element = ELEMENT_NOISE_KEYBOARD_MODE;
                else if (state->keyboard_key < 16) state->current_element = ELEMENT_NOISE_CONST_MODE;
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
    SMS_setBGScrollX (0xfc);

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

    cursor_update (gui_state.gui [gui_state.current_element].x,
                   gui_state.gui [gui_state.current_element].y,
                   gui_state.gui [gui_state.current_element].width,
                   gui_state.gui [gui_state.current_element].height);

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
            cursor_update (gui_state.gui [gui_state.current_element].x,
                           gui_state.gui [gui_state.current_element].y,
                           gui_state.gui [gui_state.current_element].width,
                           gui_state.gui [gui_state.current_element].height);
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
                draw_keyboard_update (gui_state.keyboard_key - 1, true);

                uint16_t note = notes [gui_state.keyboard_key - 1];
                /* TODO */
            }
            else
            {
                /* Treat leaving the keyboard as letting go of the key */
                /* TODO */
            }

            previous_key = gui_state.keyboard_key;
            gui_state.keyboard_update = false;
        }

        if (gui_state.element_update)
        {
            const gui_element_t *element = &gui_state.gui [gui_state.current_element];
            uint16_t value = gui_state.element_values [gui_state.current_element];

            element_update (element, value);

            gui_state.element_update = false;
        }

        if (gui_state.current_element == ELEMENT_KEYBOARD)
        {
            /* TODO */
        }
    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
