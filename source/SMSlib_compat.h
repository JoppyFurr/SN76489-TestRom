/*
 * Some SGlib functions can be used as drop-in replacements for SMSlib functions.
 * This file provides a mapping to help reduce ifdefs throughout shared code.
 */

#define SMS_getKeysStatus       SG_getKeysStatus
#define SMS_getKeysPressed      SG_getKeysPressed
#define SMS_getKeysReleased     SG_getKeysReleased

#define SMS_displayOn           SG_displayOn
#define SMS_waitForVBlank       SG_waitForVBlank

#define SMS_loadTileMap         SG_loadTileMap
#define SMS_setTileatXY         SG_setTileatXY
#define SMS_initSprites         SG_initSprites
#define SMS_addSprite(X, Y, P)  do { \
                                    SG_addSprite (X, Y, P, 8); /* Medium Red */ \
                                } while (0);
#define SMS_copySpritestoSAT()  do { \
                                    SG_finalizeSprites (); \
                                    SG_copySpritestoSAT (); \
                                } while (0);
