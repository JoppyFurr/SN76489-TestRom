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
        .up   = ELEMENT_CH0_VOLUME,         .down  = ELEMENT_CH0_VOLUME,
        .left = ELEMENT_CH0_VOLUME,         .right = ELEMENT_CH0_VOLUME
    },
};
