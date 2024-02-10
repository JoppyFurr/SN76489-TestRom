/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

#include <stdint.h>
__sfr __at 0x40 sn76489_port;


/*
 * Write to a register on the SN76489.
 */
static void register_write (uint16_t value)
{
    sn76489_port = value;
}


/*
 * Write the frequency for tone channel 0.
 */
void register_write_ch0_frequency (uint16_t value)
{
    register_write (0x80 | 0x00 | (value & 0x0f));
    register_write ((value >> 4) & 0x3f);
}


/*
 * Write the attenuation for tone channel 0.
 */
void register_write_ch0_volume (uint16_t value)
{
    register_write (0x80 | 0x10 | (value & 0x0f));
}


/*
 * Write the frequency for tone channel 1.
 */
void register_write_ch1_frequency (uint16_t value)
{
    register_write (0x80 | 0x20 | (value & 0x0f));
    register_write ((value >> 4) & 0x3f);
}


/*
 * Write the attenuation for tone channel 1.
 */
void register_write_ch1_volume (uint16_t value)
{
    register_write (0x80 | 0x30 | (value & 0x0f));
}


/*
 * Write the frequency for tone channel 2.
 */
void register_write_ch2_frequency (uint16_t value)
{
    register_write (0x80 | 0x40 | (value & 0x0f));
    register_write ((value >> 4) & 0x3f);
}


/*
 * Write the attenuation for tone channel 2.
 */
void register_write_ch2_volume (uint16_t value)
{
    register_write (0x80 | 0x50 | (value & 0x0f));
}


/*
 * Write the noise control register.
 */
void register_write_noise_control (uint16_t value)
{
    register_write (0x80 | 0x60 | (value & 0x0f));
}


/*
 * Write the attenuation for the noise channel.
 */
void register_write_noise_volume (uint16_t value)
{
    register_write (0x80 | 0x70 | (value & 0x0f));
}
