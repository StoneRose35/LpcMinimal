#!/bin/bash

PROJECT_NAME="MinimalBlink"

for fname in ./src/*.c; do
    echo "building $fname"
    arm-none-eabi-gcc -Os -c "$fname" -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -o ./out/$(basename "$fname" .c).o
    #arm-none-eabi-gcc -Os -c "$fname" -mcpu=cortex-m0 -mthumb -o ./out/$(basename "$fname" .c).o
done
echo "Linking"
#arm-none-eabi-gcc -nostdlib -T "simple.ld" -Xlinker -print-memory-usage -o "$PROJECT_NAME.axf" ./out/*.o
arm-none-eabi-gcc -nostdlib -Xlinker -Map="$PROJECT_NAME.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -T "simple.ld" -o "$PROJECT_NAME.axf" ./out/*.o
echo "Generating binary file"
arm-none-eabi-objcopy -v -O binary "$PROJECT_NAME.axf" "$PROJECT_NAME.bin" 
if [[ $# -gt 0 && $1 == "-p" ]] 
then
    echo "sending bin file to device"
    lpc21isp -bin $PROJECT_NAME.bin /dev/ttyUSB0 115200 12000
fi

