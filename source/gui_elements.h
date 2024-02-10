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
    ELEMENT_CH0_KEYBOARD_MODE,
    ELEMENT_CH0_CONST_MODE,
    ELEMENT_CH0_FREQUENCY,
    ELEMENT_CH0_BUTTON,
    ELEMENT_CH1_VOLUME,
    ELEMENT_CH1_KEYBOARD_MODE,
    ELEMENT_CH1_CONST_MODE,
    ELEMENT_CH1_FREQUENCY,
    ELEMENT_CH1_BUTTON,
    ELEMENT_CH2_VOLUME,
    ELEMENT_CH2_KEYBOARD_MODE,
    ELEMENT_CH2_CONST_MODE,
    ELEMENT_CH2_FREQUENCY,
    ELEMENT_CH2_BUTTON,
    ELEMENT_NOISE_VOLUME,
    ELEMENT_NOISE_KEYBOARD_MODE,
    ELEMENT_NOISE_CONST_MODE,
    ELEMENT_NOISE_CONTROL,
    ELEMENT_NOISE_BUTTON,
    ELEMENT_KEYBOARD,
    ELEMENT_COUNT
} element_id_t;


typedef struct gui_element_s {

    element_type_t type;
    uint16_t max;

    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;

    element_id_t up;
    element_id_t down;
    element_id_t left;
    element_id_t right;

    void (*callback) (uint16_t value);

} gui_element_t;


static const gui_element_t psg_gui [ELEMENT_COUNT] = {
    [ELEMENT_CH0_VOLUME] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 5, .y = 3, .width = 2, .height = 3,
        .callback = register_write_ch0_volume,
        .up   = ELEMENT_CH0_VOLUME,             .down  = ELEMENT_CH1_VOLUME,
        .left = ELEMENT_CH0_VOLUME,             .right = ELEMENT_CH0_KEYBOARD_MODE
    },
    [ELEMENT_CH0_KEYBOARD_MODE] = {
        .type = TYPE_LED, .max = 1,
        .x = 9, .y = 4, .width = 2, .height = 2,
        .up   = ELEMENT_CH0_KEYBOARD_MODE,      .down  = ELEMENT_CH1_KEYBOARD_MODE,
        .left = ELEMENT_CH0_VOLUME,             .right = ELEMENT_CH0_CONST_MODE
    },
    [ELEMENT_CH0_CONST_MODE] = {
        .type = TYPE_LED, .max = 1,
        .x = 13, .y = 4, .width = 2, .height = 2,
        .up   = ELEMENT_CH0_CONST_MODE,         .down  = ELEMENT_CH1_CONST_MODE,
        .left = ELEMENT_CH0_KEYBOARD_MODE,      .right = ELEMENT_CH0_FREQUENCY
    },
    [ELEMENT_CH0_FREQUENCY] = {
        .type = TYPE_VALUE_WIDE, .max = 1023,
        .x = 17, .y = 3, .width = 4, .height = 3,
        .callback = register_write_ch0_frequency,
        .up   = ELEMENT_CH0_FREQUENCY,          .down  = ELEMENT_CH1_FREQUENCY,
        .left = ELEMENT_CH0_CONST_MODE,         .right = ELEMENT_CH0_BUTTON
    },
    [ELEMENT_CH0_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1,
        .x = 23, .y = 3, .width = 6, .height = 2,
        .up   = ELEMENT_CH0_BUTTON,             .down  = ELEMENT_CH1_BUTTON,
        .left = ELEMENT_CH0_FREQUENCY,          .right = ELEMENT_CH0_BUTTON
    },
    [ELEMENT_CH1_VOLUME] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 5, .y = 6, .width = 2, .height = 3,
        .callback = register_write_ch1_volume,
        .up   = ELEMENT_CH0_VOLUME,             .down  = ELEMENT_CH2_VOLUME,
        .left = ELEMENT_CH1_VOLUME,             .right = ELEMENT_CH1_KEYBOARD_MODE
    },
    [ELEMENT_CH1_KEYBOARD_MODE] = {
        .type = TYPE_LED, .max = 1,
        .x = 9, .y = 7, .width = 2, .height = 2,
        .up   = ELEMENT_CH0_KEYBOARD_MODE,      .down  = ELEMENT_CH2_KEYBOARD_MODE,
        .left = ELEMENT_CH1_VOLUME,             .right = ELEMENT_CH1_CONST_MODE
    },
    [ELEMENT_CH1_CONST_MODE] = {
        .type = TYPE_LED, .max = 1,
        .x = 13, .y = 7, .width = 2, .height = 2,
        .up   = ELEMENT_CH0_CONST_MODE,         .down  = ELEMENT_CH2_CONST_MODE,
        .left = ELEMENT_CH1_KEYBOARD_MODE,      .right = ELEMENT_CH1_FREQUENCY
    },
    [ELEMENT_CH1_FREQUENCY] = {
        .type = TYPE_VALUE_WIDE, .max = 1023,
        .x = 17, .y = 6, .width = 4, .height = 3,
        .callback = register_write_ch1_frequency,
        .up   = ELEMENT_CH0_FREQUENCY,          .down  = ELEMENT_CH2_FREQUENCY,
        .left = ELEMENT_CH1_CONST_MODE,         .right = ELEMENT_CH1_BUTTON
    },
    [ELEMENT_CH1_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1,
        .x = 23, .y = 6, .width = 6, .height = 2,
        .up   = ELEMENT_CH0_BUTTON,             .down  = ELEMENT_CH2_BUTTON,
        .left = ELEMENT_CH1_FREQUENCY,          .right = ELEMENT_CH1_BUTTON
    },
    [ELEMENT_CH2_VOLUME] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 5, .y = 9, .width = 2, .height = 3,
        .callback = register_write_ch2_volume,
        .up   = ELEMENT_CH1_VOLUME,             .down  = ELEMENT_NOISE_VOLUME,
        .left = ELEMENT_CH2_VOLUME,             .right = ELEMENT_CH2_KEYBOARD_MODE
    },
    [ELEMENT_CH2_KEYBOARD_MODE] = {
        .type = TYPE_LED, .max = 1,
        .x = 9, .y = 10, .width = 2, .height = 2,
        .up   = ELEMENT_CH1_KEYBOARD_MODE,      .down  = ELEMENT_NOISE_KEYBOARD_MODE,
        .left = ELEMENT_CH2_VOLUME,             .right = ELEMENT_CH2_CONST_MODE
    },
    [ELEMENT_CH2_CONST_MODE] = {
        .type = TYPE_LED, .max = 1,
        .x = 13, .y = 10, .width = 2, .height = 2,
        .up   = ELEMENT_CH1_CONST_MODE,         .down  = ELEMENT_NOISE_CONST_MODE,
        .left = ELEMENT_CH2_KEYBOARD_MODE,      .right = ELEMENT_CH2_FREQUENCY
    },
    [ELEMENT_CH2_FREQUENCY] = {
        .type = TYPE_VALUE_WIDE, .max = 1023,
        .x = 17, .y = 9, .width = 4, .height = 3,
        .callback = register_write_ch2_frequency,
        .up   = ELEMENT_CH1_FREQUENCY,          .down  = ELEMENT_NOISE_CONTROL,
        .left = ELEMENT_CH2_CONST_MODE,         .right = ELEMENT_CH2_BUTTON
    },
    [ELEMENT_CH2_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1,
        .x = 23, .y = 9, .width = 6, .height = 2,
        .up   = ELEMENT_CH1_BUTTON,             .down  = ELEMENT_NOISE_BUTTON,
        .left = ELEMENT_CH2_FREQUENCY,          .right = ELEMENT_CH2_BUTTON
    },
    [ELEMENT_NOISE_VOLUME] = {
        .type = TYPE_VALUE, .max = 15,
        .x = 5, .y = 12, .width = 2, .height = 3,
        .callback = register_write_noise_volume,
        .up   = ELEMENT_CH2_VOLUME,             .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_NOISE_VOLUME,           .right = ELEMENT_NOISE_KEYBOARD_MODE
    },
    [ELEMENT_NOISE_KEYBOARD_MODE] = {
        .type = TYPE_LED, .max = 1,
        .x = 9, .y = 13, .width = 2, .height = 2,
        .up   = ELEMENT_CH2_KEYBOARD_MODE,      .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_NOISE_VOLUME,           .right = ELEMENT_NOISE_CONST_MODE
    },
    [ELEMENT_NOISE_CONST_MODE] = {
        .type = TYPE_LED, .max = 1,
        .x = 13, .y = 13, .width = 2, .height = 2,
        .callback = register_write_noise_control,
        .up   = ELEMENT_CH2_CONST_MODE,         .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_NOISE_KEYBOARD_MODE,    .right = ELEMENT_NOISE_CONTROL
    },
    [ELEMENT_NOISE_CONTROL] = {
        .type = TYPE_VALUE, .max = 7,
        .x = 18, .y = 12, .width = 2, .height = 3,
        .callback = register_write_noise_control,
        .up   = ELEMENT_CH2_FREQUENCY,          .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_NOISE_CONST_MODE,       .right = ELEMENT_NOISE_BUTTON
    },
    [ELEMENT_NOISE_BUTTON] = {
        .type = TYPE_BUTTON, .max = 1,
        .x = 23, .y = 12, .width = 6, .height = 2,
        .up   = ELEMENT_CH2_BUTTON,             .down  = ELEMENT_KEYBOARD,
        .left = ELEMENT_NOISE_CONTROL,          .right = ELEMENT_NOISE_BUTTON
    },
    [ELEMENT_KEYBOARD] = {
        .type = TYPE_KEYBOARD, .height = 0,
        .down = ELEMENT_KEYBOARD,
        .left = ELEMENT_KEYBOARD,               .right = ELEMENT_KEYBOARD
    }
};
