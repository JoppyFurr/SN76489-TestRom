/*
 * SN76489 Test ROM
 * Joppy Furr 2024
 */

/* Write the frequency for tone channel 0. */
void register_write_ch0_frequency (uint16_t value);

/* Write the attenuation for tone channel 0. */
void register_write_ch0_volume (uint16_t value);

/* Write the frequency for tone channel 1. */
void register_write_ch1_frequency (uint16_t value);

/* Write the attenuation for tone channel 1. */
void register_write_ch1_volume (uint16_t value);

/* Write the frequency for tone channel 2. */
void register_write_ch2_frequency (uint16_t value);

/* Write the attenuation for tone channel 2. */
void register_write_ch2_volume (uint16_t value);

/* Write the noise control register. */
void register_write_noise_control (uint16_t value);

/* Write the attenuation for the noise channel. */
void register_write_noise_volume (uint16_t value);

#ifdef TARGET_GG
/* Write the Game Gear stereo register bit for channel-0 right. */
void register_write_ch0_stereo_right (uint16_t value);

/* Write the Game Gear stereo register bit for channel-1 right. */
void register_write_ch1_stereo_right (uint16_t value);

/* Write the Game Gear stereo register bit for channel-2 right. */
void register_write_ch2_stereo_right (uint16_t value);

/* Write the Game Gear stereo register bit for noise-right. */
void register_write_noise_stereo_right (uint16_t value);

/* Write the Game Gear stereo register bit for channel-0 left. */
void register_write_ch0_stereo_left (uint16_t value);

/* Write the Game Gear stereo register bit for channel-1 left. */
void register_write_ch1_stereo_left (uint16_t value);

/* Write the Game Gear stereo register bit for channel-2 left. */
void register_write_ch2_stereo_left (uint16_t value);

/* Write the Game Gear stereo register bit for noise-left. */
void register_write_noise_stereo_left (uint16_t value);
#endif
