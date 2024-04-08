
# SN76489 TestRom

This ROM is intended to compare the behaviour of the PSG sound chips in real Sega hardware against the behaviour of emulators.

Seeing as the SN76489 doesn't have a key-on register, the volume of each channel is held at 15 (silent) until a button/key is pressed. The 'const' mode allows the volume to be passed to the chip without a key/button being pressed. 

Both PAL and NTSC versions of the roms are generated, the only difference being the frequency values associated with the keyboard notes, as the clock frequency is slightly different. 

The output files are:
 * `SN76489-TestRom_NTSC.sms` - Master System version
 * `SN76489-TestRom_PAL.sms` - Master System version
 * `SN76489-TestRom.gg` - Game Gear version
 * `SN76489-TestRom_NTSC.sg` - SG-1000 / SC-3000 cartridge version
 * `SN76489-TestRom_PAL.sg` - SG-1000 / SC-3000 cartridge version
 * `SN76489-TestRom_NTSC.wav` - SC-3000 cassette version for BASIC IIIa or BASIC IIIb
 * `SN76489-TestRom_PAL.wav` - SC-3000 cassette version for BASIC IIIa or BASIC IIIb

To load over tape, the following steps are used on the SC-3000:

 1. `LOAD` on the SC-3000
 2. Play the .wav file
 3. `CALL &H9800` on the SC-3000

## Dependencies
 * zlib
