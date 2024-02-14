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

