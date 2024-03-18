/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef TARGET_SG
#include "SGlib.h"
#include "SMSlib_compat.h"
#include "../tile_data/colour_table.h"
#else
#include "SMSlib.h"
#include "../tile_data/palette.h"
#endif

__sfr __at 0xbf vdp_control_port;

#define PORT_A_DPAD_MASK    (PORT_A_KEY_UP | PORT_A_KEY_DOWN | PORT_A_KEY_LEFT | PORT_A_KEY_RIGHT)
#define PORT_A_KEY_MASK     (PORT_A_KEY_1 | PORT_A_KEY_2)

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

gui_state_t gui_state = { .gui = psg_gui, .current_element = ELEMENT_CH0_VOLUME };

static const uint16_t value_defaults [ELEMENT_COUNT] = {
    [ELEMENT_CH0_VOLUME] = 0,
    [ELEMENT_CH0_MODE_KEYBOARD] = 1,
    [ELEMENT_CH0_MODE_CONSTANT] = 0,
    [ELEMENT_CH0_BUTTON] = 0,
    [ELEMENT_CH1_VOLUME] = 0,
    [ELEMENT_CH1_MODE_KEYBOARD] = 0,
    [ELEMENT_CH1_MODE_CONSTANT] = 0,
    [ELEMENT_CH1_BUTTON] = 0,
    [ELEMENT_CH2_VOLUME] = 0,
    [ELEMENT_CH2_MODE_KEYBOARD] = 0,
    [ELEMENT_CH2_MODE_CONSTANT] = 0,
    [ELEMENT_CH2_BUTTON] = 0,
    [ELEMENT_NOISE_VOLUME] = 0,
    [ELEMENT_NOISE_MODE_KEYBOARD] = 0,
    [ELEMENT_NOISE_MODE_CONSTANT] = 0,
    [ELEMENT_NOISE_CONTROL] = 4,
    [ELEMENT_NOISE_BUTTON] = 0,
#ifdef TARGET_PAL
    [ELEMENT_CH0_FREQUENCY] = 424,
    [ELEMENT_CH1_FREQUENCY] = 336,
    [ELEMENT_CH2_FREQUENCY] = 283,
#else
    [ELEMENT_CH0_FREQUENCY] = 428,
    [ELEMENT_CH1_FREQUENCY] = 339,
    [ELEMENT_CH2_FREQUENCY] = 285,
#endif
#ifdef TARGET_GG
    [ELEMENT_CH0_STEREO_LEFT] = 1,
    [ELEMENT_CH0_STEREO_RIGHT] = 1,
    [ELEMENT_CH1_STEREO_LEFT] = 1,
    [ELEMENT_CH1_STEREO_RIGHT] = 1,
    [ELEMENT_CH2_STEREO_LEFT] = 1,
    [ELEMENT_CH2_STEREO_RIGHT] = 1,
    [ELEMENT_NOISE_STEREO_LEFT] = 1,
    [ELEMENT_NOISE_STEREO_RIGHT] = 1,
#endif
};


/*
 * Frequency values for keyboard notes, from C3 to E5.
 */
uint16_t notes [29] = {
#ifdef TARGET_PAL
    847, 800, 755, 713, 673, 635, 599, 566,
    534, 504, 476, 449, 424, 400, 377, 356,
    336, 317, 300, 283, 267, 252, 238, 224,
    212, 200, 189, 178, 168
#else /* NTSC */
    855, 807, 762, 719, 679, 641, 605, 571,
    539, 508, 480, 453, 428, 404, 381, 360,
    339, 320, 302, 285, 269, 254, 240, 226,
    214, 202, 190, 180, 170
#endif
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
static void element_navigate (uint16_t key_pressed)
{
    if (gui_state.current_element == ELEMENT_KEYBOARD)
    {
        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
#ifdef TARGET_GG
                if      (gui_state.keyboard_key <  8) gui_state.current_element = ELEMENT_NOISE_VOLUME;
                else if (gui_state.keyboard_key < 13) gui_state.current_element = ELEMENT_NOISE_MODE_CONSTANT;
                else if (gui_state.keyboard_key < 18) gui_state.current_element = ELEMENT_NOISE_CONTROL;
                else if (gui_state.keyboard_key < 21) gui_state.current_element = ELEMENT_NOISE_STEREO_RIGHT;
#else
                if      (gui_state.keyboard_key <  7) gui_state.current_element = ELEMENT_NOISE_VOLUME;
                else if (gui_state.keyboard_key < 11) gui_state.current_element = ELEMENT_NOISE_MODE_KEYBOARD;
                else if (gui_state.keyboard_key < 16) gui_state.current_element = ELEMENT_NOISE_MODE_CONSTANT;
                else if (gui_state.keyboard_key < 21) gui_state.current_element = ELEMENT_NOISE_CONTROL;
#endif
                else                                  gui_state.current_element = ELEMENT_NOISE_BUTTON;
                gui_state.cursor_update = true;
                gui_state.keyboard_key = 0;
                gui_state.keyboard_update = true;
                break;
            case PORT_A_KEY_LEFT:
#ifdef TARGET_GG
                if (gui_state.keyboard_key > 6)
#else
                if (gui_state.keyboard_key > 1)
#endif
                {
                    gui_state.keyboard_key--;
                    gui_state.keyboard_update = true;
                }
                break;
            case PORT_A_KEY_RIGHT:
#ifdef TARGET_GG
                if (gui_state.keyboard_key < 22)
#else
                if (gui_state.keyboard_key < 29)
#endif
                {
                    gui_state.keyboard_key++;
                    gui_state.keyboard_update = true;
                }
                break;
            default:
                break;
        }
    }
    else
    {
        uint8_t element_was = gui_state.current_element;
        const gui_element_t *element = &gui_state.gui [gui_state.current_element];

        switch (key_pressed)
        {
            case PORT_A_KEY_UP:
                gui_state.current_element = element->up;
                break;

            case PORT_A_KEY_UP | PORT_A_KEY_RIGHT:
                gui_state.current_element = gui_state.gui [element->right].up;
                break;

            case PORT_A_KEY_RIGHT:
                gui_state.current_element = element->right;
                break;

            case PORT_A_KEY_DOWN | PORT_A_KEY_RIGHT:
                gui_state.current_element = gui_state.gui [element->right].down;
                break;

            case PORT_A_KEY_DOWN:
                gui_state.current_element = element->down;
                break;

            case PORT_A_KEY_DOWN | PORT_A_KEY_LEFT:
                gui_state.current_element = gui_state.gui [element->left].down;
                break;

            case PORT_A_KEY_LEFT:
                gui_state.current_element = element->left;
                break;

            case PORT_A_KEY_UP | PORT_A_KEY_LEFT:
                gui_state.current_element = gui_state.gui [element->left].up;
                break;

            default:
                break;
        }

        if (gui_state.current_element != element_was)
        {
            /* Moving to the keyboard should hide the cursor */
            if (gui_state.current_element == ELEMENT_KEYBOARD)
            {
                /* Hide the cursor */
                SMS_initSprites ();
                SMS_copySpritestoSAT ();

                /* Select a key */
                gui_state.keyboard_key = element->x - 1;
                gui_state.keyboard_update = true;
            }

            /* "BUTTON" elements turn off when unselected. Note that
             * our pointer, 'element', still points at the previous element */
            if (element->type == TYPE_BUTTON)
            {
                gui_state.element_values [element_was] = 0;
                element_update (element, 0);
            }
        }

        gui_state.cursor_update = true;
    }
}


/*
 * Update the in-ram value of an element when a button is pressed or released.
 */
static void element_input (uint16_t key_pressed, int16_t key_released)
{
    const gui_element_t *element = &gui_state.gui [gui_state.current_element];
    uint16_t *value = &gui_state.element_values [gui_state.current_element];

    if (element->type == TYPE_VALUE || element->type == TYPE_VALUE_WIDE)
    {
        if (key_pressed == PORT_A_KEY_1 && *value > 0)
        {
            *value -= 1;
            gui_state.element_update = true;
        }
        else if (key_pressed == PORT_A_KEY_2 && *value < element->max)
        {
            *value += 1;
            gui_state.element_update = true;
        }
    }
    else if (element->type == TYPE_LED)
    {
        if (key_pressed)
        {
            *value ^= 0x0001;
            gui_state.element_update = true;
        }
    }
    else if (element->type == TYPE_BUTTON)
    {
        uint16_t key_status = SMS_getKeysStatus ();

        if (key_pressed)
        {
            *value = 1;
        }
        else if (key_released && (key_status & PORT_A_KEY_MASK) == 0)
        {
            *value = 0;
        }
        gui_state.element_update = true;
    }
}


/*
 * When changing element values, repeat the button press if held down.
 * Start repeating at 500 ms. Repeat 30 times per second.
 */
static void key_repeat (void)
{
    static uint8_t start_timer = 0;
    static uint8_t repeat_timer = 0;

    /* When the input changes, just reset the timer */
    if (SMS_getKeysPressed () || SMS_getKeysReleased ())
    {
        start_timer = 0;
        repeat_timer = 0;
        return;
    }

    /* Repeat should only affect value inputs */
    const gui_element_t *element = &gui_state.gui [gui_state.current_element];
    if (element->type != TYPE_VALUE && element->type != TYPE_VALUE_WIDE)
    {
        return;
    }

    /* If exactly one button is held down, run the timer */
    uint16_t key_status = SMS_getKeysStatus () & PORT_A_KEY_MASK;
    if (key_status == PORT_A_KEY_1 || key_status == PORT_A_KEY_2)
    {
        if (start_timer == 30) /* 30 frames for 500 ms start time */
        {
            if (++repeat_timer & 0x01) /* Every second frame for 30 repeats per second */
            {
                element_input (key_status, 0);
            }
        }
        else
        {
            start_timer++;
        }
    }
}


/*
 * Frame interrupt, used to colour-cycle the cursor.
 */
static void frame_interrupt (void)
{
#ifndef TARGET_SG
    static uint8_t frame = 0;
    frame++;

    /* Simple 3-frame palette cycle to animate cursor */
    if ((frame & 0x07) == 0)
    {
        static int band = 3;

#ifdef TARGET_SMS
        SMS_setSpritePaletteColor (band, RGB (2, 0, 0));    /* Dark Red for the previously bright band */
        band = (band == 1) ? 3 : band - 1;
        SMS_setSpritePaletteColor (band, RGB (3, 1, 1));    /* Light Red for the new bright band */
#elif defined (TARGET_GG)
        GG_setSpritePaletteColor (band, RGB (10, 0, 0));     /* Dark Red for the previously bright band */
        band = (band == 1) ? 3 : band - 1;
        GG_setSpritePaletteColor (band, RGB (15, 5, 5));    /* Light Red for the new bright band */
#endif
    }
#endif

    /* Animate the cursor slide */
    cursor_tick ();

    key_repeat ();
}


/*
 * Entry point.
 */
void main (void)
{
    /* Configure VDP and load data to VRAM */
#ifdef TARGET_SG
    vdp_control_port = 0x00; /* Mode 0 */
    vdp_control_port = 0x80;
    vdp_control_port = 0x80; /* Colour table at 0x2000 */
    vdp_control_port = 0x83;
    vdp_control_port = 0x00; /* Pattern table at 0x0000 */
    vdp_control_port = 0x84;

    SG_loadTilePatterns (patterns, 0, sizeof (patterns));
    SG_loadTileColours (colour_table, 0, sizeof (colour_table));
    SG_loadSpritePatterns (&patterns [PATTERN_CURSOR_TMS * 2], 0, 72);
    SG_setBackdropColor (12);                       /* Dark green */
#elif defined (TARGET_SMS)
    SMS_loadBGPalette (palette);
    SMS_loadSpritePalette (palette);
    SMS_setSpritePaletteColor (1, RGB (2, 0, 0));   /* Dark Red */
    SMS_setSpritePaletteColor (2, RGB (2, 0, 0));   /* Dark Red */
    SMS_setSpritePaletteColor (3, RGB (3, 1, 1));   /* Light Red */
    SMS_setBGPaletteColor (4, RGB (2, 2, 3));       /* Light Lavender */
    SMS_setBackdropColor (0);
    SMS_loadTiles (patterns, 0, sizeof (patterns));
    SMS_useFirstHalfTilesforSprites (true);
#elif defined (TARGET_GG)
    GG_loadBGPalette (palette);
    GG_loadSpritePalette (palette);
    GG_setSpritePaletteColor (1, RGB (8, 0, 0));    /* Dark Red */
    GG_setSpritePaletteColor (2, RGB (8, 0, 0));    /* Dark Red */
    GG_setSpritePaletteColor (3, RGB (15, 4, 4));   /* Light Red */
    GG_setBGPaletteColor (4, RGB (12, 12,  15));    /* Light Lavender */
    SMS_setBackdropColor (0);
    SMS_loadTiles (patterns, 0, sizeof (patterns));
    SMS_useFirstHalfTilesforSprites (true);
#endif

    SMS_initSprites ();
    SMS_copySpritestoSAT ();

    draw_reset (0, 24);

    draw_title ();

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
    draw_labels ();
    draw_footer ();

    cursor_target (gui_state.gui [gui_state.current_element].cursor_x,
                   gui_state.gui [gui_state.current_element].cursor_y,
                   gui_state.gui [gui_state.current_element].cursor_w,
                   gui_state.gui [gui_state.current_element].cursor_h);

#ifndef TARGET_SG
    SMS_setFrameInterruptHandler (frame_interrupt);
#endif

    SMS_displayOn ();

    /* Main loop */
    while (true)
    {
        SMS_waitForVBlank ();
#ifdef TARGET_SG
        frame_interrupt ();
#endif

        uint16_t key_pressed = SMS_getKeysPressed ();
        uint16_t key_released = SMS_getKeysReleased ();
        uint16_t key_status = SMS_getKeysStatus ();

        /* Navigation */
        if (key_pressed & PORT_A_DPAD_MASK)
        {
            element_navigate (key_pressed);
        }

        /* Button input */
        if ((key_pressed | key_released) & PORT_A_KEY_MASK)
        {
            element_input (key_pressed & PORT_A_KEY_MASK, key_released & PORT_A_KEY_MASK);
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

                for (int channel = 0; channel < (ELEMENTS_PER_CHANNEL * 3); channel += ELEMENTS_PER_CHANNEL)
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
#ifdef TARGET_SMS
                SMS_setBGPaletteColor (4, RGB (2, 2, 3));       /* Light Lavender */
#elif defined (TARGET_GG)
                GG_setBGPaletteColor (4, RGB (10, 10,  15));    /* Light Lavender */
#endif
            }

            previous_key = gui_state.keyboard_key;
            gui_state.keyboard_update = false;
        }

        /* Key-down / key-up events on the keyboard */
        if (gui_state.current_element == ELEMENT_KEYBOARD)
        {
#ifndef TARGET_SG
            if (key_pressed & PORT_A_KEY_MASK)
            {
#ifdef TARGET_SMS
                SMS_setBGPaletteColor (4, RGB (1, 1, 2));       /* Dark Lavender */
#elif defined (TARGET_GG)
                GG_setBGPaletteColor (4, RGB (5, 5, 10));    /* Dark Lavender */
#endif
            }
            else if ((key_released & PORT_A_KEY_MASK) && (key_status & PORT_A_KEY_MASK) == 0)
            {
#ifdef TARGET_SMS
                SMS_setBGPaletteColor (4, RGB (2, 2, 3));       /* Light Lavender */
#elif defined (TARGET_GG)
                GG_setBGPaletteColor (4, RGB (10, 10, 15));    /* Light Lavender */
#endif
            }
#endif

            for (int channel = 0; channel < (ELEMENTS_PER_CHANNEL * 4); channel += ELEMENTS_PER_CHANNEL)
            {
                if (gui_state.element_values [ELEMENT_CH0_MODE_KEYBOARD + channel])
                {
                    if (key_pressed & PORT_A_KEY_MASK)
                    {
                        element_update (&gui_state.gui [ELEMENT_CH0_BUTTON + channel], true);
                    }
                    else if ((key_released & PORT_A_KEY_MASK) && (key_status & PORT_A_KEY_MASK) == 0)
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

#if defined (TARGET_SMS) || defined (TARGET_GG)
SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
#endif
