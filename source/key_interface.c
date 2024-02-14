/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

#include <stdbool.h>
#include <stdint.h>

#include "register.h"
#include "key_interface.h"

extern uint16_t *element_values_ptr;

/*
 * Unlike the YM2413, the SN76489 does not have a 'key' register to turn channels
 * on and off. Instead, the attenuation is set to the maximum value, 0x0f, to stop
 * the note sounding.
 *
 * To allow the use of momentary buttons and a keyboard interface, the configured
 * attenuation is stored here, rather than written directly to the register on the
 * chip. The value is only written to the register during key-down events.
 */

channel_state_t channel_state [4] = {
    { .volume = 0x0f, .mode = MODE_DEFAULT, .key_on = false },
    { .volume = 0x0f, .mode = MODE_DEFAULT, .key_on = false },
    { .volume = 0x0f, .mode = MODE_DEFAULT, .key_on = false },
    { .volume = 0x0f, .mode = MODE_DEFAULT, .key_on = false },
};


/*
 * Update tone channel 0 volume to simulate a key register.
 */
static void key_set_ch0_key (bool value)
{
    register_write_ch0_volume (value ? channel_state [0].volume : 0x0f);
}


/*
 * Update tone channel 0 volume.
 */
void key_set_ch0_volume (uint16_t value)
{
    channel_state [0].volume = value & 0x0f;
}


/*
 * Set tone channel 0 to keyboard-mode.
 */
void key_set_ch0_mode_keyboard (uint16_t value)
{
    if (value)
    {
        channel_state [0].mode = MODE_KEYBOARD;
        key_set_ch0_key (false);
    }
    else
    {
        channel_state [0].mode = MODE_DEFAULT;
        key_set_ch0_key (false);
    }
}


/*
 * Set tone channel 0 to constant-mode.
 */
void key_set_ch0_mode_constant (uint16_t value)
{
    if (value)
    {
        channel_state [0].mode = MODE_CONSTANT;
        key_set_ch0_key (true);
    }
    else
    {
        channel_state [0].mode = MODE_DEFAULT;
        key_set_ch0_key (false);
    }
}


/*
 * Update tone channel 0 frequency.
 */
void key_set_ch0_frequency (uint16_t value)
{
    register_write_ch0_frequency (value);
}


/*
 * Set tone channel 0 momentary button.
 */
void key_set_ch0_button (uint16_t value)
{
    if (channel_state [0].mode != MODE_CONSTANT)
    {
        key_set_ch0_key (value);
    }
}


/*
 * Update tone channel 1 volume to simulate a key register.
 */
void key_set_ch1_key (uint16_t value)
{
    register_write_ch1_volume (value ? channel_state [1].volume : 0x0f);
}


/*
 * Update tone channel 1 volume.
 */
void key_set_ch1_volume (uint16_t value)
{
    channel_state [1].volume = value & 0x0f;
}


/*
 * Set tone channel 1 to keyboard-mode.
 */
void key_set_ch1_mode_keyboard (uint16_t value)
{
    if (value)
    {
        channel_state [1].mode = MODE_KEYBOARD;
        key_set_ch1_key (false);
    }
    else
    {
        channel_state [1].mode = MODE_DEFAULT;
        key_set_ch1_key (false);
    }
}


/*
 * Set tone channel 1 to constant-mode.
 */
void key_set_ch1_mode_constant (uint16_t value)
{
    if (value)
    {
        channel_state [1].mode = MODE_CONSTANT;
        key_set_ch1_key (true);
    }
    else
    {
        channel_state [1].mode = MODE_DEFAULT;
        key_set_ch1_key (false);
    }
}


/*
 * Update tone channel 1 frequency.
 */
void key_set_ch1_frequency (uint16_t value)
{
    register_write_ch1_frequency (value);
}


/*
 * Set tone channel 1 momentary button.
 */
void key_set_ch1_button (uint16_t value)
{
    if (channel_state [1].mode != MODE_CONSTANT)
    {
        key_set_ch1_key (value);
    }
}


/*
 * Update tone channel 2 key.
 */
void key_set_ch2_key (uint16_t value)
{
    register_write_ch2_volume (value ? channel_state [2].volume : 0x0f);
}


/*
 * Update tone channel 2 volume.
 */
void key_set_ch2_volume (uint16_t value)
{
    channel_state [2].volume = value & 0x0f;
}


/*
 * Set tone channel 2 to keyboard-mode.
 */
void key_set_ch2_mode_keyboard (uint16_t value)
{
    if (value)
    {
        channel_state [2].mode = MODE_KEYBOARD;
        key_set_ch2_key (false);
    }
    else
    {
        channel_state [2].mode = MODE_DEFAULT;
        key_set_ch2_key (false);
    }
}


/*
 * Set tone channel 2 to constant-mode.
 */
void key_set_ch2_mode_constant (uint16_t value)
{
    if (value)
    {
        channel_state [2].mode = MODE_CONSTANT;
        key_set_ch2_key (true);
    }
    else
    {
        channel_state [2].mode = MODE_DEFAULT;
        key_set_ch2_key (false);
    }
}


/*
 * Update tone channel 2 frequency.
 */
void key_set_ch2_frequency (uint16_t value)
{
    register_write_ch2_frequency (value);
}


/*
 * Set tone channel 2 momentary button.
 */
void key_set_ch2_button (uint16_t value)
{
    if (channel_state [2].mode != MODE_CONSTANT)
    {
        key_set_ch2_key (value);
    }
}


/*
 * Update noise channel key.
 */
void key_set_noise_key (uint16_t value)
{
    register_write_noise_volume (value ? channel_state [3].volume : 0x0f);
}


/*
 * Update noise channel volume.
 */
void key_set_noise_volume (uint16_t value)
{
    channel_state [3].volume = value & 0x0f;
}


/*
 * Set noise channel to keyboard-mode.
 */
void key_set_noise_mode_keyboard (uint16_t value)
{
    if (value)
    {
        channel_state [3].mode = MODE_KEYBOARD;
        key_set_noise_key (false);
    }
    else
    {
        channel_state [3].mode = MODE_DEFAULT;
        key_set_noise_key (false);
    }
}


/*
 * Set noise channel to constant-mode.
 */
void key_set_noise_mode_constant (uint16_t value)
{
    if (value)
    {
        channel_state [3].mode = MODE_CONSTANT;
        key_set_noise_key (true);
    }
    else
    {
        channel_state [3].mode = MODE_DEFAULT;
        key_set_noise_key (false);
    }
}


/*
 * Set noise channel momentary button.
 */
void key_set_noise_button (uint16_t value)
{
    if (channel_state [3].mode != MODE_CONSTANT)
    {
        key_set_noise_key (value);
    }
}


/*
 * Update noise control register.
 */
void key_set_noise_control (uint16_t value)
{
    register_write_noise_control (value);
}
