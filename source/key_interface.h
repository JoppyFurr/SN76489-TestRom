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

/* Update tone channel 0 key. */
void key_set_ch0_key (uint16_t value);

/* Update tone channel 1 volume. */
void key_set_ch1_volume (uint16_t value);

/* Update tone channel 1 key. */
void key_set_ch1_key (uint16_t value);

/* Update tone channel 2 volume. */
void key_set_ch2_volume (uint16_t value);

/* Update tone channel 2 key. */
void key_set_ch2_key (uint16_t value);

/* Update noise channel volume. */
void key_set_noise_volume (uint16_t value);

/* Update noise channel key. */
void key_set_noise_key (uint16_t value);
