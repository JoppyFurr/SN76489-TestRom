/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

/* Draw a button indicator. */
void draw_button (uint8_t x, uint8_t y, bool value);

/* Draw the footer banner at the bottom of the screen. */
void draw_footer (void);

/* Initialise the keyboard display. */
void draw_keyboard (void);

/* Update the display of a key to be either active or inactive. */
void draw_keyboard_update (uint8_t key, bool active);

/* Fill the name table with tile-zero. */
void draw_reset (uint8_t from, uint8_t to);

/* Draw the control labels for melody mode. */
void draw_labels (void);

/* Draw an LED indicator. */
void draw_led (uint8_t x, uint8_t y, bool value);

/* Draw the title text, "SN76489 TestRom" */
void draw_title (void);

/* Draw a two digit value indicator. */
void draw_value (uint8_t x, uint8_t y, uint8_t value);

/* Draw a four digit value indicator. */
void draw_value_wide (uint8_t x, uint8_t y, uint16_t value);

/* Draw a dimmed-out two digit value indicator. */
void draw_value_hidden (uint8_t x, uint8_t y);
