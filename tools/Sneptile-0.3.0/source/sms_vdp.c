/*
 * Sneptile
 * Joppy Furr 2024
 */

#define _GNU_SOURCE
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "sneptile.h"

/* State */
static uint32_t pattern_index = 0;
static uint8_t palette [16] = { };
static uint32_t palette_size = 0;

/* Mode-4 Output Files */
static FILE *pattern_file = NULL;
static FILE *pattern_index_file = NULL;
static FILE *palette_file = NULL;


/*
 * Open the three output files.
 */
int mode4_open_files (void)
{
    char *pattern_path = "pattern.h";
    char *pattern_index_path = "pattern_index.h";
    char *palette_path = "palette.h";

    /* If the user has specified an output
     * directory, create and change into it */
    if (output_dir != NULL)
    {
        asprintf (&pattern_path, "%s/pattern.h", output_dir);
        asprintf (&pattern_index_path, "%s/pattern_index.h", output_dir);
        asprintf (&palette_path, "%s/palette.h", output_dir);
    }

    pattern_file = fopen (pattern_path, "w");
    if (pattern_file == NULL)
    {
        fprintf (stderr, "Unable to open output file pattern.h\n");
        return RC_ERROR;
    }
    fprintf (pattern_file, "static const uint32_t patterns [] = {\n");

    pattern_index_file = fopen (pattern_index_path, "w");
    if (pattern_index_file == NULL)
    {
        fprintf (stderr, "Unable to open output file pattern_index.h\n");
        return RC_ERROR;
    }

    palette_file = fopen (palette_path, "w");
    if (palette_file == NULL)
    {
        fprintf (stderr, "Unable to open output file palette.h\n");
        return RC_ERROR;
    }

    if (output_dir != NULL)
    {
        free (pattern_path);
        free (pattern_index_path);
        free (palette_path);
    }

    return RC_OK;
}


/*
 * Mark the start of a new source file.
 */
void mode4_new_input_file (const char *name)
{
    /* Mark in patterns file */
    fprintf (pattern_file, "\n    /* %s */\n", name);

    /* Generate pattern index define */
    fprintf (pattern_index_file, "#define PATTERN_");

    for (char c = *name; *name != '\0'; c = *++name)
    {
        /* Don't include the file extension */
        if (c == '.')
        {
            break;
        }
        if (!isalnum (c))
        {
            c = '_';
        }

        fprintf (pattern_index_file, "%c", toupper(c));
    }

    fprintf (pattern_index_file, " %d\n", pattern_index);
}


/*
 * Convert from 6-bit SMS colour to the equivalent 12-bit GG colour.
 */
static uint16_t mode4_sms_colour_to_gg (uint8_t sms_colour)
{
    uint16_t gg_colour = 0;

    gg_colour |=  (sms_colour      & 0x03) * 5;         /* Red */
    gg_colour |= ((sms_colour >> 2 & 0x03) * 5) << 4;   /* Green */
    gg_colour |= ((sms_colour >> 4 & 0x03) * 5) << 8;   /* Blue */

    return gg_colour;
}


/*
 * Output the palette file.
 */
static int mode4_palette_write (void)
{
    if (palette_size > 16)
    {
        fprintf (stderr, "Error: Exceeded palette limit with %d colours.\n", palette_size);
        return RC_ERROR;
    }

    /* SMS Palette */
    fprintf (palette_file, "#ifdef TARGET_SMS\n");
    fprintf (palette_file, "static const uint8_t palette [16] = { ");

    for (uint32_t i = 0; i < palette_size; i++)
    {
        fprintf (palette_file, "0x%02x%s", palette [i], ((i + 1) < palette_size) ? ", " : " };\n");
    }

    /* GG Palette */
    fprintf (palette_file, "#elif defined (TARGET_GG)\n");
    fprintf (palette_file, "static const uint16_t palette [16] = { ");

    for (uint32_t i = 0; i < palette_size; i++)
    {
        fprintf (palette_file, "0x%04x%s", mode4_sms_colour_to_gg (palette [i]), ((i + 1) < palette_size) ? ", " : " };\n");
    }

    fprintf (palette_file, "#endif\n");

    return RC_OK;
}


/*
 * Finalize and close the three output files.
 */
int mode4_close_files (void)
{
    int rc;

    /* First, write the completed palette to file */
    rc = mode4_palette_write ();

    /* Pattern file */
    fprintf (pattern_file, "};\n");
    fclose (pattern_file);
    pattern_file = NULL;

    /* Pattern index file */
    fclose (pattern_index_file);
    pattern_index_file = NULL;

    /* Palette file */
    fclose (palette_file);
    palette_file = NULL;

    return rc;
}


/*
 * Add a colour to the palette.
 * Return the index of the newly added colour.
 */
uint8_t mode4_palette_add_colour (uint8_t colour)
{
    palette [palette_size] = colour;
    return palette_size++;
}


/*
 * Convert from pixel colour to palette index.
 * New colours are added to the palette as needed.
 */
static uint8_t mode4_rgb_to_index (pixel_t p)
{
    /* First, convert the pixel to a 6-bit Master System colour. */
    uint8_t colour = ((p.r & 0xc0) >> 6)
                   | ((p.g & 0xc0) >> 4)
                   | ((p.b & 0xc0) >> 2);

    /* Next, check if the colour is already in the palette */
    for (uint32_t i = 0; i < palette_size; i++)
    {
        if (palette [i] == colour)
        {
            return i;
        }
    }

    /* If not, add it */
    return mode4_palette_add_colour (colour);
}


/*
 * Process a single 8×8 tile.
 */
void mode4_process_tile (pixel_t *buffer, uint32_t stride)
{
    fprintf (pattern_file, "    ");
    for (uint32_t y = 0; y < 8; y++)
    {
        uint8_t line_data[4] = { };

        for (uint32_t x = 0; x < 8; x++)
        {
            uint8_t index = 0;
            pixel_t p = buffer [x + y * stride];

            /* If the pixel is non-transparent, calculate its colour index */
            if (p.a != 0)
            {
                index = mode4_rgb_to_index (p);
            }

            /* Convert index to bitplane representation */
            for (uint32_t i = 0; i < 4; i++)
            {
                if (index & (1 << i))
                {
                    line_data [i] |= (1 << (7 - x));
                }
            }
        }

        fprintf (pattern_file, "0x%02x%02x%02x%02x%s",
                 line_data [3], line_data [2], line_data [1], line_data [0],
                 (y < 7) ? ", " : ",\n");
    }

    pattern_index++;
}




