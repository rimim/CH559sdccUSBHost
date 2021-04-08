#!/bin/sh
PROJECT_NAME=CH559USB
XRAM_SIZE=0x0800
XRAM_LOC=0x0600
CODE_SIZE=0xEFFF
DFREQ_SYS=48000000
SDCC_COMPILER=./sdcc_linux/bin
SDCC_FLAGS="-mmcs51 --model-large --xram-size $XRAM_SIZE --xram-loc $XRAM_LOC --code-size $CODE_SIZE -I/ -DFREQ_SYS=$DFREQ_SYS"

if [ ! -f config.h ] ; then
	echo "//add your personal defines here" > config.h
fi

#uuid | awk -F "-" '{print #define BT_MAC "$5"}' | xxd -r -p >> ID.bin
uuid | awk -F "-" '{print "#define BT_MAC \"" substr($5,0,12) "\""}' > btmac.h

#uuid | awk -F "-" '{print $5}' | xxd -r -p >> ID.bin

${SDCC_COMPILER}/sdcc -c ${SDCC_FLAGS} main.c
${SDCC_COMPILER}/sdcc -c ${SDCC_FLAGS} util.c
${SDCC_COMPILER}/sdcc -c ${SDCC_FLAGS} USBHost.c
${SDCC_COMPILER}/sdcc -c ${SDCC_FLAGS} uart.c

${SDCC_COMPILER}/sdcc main.rel util.rel USBHost.rel uart.rel ${SDCC_FLAGS} -o ${PROJECT_NAME}.ihx

${SDCC_COMPILER}/packihx ${PROJECT_NAME}.ihx > ${PROJECT_NAME}.hex

objcopy -Iihex -Obinary ${PROJECT_NAME}.hex ${PROJECT_NAME}.bin
#${SDCC_COMPILER}/hex2bin -c ${PROJECT_NAME}.hex

rm -f ${PROJECT_NAME}.lk ${PROJECT_NAME}.map ${PROJECT_NAME}.mem ${PROJECT_NAME}.ihx

rm -f *.asm *.lst *.rel *.rst *.sym

#Rem This tool flashes the bin file directly to the ch559 chip, you need to install the libusb-win32 driver with the zadig( https://zadig.akeo.ie/ ) tool so the tool can access the usb device
#chflasher.exe ${PROJECT_NAME}.bin
