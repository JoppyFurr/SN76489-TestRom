/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

/*
 * There are three ways to key-on a channel channels:
 *  1. Each channel in the GUI has a momentary button on the right.
 *  2. Each channel can be put into 'constant' mode, where the note continuously sounds.
 *  3. Each channel can be put into 'keyboard' mode, where the note is triggered by the
 *     on-screen piano keyboard.
 */
typedef enum channel_mode_e {
    MODE_DEFAULT,
    MODE_CONSTANT,
    MODE_KEYBOARD
} channel_mode_t;

typedef struct channel_state_s {
    uint8_t volume;
    channel_mode_t mode;
    bool key_on;
} channel_state_t;


/* Update tone channel 0 volume. */
void key_set_ch0_volume (uint16_t value);

/* Set tone channel 0 to keyboard-mode. */
void key_set_ch0_mode_keyboard (uint16_t value);

/* Set tone channel 0 to constant-mode. */
void key_set_ch0_mode_constant (uint16_t value);

/* Update tone channel 0 frequency. */
void key_set_ch0_frequency (uint16_t value);

/* Update tone channel 0 momentary button. */
void key_set_ch0_button (uint16_t value);

/* Update tone channel 1 volume. */
void key_set_ch1_volume (uint16_t value);

/* Set tone channel 1 to keyboard-mode. */
void key_set_ch1_mode_keyboard (uint16_t value);

/* Set tone channel 1 to constant-mode. */
void key_set_ch1_mode_constant (uint16_t value);

/* Update tone channel 1 frequency. */
void key_set_ch1_frequency (uint16_t value);

/* Update tone channel 1 momentary button. */
void key_set_ch1_button (uint16_t value);

/* Update tone channel 2 volume. */
void key_set_ch2_volume (uint16_t value);

/* Set tone channel 2 to keyboard-mode. */
void key_set_ch2_mode_keyboard (uint16_t value);

/* Set tone channel 2 to constant-mode. */
void key_set_ch2_mode_constant (uint16_t value);

/* Update tone channel 2 frequency. */
void key_set_ch2_frequency (uint16_t value);

/* Update tone channel 2 momentary button. */
void key_set_ch2_button (uint16_t value);

/* Update noise channel volume. */
void key_set_noise_volume (uint16_t value);

/* Set noise channel to keyboard-mode. */
void key_set_noise_mode_keyboard (uint16_t value);

/* Set noise channel to constant-mode. */
void key_set_noise_mode_constant (uint16_t value);

/* Update noise control register. */
void key_set_noise_control (uint16_t value);

/* Update noise channel momentary button. */
void key_set_noise_button (uint16_t value);
