/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

#include <stdint.h>
__sfr __at 0x40 sn76489_port;


/*
 * Write to a register on the SN76489.
 */
static void register_write (uint8_t addr, uint16_t value)
{
    /* TODO */
}


/*
 * Write the attenuation for tone channel 0.
 */
void register_write_ch0_volume (uint16_t value)
{
    /* TODO */
}


/*
 * Write the frequency for tone channel 0.
 */
void register_write_ch0_frequency (uint16_t value)
{
    /* TODO */
}


/*
 * Write the attenuation for tone channel 1.
 */
void register_write_ch1_volume (uint16_t value)
{
    /* TODO */
}


/*
 * Write the frequency for tone channel 1.
 */
void register_write_ch1_frequency (uint16_t value)
{
    /* TODO */
}


/*
 * Write the attenuation for tone channel 2.
 */
void register_write_ch2_volume (uint16_t value)
{
    /* TODO */
}


/*
 * Write the frequency for tone channel 2.
 */
void register_write_ch2_frequency (uint16_t value)
{
    /* TODO */
}


/*
 * Write the attenuation for the noise channel.
 */
void register_write_noise_volume (uint16_t value)
{
    /* TODO */
}


/*
 * Write the noise control register.
 */
void register_write_noise_control (uint16_t value)
{
    /* TODO */
}
