#!/bin/sh
echo ""
echo "SN76489 Test ROM Build Script"
echo "-------------------------------"

# Exit on first error.
set -e

sdcc="${HOME}/Code/sdcc-4.3.0/bin/sdcc"
devkitSMS="${HOME}/Code/devkitSMS"
SMSlib="${devkitSMS}/SMSlib"
SGlib="${devkitSMS}/SGlib"
ihx2sms="${devkitSMS}/ihx2sms/Linux/ihx2sms"
sneptile="./tools/Sneptile-0.3.0/Sneptile"

build_sneptile ()
{
    # Early return if we've already got an up-to-date build
    if [ -e $sneptile \
         -a "./tools/Sneptile-0.3.0/source/main.c" -ot $sneptile \
         -a "./tools/Sneptile-0.3.0/source/sms_vdp.c" -ot $sneptile \
         -a "./tools/Sneptile-0.3.0/source/tms9928a.c" -ot $sneptile ]
    then
        return
    fi

    echo "Building Sneptile..."
    (
        cd "tools/Sneptile-0.3.0"
        ./build.sh
    )
}


# Parameter {1} - 'PAL' or 'NTSC'
build_sn76489_test_rom_sms ()
{
    echo "Building SN76489 Test ROM for SMS (${1})..."
    rm -rf build tile_data

    echo "  Generating tile data..."
    mkdir -p tile_data
    (
        # Index 0 is used for transparency, use dark grey, our background colour.
        # Index 1, 2, and 3, are used for the cursor colour-cycle.
        # Index 4 is used for the selected key colour.
        $sneptile --output tile_data --palette 0x15 0x01 0x02 0x03 0x33 \
            tiles/empty.png \
            tiles/button.png \
            tiles/cursor.png \
            tiles/digits.png \
            tiles/footer.png \
            tiles/keys_outline.png \
            tiles/keys_active.png \
            tiles/keys_inactive.png \
            tiles/labels.png \
            tiles/led.png \
            tiles/title.png
    )

    mkdir -p build
    echo "  Compiling..."
    for file in cursor draw register key_interface main
    do
        echo "   -> ${file}.c"
        ${sdcc} -c -mz80 -DTARGET_SMS -DTARGET_${1} --peep-file ${devkitSMS}/SMSlib/src/peep-rules.txt -I ${SMSlib}/src \
            -o "build/${file}.rel" "source/${file}.c" || exit 1
    done

    echo ""
    echo "  Linking..."
    ${sdcc} -o build/SN76489_TestRom_${1}.ihx -mz80 --no-std-crt0 --data-loc 0xC000 ${devkitSMS}/crt0/crt0_sms.rel build/*.rel ${SMSlib}/SMSlib.lib || exit 1

    echo ""
    echo "  Generating ROM..."
    ${ihx2sms} build/SN76489_TestRom_${1}.ihx SN76489_TestRom_${1}.sms || exit 1

    echo ""
    echo "  Done"
}


build_sn76489_test_rom_gg ()
{
    echo "Building SN76489 Test ROM for GG..."
    rm -rf build tile_data

    echo "  Generating tile data..."
    mkdir -p tile_data
    (
        # Index 0 is used for transparency, use dark grey, our background colour.
        # Index 1, 2, and 3, are used for the cursor colour-cycle.
        # Index 4 is used for the selected key colour.
        $sneptile --output tile_data --palette 0x15 0x01 0x02 0x03 0x33 \
            tiles/empty.png \
            tiles/button.png \
            tiles/cursor.png \
            tiles/digits.png \
            tiles/footer_gg.png \
            tiles/keys_outline.png \
            tiles/keys_active.png \
            tiles/keys_inactive.png \
            tiles/labels_gg.png \
            tiles/led.png \
            tiles/title_gg.png
    )

    mkdir -p build
    echo "  Compiling..."
    for file in cursor draw register key_interface main
    do
        echo "   -> ${file}.c"
        ${sdcc} -c -mz80 -DTARGET_GG -DTARGET_NTSC --peep-file ${devkitSMS}/SMSlib/src/peep-rules.txt -I ${SMSlib}/src \
            -o "build/${file}.rel" "source/${file}.c" || exit 1
    done

    echo ""
    echo "  Linking..."
    ${sdcc} -o build/SN76489_TestRom.ihx -mz80 --no-std-crt0 --data-loc 0xC000 ${devkitSMS}/crt0/crt0_sms.rel build/*.rel ${SMSlib}/SMSlib_GG.lib || exit 1

    echo ""
    echo "  Generating ROM..."
    ${ihx2sms} build/SN76489_TestRom.ihx SN76489_TestRom.gg || exit 1

    echo ""
    echo "  Done"
}


# Parameter {1} - 'PAL' or 'NTSC'
build_sn76489_test_rom_sg ()
{
    echo "Building SN76489 Test ROM for SG-1000 (${1})..."
    rm -rf build tile_data

    echo "  Generating tile data..."
    mkdir -p tile_data
    (
        # Note, tiles are organized so that similar-coloured files are
        # together, as they can share a mode-0 colour-table entry.
        $sneptile --mode-0 --output tile_data \
            tiles/empty_tms.png \
            tiles/keys_outline_tms.png \
            tiles/title_tms.png \
            tiles/button_tms.png \
            tiles/digits_tms.png \
            tiles/led_tms.png \
            tiles/labels_tms.png \
            tiles/cursor_tms.png \
            tiles/keys_active_tms.png \
            tiles/keys_inactive_tms.png \
            tiles/footer_tms.png
    )

    mkdir -p build
    echo "  Compiling..."
    for file in cursor draw register key_interface main
    do
        echo "   -> ${file}.c"
        ${sdcc} -c -mz80 -DTARGET_SG -DTARGET_${1} --peep-file ${devkitSMS}/SMSlib/src/peep-rules.txt -I ${SGlib}/src \
            -o "build/${file}.rel" "source/${file}.c" || exit 1
    done

    echo ""
    echo "  Linking..."
    ${sdcc} -o build/SN76489_TestRom_${1}.ihx -mz80 --no-std-crt0 --data-loc 0xC000 ${devkitSMS}/crt0/crt0_sg.rel build/*.rel ${SGlib}/SGlib.rel || exit 1

    echo ""
    echo "  Generating ROM..."
    ${ihx2sms} build/SN76489_TestRom_${1}.ihx SN76489_TestRom_${1}.sg || exit 1

    echo ""
    echo "  Done"
}

build_sneptile
build_sn76489_test_rom_sms PAL
build_sn76489_test_rom_sms NTSC
build_sn76489_test_rom_gg
build_sn76489_test_rom_sg PAL
build_sn76489_test_rom_sg NTSC
