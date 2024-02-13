/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

#include <stdbool.h>
#include <stdint.h>

#include "register.h"
#include "key_interface.h"

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
    { .volume = 0x0f, .key_on = false },
    { .volume = 0x0f, .key_on = false },
    { .volume = 0x0f, .key_on = false },
    { .volume = 0x0f, .key_on = false },
};

/*
 * Update tone channel 0 volume.
 */
void key_set_ch0_volume (uint16_t value)
{
    channel_state [0].volume = value & 0x0f;

    /* TODO: If key.. */
}


/*
 * Update tone channel 0 key.
 */
void key_set_ch0_key (uint16_t value)
{
    register_write_ch0_volume (value ? channel_state [0].volume : 0x0f);
}


/*
 * Update tone channel 1 volume.
 */
void key_set_ch1_volume (uint16_t value)
{
    channel_state [1].volume = value & 0x0f;
}


/*
 * Update tone channel 1 key.
 */
void key_set_ch1_key (uint16_t value)
{
    register_write_ch1_volume (value ? channel_state [1].volume : 0x0f);
}


/*
 * Update tone channel 2 volume.
 */
void key_set_ch2_volume (uint16_t value)
{
    channel_state [2].volume = value & 0x0f;
}


/*
 * Update tone channel 2 key.
 */
void key_set_ch2_key (uint16_t value)
{
    register_write_ch2_volume (value ? channel_state [2].volume : 0x0f);
}


/*
 * Update noise channel volume.
 */
void key_set_noise_volume (uint16_t value)
{
    channel_state [3].volume = value & 0x0f;
}


/*
 * Update noise channel key.
 */
void key_set_noise_key (uint16_t value)
{
    register_write_noise_volume (value ? channel_state [3].volume : 0x0f);
}

