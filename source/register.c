/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

#include <stdint.h>
__sfr __at 0x06 gg_stereo_port;
__sfr __at 0x40 sn76489_port;

#ifdef TARGET_GG
static uint8_t gg_stereo_reg;
#endif


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


#ifdef TARGET_GG
/*
 * Write the Game Gear stereo register bit for channel-0 right.
 */
void register_write_ch0_stereo_right (uint16_t value)
{
    gg_stereo_reg &= 0xfe;
    gg_stereo_reg |= value & 0x01;
    gg_stereo_port = gg_stereo_reg;
}


/*
 * Write the Game Gear stereo register bit for channel-1 right.
 */
void register_write_ch1_stereo_right (uint16_t value)
{
    gg_stereo_reg &= 0xfd;
    gg_stereo_reg |= (value << 1) & 0x02;
    gg_stereo_port = gg_stereo_reg;
}


/*
 * Write the Game Gear stereo register bit for channel-2 right.
 */
void register_write_ch2_stereo_right (uint16_t value)
{
    gg_stereo_reg &= 0xfb;
    gg_stereo_reg |= (value << 2) & 0x04;
    gg_stereo_port = gg_stereo_reg;
}


/*
 * Write the Game Gear stereo register bit for noise-right.
 */
void register_write_noise_stereo_right (uint16_t value)
{
    gg_stereo_reg &= 0xf7;
    gg_stereo_reg |= (value << 3) & 0x08;
    gg_stereo_port = gg_stereo_reg;
}


/*
 * Write the Game Gear stereo register bit for channel-0 left.
 */
void register_write_ch0_stereo_left (uint16_t value)
{
    gg_stereo_reg &= 0xef;
    gg_stereo_reg |= (value << 4) & 0x10;
    gg_stereo_port = gg_stereo_reg;
}


/*
 * Write the Game Gear stereo register bit for channel-1 left.
 */
void register_write_ch1_stereo_left (uint16_t value)
{
    gg_stereo_reg &= 0xdf;
    gg_stereo_reg |= (value << 5) & 0x20;
    gg_stereo_port = gg_stereo_reg;
}


/*
 * Write the Game Gear stereo register bit for channel-2 left.
 */
void register_write_ch2_stereo_left (uint16_t value)
{
    gg_stereo_reg &= 0xbf;
    gg_stereo_reg |= (value << 6) & 0x40;
    gg_stereo_port = gg_stereo_reg;
}


/*
 * Write the Game Gear stereo register bit for noise-left.
 */
void register_write_noise_stereo_left (uint16_t value)
{
    gg_stereo_reg &= 0x7f;
    gg_stereo_reg |= (value << 7) & 0x80;
    gg_stereo_port = gg_stereo_reg;
}
#endif
