/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

typedef enum element_type_e {
    TYPE_VALUE = 0,
    TYPE_VALUE_WIDE,
    TYPE_LED,
    TYPE_BUTTON,
    TYPE_KEYBOARD
} element_type_t;

typedef enum element_id_e {
    ELEMENT_CH0_VOLUME = 0,
    ELEMENT_CH0_MODE_KEYBOARD,
    ELEMENT_CH0_MODE_CONSTANT,
    ELEMENT_CH0_FREQUENCY,
    ELEMENT_CH0_BUTTON,
    ELEMENT_CH1_VOLUME,
    ELEMENT_CH1_MODE_KEYBOARD,
    ELEMENT_CH1_MODE_CONSTANT,
    ELEMENT_CH1_FREQUENCY,
    ELEMENT_CH1_BUTTON,
    ELEMENT_CH2_VOLUME,
    ELEMENT_CH2_MODE_KEYBOARD,
    ELEMENT_CH2_MODE_CONSTANT,
    ELEMENT_CH2_FREQUENCY,
    ELEMENT_CH2_BUTTON,
    ELEMENT_NOISE_VOLUME,
    ELEMENT_NOISE_MODE_KEYBOARD,
    ELEMENT_NOISE_MODE_CONSTANT,
    ELEMENT_NOISE_CONTROL,
    ELEMENT_NOISE_BUTTON,
    ELEMENT_KEYBOARD,
    ELEMENT_COUNT
} element_id_t;

#define CHANNEL_OFFSET 5


typedef struct gui_element_s {

    element_type_t type;
    uint16_t max;

    uint8_t x;
    uint8_t y;

    uint8_t cursor_x;
    uint8_t cursor_y;
    uint8_t cursor_w;
    uint8_t cursor_h;

    element_id_t up;
    element_id_t down;
    element_id_t left;
    element_id_t right;

    void (*callback) (uint16_t value);

} gui_element_t;


static const gui_element_t psg_gui [ELEMENT_COUNT] = {
    [ELEMENT_CH0_VOLUME] = {
        .type = TYPE_VALUE, .max = 15, .x = 5, .y = 3,
        .cursor_x = 39, .cursor_y = 23, .cursor_w = 18, .cursor_h = 24,
        .callback = key_set_ch0_volume,
        .up   = ELEMENT_CH0_VOLUME,             .down  = ELEMENT_CH1_VOLUME,
        .left = ELEMENT_CH0_VOLUME,             .right = ELEMENT_CH0_MODE_KEYBOARD
    },
    [ELEMENT_CH0_MODE_KEYBOARD] = {
        .type = TYPE_LED, .max = 1, .x = 9, .y = 4,
        .cursor_x = 63, .cursor_y = 31, .cursor_w = 34, .cursor_h = 16,
        .callback = key_set_ch0_mode_keyboard,
        .up   = ELEMENT_CH0_MODE_KEYBOARD,      .down  = ELEMENT_CH1_MODE_KEYBOARD,
        .left = ELEMENT_CH0_VOLUME,             .right = ELEMENT_CH0_MODE_CONSTANT
    },
    [ELEMENT_CH0_MODE_CONSTANT] = {
        .type = TYPE_LED, .max = 1, .x = 13, .y = 4,
        .cursor_x = 101, .cursor_y = 31, .cursor_w = 22, .cursor_h = 16,
        .callback = key_set_ch0_mode_constant,
        .up   = ELEMENT_CH0_MODE_CONSTANT,      .down  = ELEMENT_CH1_MODE_CONSTANT,
        .left = ELEMENT_CH0_MODE_KEYBOARD,      .right = ELEMENT_CH0_FREQUENCY
    },
    [ELEMENT_CH0_FREQUENCY] = {
        .type = TYPE_VALUE_WIDE, .max = 1023, .x = 17, .y = 3,
        .cursor_x = 133, .cursor_y = 23, .cursor_w = 38, .cursor_h = 25,
        .callback = key_set_ch0_frequency,
        .up   = ELEMENT_CH0_FREQUENCY,          .down  = ELEMENT_CH1_FREQUENCY,
        .left = ELEMENT_CH0_MODE_CONSTANT,      .right = ELEMENT_CH0_BUTTON
    },
    [ELEMENT_CH0_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1, .x = 23, .y = 3,
        .cursor_x = 183, .cursor_y = 23, .cursor_w = 48, .cursor_h = 18,
        .callback = key_set_ch0_button,
        .up   = ELEMENT_CH0_BUTTON,             .down  = ELEMENT_CH1_BUTTON,
        .left = ELEMENT_CH0_FREQUENCY,          .right = ELEMENT_CH0_BUTTON
    },
    [ELEMENT_CH1_VOLUME] = {
        .type = TYPE_VALUE, .max = 15, .x = 5, .y = 6,
        .cursor_x = 39, .cursor_y = 47, .cursor_w = 18, .cursor_h = 24,
        .callback = key_set_ch1_volume,
        .up   = ELEMENT_CH0_VOLUME,             .down  = ELEMENT_CH2_VOLUME,
        .left = ELEMENT_CH1_VOLUME,             .right = ELEMENT_CH1_MODE_KEYBOARD
    },
    [ELEMENT_CH1_MODE_KEYBOARD] = {
        .type = TYPE_LED, .max = 1, .x = 9, .y = 7,
        .cursor_x = 63, .cursor_y = 55, .cursor_w = 34, .cursor_h = 16,
        .callback = key_set_ch1_mode_keyboard,
        .up   = ELEMENT_CH0_MODE_KEYBOARD,      .down  = ELEMENT_CH2_MODE_KEYBOARD,
        .left = ELEMENT_CH1_VOLUME,             .right = ELEMENT_CH1_MODE_CONSTANT
    },
    [ELEMENT_CH1_MODE_CONSTANT] = {
        .type = TYPE_LED, .max = 1, .x = 13, .y = 7,
        .cursor_x = 101, .cursor_y = 55, .cursor_w = 22, .cursor_h = 16,
        .callback = key_set_ch1_mode_constant,
        .up   = ELEMENT_CH0_MODE_CONSTANT,      .down  = ELEMENT_CH2_MODE_CONSTANT,
        .left = ELEMENT_CH1_MODE_KEYBOARD,      .right = ELEMENT_CH1_FREQUENCY
    },
    [ELEMENT_CH1_FREQUENCY] = {
        .type = TYPE_VALUE_WIDE, .max = 1023, .x = 17, .y = 6,
        .cursor_x = 133, .cursor_y = 47, .cursor_w = 38, .cursor_h = 25,
        .callback = key_set_ch1_frequency,
        .up   = ELEMENT_CH0_FREQUENCY,          .down  = ELEMENT_CH2_FREQUENCY,
        .left = ELEMENT_CH1_MODE_CONSTANT,      .right = ELEMENT_CH1_BUTTON
    },
    [ELEMENT_CH1_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1, .x = 23, .y = 6,
        .cursor_x = 183, .cursor_y = 47, .cursor_w = 48, .cursor_h = 18,
        .callback = key_set_ch1_button,
        .up   = ELEMENT_CH0_BUTTON,             .down  = ELEMENT_CH2_BUTTON,
        .left = ELEMENT_CH1_FREQUENCY,          .right = ELEMENT_CH1_BUTTON
    },
    [ELEMENT_CH2_VOLUME] = {
        .type = TYPE_VALUE, .max = 15, .x = 5, .y = 9,
        .cursor_x = 39, .cursor_y = 71, .cursor_w = 18, .cursor_h = 24,
        .callback = key_set_ch2_volume,
        .up   = ELEMENT_CH1_VOLUME,             .down  = ELEMENT_NOISE_VOLUME,
        .left = ELEMENT_CH2_VOLUME,             .right = ELEMENT_CH2_MODE_KEYBOARD
    },
    [ELEMENT_CH2_MODE_KEYBOARD] = {
        .type = TYPE_LED, .max = 1, .x = 9, .y = 10,
        .cursor_x = 63, .cursor_y = 79, .cursor_w = 34, .cursor_h = 16,
        .callback = key_set_ch2_mode_keyboard,
        .up   = ELEMENT_CH1_MODE_KEYBOARD,      .down  = ELEMENT_NOISE_MODE_KEYBOARD,
        .left = ELEMENT_CH2_VOLUME,             .right = ELEMENT_CH2_MODE_CONSTANT
    },
    [ELEMENT_CH2_MODE_CONSTANT] = {
        .type = TYPE_LED, .max = 1, .x = 13, .y = 10,
        .cursor_x = 101, .cursor_y = 79, .cursor_w = 22, .cursor_h = 16,
        .callback = key_set_ch2_mode_constant,
        .up   = ELEMENT_CH1_MODE_CONSTANT,      .down  = ELEMENT_NOISE_MODE_CONSTANT,
        .left = ELEMENT_CH2_MODE_KEYBOARD,      .right = ELEMENT_CH2_FREQUENCY
    },
    [ELEMENT_CH2_FREQUENCY] = {
        .type = TYPE_VALUE_WIDE, .max = 1023, .x = 17, .y = 9,
        .cursor_x = 133, .cursor_y = 71, .cursor_w = 38, .cursor_h = 25,
        .callback = key_set_ch2_frequency,
        .up   = ELEMENT_CH1_FREQUENCY,          .down  = ELEMENT_NOISE_CONTROL,
        .left = ELEMENT_CH2_MODE_CONSTANT,      .right = ELEMENT_CH2_BUTTON
    },
    [ELEMENT_CH2_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1, .x = 23, .y = 9,
        .cursor_x = 183, .cursor_y = 71, .cursor_w = 48, .cursor_h = 18,
        .callback = key_set_ch2_button,
        .up   = ELEMENT_CH1_BUTTON,             .down  = ELEMENT_NOISE_BUTTON,
        .left = ELEMENT_CH2_FREQUENCY,          .right = ELEMENT_CH2_BUTTON
    },
    [ELEMENT_NOISE_VOLUME] = {
        .type = TYPE_VALUE, .max = 15, .x = 5, .y = 12,
        .cursor_x = 39, .cursor_y = 95, .cursor_w = 18, .cursor_h = 24,
        .callback = key_set_noise_volume,
        .up   = ELEMENT_CH2_VOLUME,             .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_NOISE_VOLUME,           .right = ELEMENT_NOISE_MODE_KEYBOARD
    },
    [ELEMENT_NOISE_MODE_KEYBOARD] = {
        .type = TYPE_LED, .max = 1, .x = 9, .y = 13,
        .cursor_x = 63, .cursor_y = 103, .cursor_w = 34, .cursor_h = 16,
        .callback = key_set_noise_mode_keyboard,
        .up   = ELEMENT_CH2_MODE_KEYBOARD,      .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_NOISE_VOLUME,           .right = ELEMENT_NOISE_MODE_CONSTANT
    },
    [ELEMENT_NOISE_MODE_CONSTANT] = {
        .type = TYPE_LED, .max = 1, .x = 13, .y = 13,
        .cursor_x = 101, .cursor_y = 103, .cursor_w = 22, .cursor_h = 16,
        .callback = key_set_noise_mode_constant,
        .up   = ELEMENT_CH2_MODE_CONSTANT,      .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_NOISE_MODE_KEYBOARD,    .right = ELEMENT_NOISE_CONTROL
    },
    [ELEMENT_NOISE_CONTROL] = {
        .type = TYPE_VALUE, .max = 7, .x = 18, .y = 12,
        .cursor_x = 133, .cursor_y = 95, .cursor_w = 38, .cursor_h = 24,
        .callback = key_set_noise_control,
        .up   = ELEMENT_CH2_FREQUENCY,          .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_NOISE_MODE_CONSTANT,    .right = ELEMENT_NOISE_BUTTON
    },
    [ELEMENT_NOISE_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1, .x = 23, .y = 12,
        .cursor_x = 183, .cursor_y = 95, .cursor_w = 45, .cursor_h = 18,
        .callback = key_set_noise_button,
        .up   = ELEMENT_CH2_BUTTON,             .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_NOISE_CONTROL,          .right = ELEMENT_NOISE_BUTTON
    },
    [ELEMENT_KEYBOARD] = {
        .type = TYPE_KEYBOARD, .cursor_w = 0,
        .down = ELEMENT_KEYBOARD,
        .left = ELEMENT_KEYBOARD,               .right = ELEMENT_KEYBOARD
    }
};
