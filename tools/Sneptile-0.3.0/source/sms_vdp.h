/*
 * Sneptile
 * Joppy Furr 2024
 */

/* Open the three output files. */
int mode4_open_files (void);

/* Finalize and close the three output files. */
int mode4_close_files (void);

/* Add a colour to the palette. */
uint8_t mode4_palette_add_colour (uint8_t colour);

/* Mark the start of a new source file. */
void mode4_new_input_file (const char *name);

/* Process a single 8×8 tile. */
void mode4_process_tile (pixel_t *buffer, uint32_t stride);
