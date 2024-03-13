/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "SMSlib.h"

#include "../tile_data/pattern_index.h"

const uint16_t keyboard_upper_active [12] = {
    PATTERN_KEYS + 10, PATTERN_KEYS + 18, PATTERN_KEYS + 12, PATTERN_KEYS + 18,
    PATTERN_KEYS + 12, PATTERN_KEYS + 10, PATTERN_KEYS + 18, PATTERN_KEYS + 12,
    PATTERN_KEYS + 18, PATTERN_KEYS + 12, PATTERN_KEYS + 18, PATTERN_KEYS + 12
};

const uint16_t keyboard_upper_inactive [12] = {
    PATTERN_KEYS + 2, PATTERN_KEYS + 8, PATTERN_KEYS + 4, PATTERN_KEYS + 8,
    PATTERN_KEYS + 4, PATTERN_KEYS + 2, PATTERN_KEYS + 8, PATTERN_KEYS + 4,
    PATTERN_KEYS + 8, PATTERN_KEYS + 4, PATTERN_KEYS + 8, PATTERN_KEYS + 4
};

const uint16_t keyboard_mid_active [12] = {
    PATTERN_KEYS + 10, PATTERN_KEYS + 20, PATTERN_KEYS + 12, PATTERN_KEYS + 20,
    PATTERN_KEYS + 12, PATTERN_KEYS + 10, PATTERN_KEYS + 20, PATTERN_KEYS + 12,
    PATTERN_KEYS + 20, PATTERN_KEYS + 12, PATTERN_KEYS + 20, PATTERN_KEYS + 12
};

const uint16_t keyboard_lower_inactive [12] = {
    PATTERN_KEYS + 2, PATTERN_KEYS + 3, PATTERN_KEYS + 4, PATTERN_KEYS + 5,
    PATTERN_KEYS + 4, PATTERN_KEYS + 2, PATTERN_KEYS + 6, PATTERN_KEYS + 4,
    PATTERN_KEYS + 7, PATTERN_KEYS + 4, PATTERN_KEYS + 5, PATTERN_KEYS + 4
};

const bool key_extends_left [12] = {
    false, false, true, false, true,
    false, false, true, false, true, false, true
};

const bool key_extends_right [12] = {
    true, false, true, false, false,
    true, false, true, false, true, false, false
};


/*
 * Draw a button indicator.
 */
void draw_button (uint8_t x, uint8_t y, bool value)
{
    const uint16_t button [8] = {
        PATTERN_BUTTON + 0, PATTERN_BUTTON + 1, PATTERN_BUTTON + 2, PATTERN_BUTTON + 3,
        PATTERN_BUTTON + 4, PATTERN_BUTTON + 5, PATTERN_BUTTON + 6, PATTERN_BUTTON + 7
    };

    SMS_loadTileMapArea (x, y, value ? &button [4] : &button [0], 2, 2);
}


/*
 * Draw the footer banner at the bottom of the screen.
 */
void draw_footer (void)
{
#ifdef TARGET_SMS
    const uint16_t name [5] = {
        PATTERN_FOOTER + 0, PATTERN_FOOTER + 1, PATTERN_FOOTER + 2,
        PATTERN_FOOTER + 3, PATTERN_FOOTER + 4
    };
    SMS_loadTileMapArea (0, 23, name, 5, 1);

    const uint16_t version [3] = {
        PATTERN_FOOTER + 5, PATTERN_FOOTER + 6, PATTERN_FOOTER + 7,
    };
    SMS_loadTileMapArea (29, 23, version, 3, 1);
#elif defined (TARGET_GG)
    const uint16_t name [5] = {
        PATTERN_FOOTER_GG + 0, PATTERN_FOOTER_GG + 1, PATTERN_FOOTER_GG + 2,
        PATTERN_FOOTER_GG + 3, PATTERN_FOOTER_GG + 4
    };
    SMS_loadTileMapArea (6, 20, name, 5, 1);

    const uint16_t version [3] = {
        PATTERN_FOOTER_GG + 5, PATTERN_FOOTER_GG + 6, PATTERN_FOOTER_GG + 7,
    };
    SMS_loadTileMapArea (23, 20, version, 3, 1);
#endif
}

#ifdef TARGET_SMS

#define KEYBOARD_X_START    2
#define KEYBOARD_X_END     30
#define KEYBOARD_Y_START   17
#define KEYBOARD_Y_END     21

#elif defined (TARGET_GG)

#define KEYBOARD_X_START    8
#define KEYBOARD_X_END     24
#define KEYBOARD_Y_START   17
#define KEYBOARD_Y_END     19

#endif

/*
 * Initialise the keyboard display.
 */
void draw_keyboard (void)
{
    uint16_t key_tile;

    for (uint8_t col = KEYBOARD_X_START; col <= KEYBOARD_X_END; col++)
    {
        /* Top outline */
        SMS_setTileatXY (col, KEYBOARD_Y_START - 1, PATTERN_KEYS + 0);

#ifdef TARGET_SMS
        /* Top section of keys */
        key_tile = keyboard_upper_inactive [(col - 2) % 12];
        SMS_setTileatXY (col, 17, key_tile);
        SMS_setTileatXY (col, 18, key_tile);
        SMS_setTileatXY (col, 19, key_tile);

        /* Bottom section of keys */
        key_tile = keyboard_lower_inactive [(col - 2) % 12];
        SMS_setTileatXY (col, 20, key_tile);
        SMS_setTileatXY (col, 21, key_tile);
#elif defined (TARGET_GG)
        /* Top section of keys */
        key_tile = keyboard_upper_inactive [(col - 3) % 12];
        SMS_setTileatXY (col, 17, key_tile);
        SMS_setTileatXY (col, 18, key_tile);

        /* Bottom section of keys */
        key_tile = keyboard_lower_inactive [(col - 3) % 12];
        SMS_setTileatXY (col, 19, key_tile);
#endif

        /* Bottom outline */
        SMS_setTileatXY (col, KEYBOARD_Y_END + 1, PATTERN_KEYS + 16);
    }

    /* Right outline */
    SMS_setTileatXY (KEYBOARD_X_END + 1, KEYBOARD_Y_START - 1, PATTERN_KEYS + 1);
    for (uint8_t row = KEYBOARD_Y_START; row <= KEYBOARD_Y_END; row++)
    {
        SMS_setTileatXY (KEYBOARD_X_END + 1, row, PATTERN_KEYS + 9);
    }
    SMS_setTileatXY (KEYBOARD_X_END + 1, KEYBOARD_Y_END + 1, PATTERN_KEYS + 17);
}


/*
 * Update the display of a key to be either active or inactive.
 * Assumes that only one key will ever be active at a time.
 */
void draw_keyboard_update (uint8_t key, bool active)
{
    uint16_t key_tile;

    /* Top section */
    if (active)
    {
        key_tile = keyboard_upper_active [key % 12];
    }
    else
    {
        key_tile = keyboard_upper_inactive [key % 12];
    }

#ifdef TARGET_SMS
    SMS_setTileatXY (key + 2, 17, key_tile);
    SMS_setTileatXY (key + 2, 18, key_tile);

    /* Mid section */
    if (active)
    {
        key_tile = keyboard_mid_active [key % 12];
    }
    SMS_setTileatXY (key + 2, 19, key_tile);

    /* Bottom section */
    if (key_extends_left [key % 12] || key_extends_right [key % 12])
    {
        if (active)
        {
            switch (key % 12)
            {
                case 0:
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 10);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 10);
                    SMS_setTileatXY (key + 3, 20, PATTERN_KEYS + 11);
                    SMS_setTileatXY (key + 3, 21, PATTERN_KEYS + 11);
                    break;
                case 2:
                    SMS_setTileatXY (key + 1, 20, PATTERN_KEYS + 19);
                    SMS_setTileatXY (key + 1, 21, PATTERN_KEYS + 19);
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 3, 20, PATTERN_KEYS + 13);
                    SMS_setTileatXY (key + 3, 21, PATTERN_KEYS + 13);
                    break;
                case 4:
                case 11:
                    SMS_setTileatXY (key + 1, 20, PATTERN_KEYS + 21);
                    SMS_setTileatXY (key + 1, 21, PATTERN_KEYS + 21);
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 12);
                    break;
                case 5:
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 10);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 10);
                    SMS_setTileatXY (key + 3, 20, PATTERN_KEYS + 14);
                    SMS_setTileatXY (key + 3, 21, PATTERN_KEYS + 14);
                    break;
                case 7:
                    SMS_setTileatXY (key + 1, 20, PATTERN_KEYS + 22);
                    SMS_setTileatXY (key + 1, 21, PATTERN_KEYS + 22);
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 3, 20, PATTERN_KEYS + 15);
                    SMS_setTileatXY (key + 3, 21, PATTERN_KEYS + 15);
                    break;
                case 9:
                    SMS_setTileatXY (key + 1, 20, PATTERN_KEYS + 23);
                    SMS_setTileatXY (key + 1, 21, PATTERN_KEYS + 23);
                    SMS_setTileatXY (key + 2, 20, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 2, 21, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 3, 20, PATTERN_KEYS + 13);
                    SMS_setTileatXY (key + 3, 21, PATTERN_KEYS + 13);
                    break;
            }
        }
        else
        {
            uint8_t from = ((key_extends_left  [key % 12]) ? key - 1 : key) + 2;
            uint8_t to   = ((key_extends_right [key % 12]) ? key + 1 : key) + 2;

            for (uint8_t col = from; col <= to; col++)
            {
                key_tile = keyboard_lower_inactive [(col - 2) % 12];
                SMS_setTileatXY (col, 20, key_tile);
                SMS_setTileatXY (col, 21, key_tile);
            }
        }
    }
#elif defined (TARGET_GG)

    /* Note: 1 = C, 5 is our leftmost key, an F. */

    SMS_setTileatXY (key + 3, 17, key_tile);

    /* Mid section */
    if (active)
    {
        key_tile = keyboard_mid_active [key % 12];
    }
    SMS_setTileatXY (key + 3, 18, key_tile);

    /* Bottom section */
    if (key_extends_left [key % 12] || key_extends_right [key % 12])
    {
        if (active)
        {
            /* Special case, as we end on an A on the Game Gear build. */
            if (key == 21)
            {
                SMS_setTileatXY (key + 2, 19, PATTERN_KEYS + 23);
                SMS_setTileatXY (key + 3, 19, PATTERN_KEYS + 12);
                return;
            }

            switch (key % 12)
            {
                case 0:
                    SMS_setTileatXY (key + 3, 19, PATTERN_KEYS + 10);
                    SMS_setTileatXY (key + 4, 19, PATTERN_KEYS + 11);
                    break;
                case 2:
                    SMS_setTileatXY (key + 2, 19, PATTERN_KEYS + 19);
                    SMS_setTileatXY (key + 3, 19, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 4, 19, PATTERN_KEYS + 13);
                    break;
                case 4:
                case 11:
                    SMS_setTileatXY (key + 2, 19, PATTERN_KEYS + 21);
                    SMS_setTileatXY (key + 3, 19, PATTERN_KEYS + 12);
                    break;
                case 5:
                    SMS_setTileatXY (key + 3, 19, PATTERN_KEYS + 10);
                    SMS_setTileatXY (key + 4, 19, PATTERN_KEYS + 14);
                    break;
                case 7:
                    SMS_setTileatXY (key + 2, 19, PATTERN_KEYS + 22);
                    SMS_setTileatXY (key + 3, 19, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 4, 19, PATTERN_KEYS + 15);
                    break;
                case 9:
                    SMS_setTileatXY (key + 2, 19, PATTERN_KEYS + 23);
                    SMS_setTileatXY (key + 3, 19, PATTERN_KEYS + 12);
                    SMS_setTileatXY (key + 4, 19, PATTERN_KEYS + 13);
                    break;
            }
        }
        else
        {
            uint8_t from = ((key_extends_left  [key % 12]) ? key - 1 : key) + 3;
            uint8_t to   = ((key_extends_right [key % 12]) ? key + 1 : key) + 3;

            for (uint8_t col = from; col <= to; col++)
            {
                /* Special case, as we end on an A on the Game Gear build. */
                if (col == 24)
                {
                    key_tile = PATTERN_KEYS + 4;
                    SMS_setTileatXY (col, 19, key_tile);
                    break;
                }
                else
                {
                    key_tile = keyboard_lower_inactive [(col - 3) % 12];
                }
                SMS_setTileatXY (col, 19, key_tile);
            }
        }
    }
#endif
}


/*
 * Draw the control labels for melody mode.
 */
void draw_labels (void)
{
#ifdef TARGET_SMS

    /* Tone configuration */
    const uint16_t tone_channel_settings [] = {
        PATTERN_LABELS +  0, PATTERN_LABELS +  1, PATTERN_LABELS +  2, PATTERN_LABELS +  3,
        PATTERN_LABELS +  4, PATTERN_LABELS +  5, PATTERN_LABELS +  6, PATTERN_LABELS +  7,
        PATTERN_LABELS +  8, PATTERN_LABELS +  9, PATTERN_LABELS + 10, PATTERN_LABELS + 11,
        PATTERN_LABELS + 12, PATTERN_LABELS + 13, PATTERN_LABELS + 14, PATTERN_LABELS + 15,
        PATTERN_LABELS + 16
    };
    SMS_loadTileMapArea (5,  5, tone_channel_settings,  17, 1);
    SMS_loadTileMapArea (5,  8, tone_channel_settings,  17, 1);
    SMS_loadTileMapArea (5, 11, tone_channel_settings,  17, 1);

    /* Noise configuration */
    const uint16_t noise_channel_settings [] = {
        PATTERN_LABELS +  0, PATTERN_LABELS +  1, PATTERN_LABELS +  2, PATTERN_LABELS +  3,
        PATTERN_LABELS +  4, PATTERN_LABELS +  5, PATTERN_LABELS +  6, PATTERN_LABELS +  7,
        PATTERN_LABELS +  8, PATTERN_LABELS +  9, PATTERN_LABELS + 10, PATTERN_LABELS + 17,
        PATTERN_LABELS + 18, PATTERN_LABELS + 19, PATTERN_LABELS + 20, PATTERN_LABELS + 21,
        PATTERN_LABELS + 22
    };
    SMS_loadTileMapArea (5, 14, noise_channel_settings, 17, 1);

    /* Buttons */
    const uint16_t tone_1_button [] = { PATTERN_LABELS + 23, PATTERN_LABELS + 24, PATTERN_LABELS + 25 };
    const uint16_t tone_2_button [] = { PATTERN_LABELS + 23, PATTERN_LABELS + 24, PATTERN_LABELS + 26 };
    const uint16_t tone_3_button [] = { PATTERN_LABELS + 23, PATTERN_LABELS + 24, PATTERN_LABELS + 27 };
    const uint16_t noise_button []  = { PATTERN_LABELS + 28, PATTERN_LABELS + 29, PATTERN_LABELS + 30 };
    SMS_loadTileMapArea (26,  3, tone_1_button, 3, 1);
    SMS_loadTileMapArea (26,  6, tone_2_button, 3, 1);
    SMS_loadTileMapArea (26,  9, tone_3_button, 3, 1);
    SMS_loadTileMapArea (26, 12, noise_button,  3, 1);

#elif defined (TARGET_GG)

    /* Volume */
    const uint16_t volume [] = { PATTERN_LABELS_GG + 0, PATTERN_LABELS_GG + 1, PATTERN_LABELS_GG + 2, PATTERN_LABELS_GG + 3 };
    const uint16_t volume_last [] = { PATTERN_LABELS_GG + 0, PATTERN_LABELS_GG + 1, PATTERN_LABELS_GG + 29, PATTERN_LABELS_GG + 30 };
    SMS_loadTileMapArea (6,  7, volume, 4, 1);
    SMS_loadTileMapArea (6, 10, volume, 4, 1);
    SMS_loadTileMapArea (6, 13, volume, 4, 1);
    SMS_loadTileMapArea (6, 16, volume_last, 4, 1);

    /* Keys / Const */
    const uint16_t keys_const [] = {
        PATTERN_LABELS_GG + 4, PATTERN_LABELS_GG + 5, PATTERN_EMPTY, PATTERN_LABELS_GG + 8,
        PATTERN_LABELS_GG + 9, PATTERN_LABELS_GG + 10
    };
    SMS_loadTileMapArea (12,  5, keys_const, 3, 2);
    SMS_loadTileMapArea (12,  8, keys_const, 3, 2);
    SMS_loadTileMapArea (12, 11, keys_const, 3, 2);
    SMS_loadTileMapArea (12, 14, keys_const, 3, 2);

    /* Frequency */
    const uint16_t frequency [] = {
        PATTERN_LABELS_GG + 11, PATTERN_LABELS_GG + 12, PATTERN_LABELS_GG + 13, PATTERN_LABELS_GG + 14,
        PATTERN_LABELS_GG + 15, PATTERN_LABELS_GG + 16
    };
    SMS_loadTileMapArea (14,  7, frequency, 6, 1);
    SMS_loadTileMapArea (14, 10, frequency, 6, 1);
    SMS_loadTileMapArea (14, 13, frequency, 6, 1);

    /* Noise Control */
    const uint16_t noise_control [] = {
        PATTERN_LABELS_GG + 23, PATTERN_LABELS_GG + 24, PATTERN_LABELS_GG + 25, PATTERN_LABELS_GG + 26,
        PATTERN_LABELS_GG + 27, PATTERN_LABELS_GG + 28
    };
    SMS_loadTileMapArea (14, 16, noise_control, 6, 1);

    /* Stereo Control */
    const uint16_t stereo [] = { PATTERN_LABELS_GG + 6, PATTERN_LABELS_GG + 7 };
    SMS_loadTileMapArea (22,  5, stereo, 1, 2);
    SMS_loadTileMapArea (22,  8, stereo, 1, 2);
    SMS_loadTileMapArea (22, 11, stereo, 1, 2);
    SMS_loadTileMapArea (22, 14, stereo, 1, 2);

    /* Buttons */
    const uint16_t tone_1_button [] = { PATTERN_LABELS_GG + 17, PATTERN_LABELS_GG + 18, PATTERN_LABELS_GG + 19, PATTERN_LABELS_GG + 20 };
    const uint16_t tone_2_button [] = { PATTERN_LABELS_GG + 17, PATTERN_LABELS_GG + 18, PATTERN_LABELS_GG + 19, PATTERN_LABELS_GG + 21 };
    const uint16_t tone_3_button [] = { PATTERN_LABELS_GG + 17, PATTERN_LABELS_GG + 18, PATTERN_LABELS_GG + 19, PATTERN_LABELS_GG + 22 };
    const uint16_t noise_button []  = { PATTERN_LABELS_GG + 31, PATTERN_LABELS_GG + 32, PATTERN_LABELS_GG + 33, PATTERN_LABELS_GG + 34 };
    SMS_loadTileMapArea (22,  7, tone_1_button, 4, 1);
    SMS_loadTileMapArea (22, 10, tone_2_button, 4, 1);
    SMS_loadTileMapArea (22, 13, tone_3_button, 4, 1);
    SMS_loadTileMapArea (22, 16, noise_button,  4, 1);

#endif
}


/*
 * Draw an LED indicator.
 */
void draw_led (uint8_t x, uint8_t y, bool value)
{
    if (value)
    {
        SMS_setTileatXY (x,     y, PATTERN_LED + 2);
        SMS_setTileatXY (x + 1, y, PATTERN_LED + 3);
    }
    else
    {
        SMS_setTileatXY (x,     y, PATTERN_LED + 0);
        SMS_setTileatXY (x + 1, y, PATTERN_LED + 1);
    }
}


/*
 * Fill the name table with tile-zero.
 */
void draw_reset (uint8_t from, uint8_t to)
{
    uint16_t blank_line [32] = { 0 };

    for (uint8_t row = from; row < to; row++)
    {
        SMS_loadTileMapArea (0, row, blank_line, 32, 1);
    }
}


/*
 * Draw the title text, "SN76489 TestRom"
 */
void draw_title (void)
{
#ifdef TARGET_SMS
    uint16_t title [22] = { PATTERN_TITLE +  0, PATTERN_TITLE +  1, PATTERN_TITLE +  2, PATTERN_TITLE +  3,
                            PATTERN_TITLE +  4, PATTERN_TITLE +  5, PATTERN_TITLE +  6, PATTERN_TITLE +  7,
                            PATTERN_TITLE +  8, PATTERN_TITLE +  9, PATTERN_TITLE + 10, PATTERN_TITLE + 11,
                            PATTERN_TITLE + 12, PATTERN_TITLE + 13, PATTERN_TITLE + 14, PATTERN_TITLE + 15,
                            PATTERN_TITLE + 16, PATTERN_TITLE + 17, PATTERN_TITLE + 18, PATTERN_TITLE + 19,
                            PATTERN_TITLE + 20, PATTERN_TITLE + 21 };
    SMS_loadTileMapArea (11, 0, title, 11, 2);
#elif defined (TARGET_GG)
    uint16_t title [24] = { PATTERN_TITLE_GG +  0, PATTERN_TITLE_GG +  1, PATTERN_TITLE_GG +  2, PATTERN_TITLE_GG +  3,
                            PATTERN_TITLE_GG +  4, PATTERN_TITLE_GG +  5, PATTERN_TITLE_GG +  6, PATTERN_TITLE_GG +  7,
                            PATTERN_TITLE_GG +  8, PATTERN_TITLE_GG +  9, PATTERN_TITLE_GG + 10, PATTERN_TITLE_GG + 11,
                            PATTERN_TITLE_GG + 12, PATTERN_TITLE_GG + 13, PATTERN_TITLE_GG + 14, PATTERN_TITLE_GG + 15,
                            PATTERN_TITLE_GG + 16, PATTERN_TITLE_GG + 17, PATTERN_TITLE_GG + 18, PATTERN_TITLE_GG + 19,
                            PATTERN_TITLE_GG + 20, PATTERN_TITLE_GG + 21, PATTERN_TITLE_GG + 22, PATTERN_TITLE_GG + 23 };
    SMS_loadTileMapArea (10, 3, title, 12, 2);
#endif
}


/*
 * Draw a two digit value indicator.
 */
void draw_value (uint8_t x, uint8_t y, uint8_t value)
{
    uint8_t digit_1 = value / 10;
    uint8_t digit_2 = value % 10;

    /* Don't show leading zeros */
    if (digit_1 == 0)
    {
        digit_1 = 10;
    }

    /* First digit */
    SMS_setTileatXY (x, y,     PATTERN_DIGITS + digit_1);
    SMS_setTileatXY (x, y + 1, PATTERN_DIGITS + digit_1 + 11);

    /* Second digit */
    SMS_setTileatXY (x + 1, y,     PATTERN_DIGITS + digit_2);
    SMS_setTileatXY (x + 1, y + 1, PATTERN_DIGITS + digit_2 + 11);

}


/*
 * Draw a four digit value indicator.
 */
void draw_value_wide (uint8_t x, uint8_t y, uint16_t value)
{
    uint8_t digit_1 = value / 1000;
    uint8_t digit_2 = (value % 1000) / 100;
    uint8_t digit_3 = (value % 100) / 10;
    uint8_t digit_4 = value % 10;

    /* Don't show leading zeros */
    if (digit_1 == 0)
    {
        digit_1 = 10;

        if (digit_2 == 0)
        {
            digit_2 = 10;

            if (digit_3 == 0)
            {
                digit_3 = 10;
            }
        }
    }

    /* First digit */
    SMS_setTileatXY (x, y,     PATTERN_DIGITS + digit_1);
    SMS_setTileatXY (x, y + 1, PATTERN_DIGITS + digit_1 + 11);

    /* Second digit */
    SMS_setTileatXY (x + 1, y,     PATTERN_DIGITS + digit_2);
    SMS_setTileatXY (x + 1, y + 1, PATTERN_DIGITS + digit_2 + 11);

    /* Third digit */
    SMS_setTileatXY (x + 2, y,     PATTERN_DIGITS + digit_3);
    SMS_setTileatXY (x + 2, y + 1, PATTERN_DIGITS + digit_3 + 11);

    /* Fourth digit */
    SMS_setTileatXY (x + 3, y,     PATTERN_DIGITS + digit_4);
    SMS_setTileatXY (x + 3, y + 1, PATTERN_DIGITS + digit_4 + 11);

}


/*
 * Draw a dimmed-out two digit value indicator.
 */
void draw_value_hidden (uint8_t x, uint8_t y)
{
    /* First digit */
    SMS_setTileatXY (x, y,     PATTERN_DIGITS + 10);
    SMS_setTileatXY (x, y + 1, PATTERN_DIGITS + 21);

    /* Second digit */
    SMS_setTileatXY (x + 1, y,     PATTERN_DIGITS + 10);
    SMS_setTileatXY (x + 1, y + 1, PATTERN_DIGITS + 21);
}
