#!/bin/bash

for fname in ./src/*.c; do
    echo "building $fname"
    #arm-none-eabi-gcc -Os -c "$fname" -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -o ./out/$(basename "$fname" .c).o
    arm-none-eabi-gcc -Os -c "$fname" -mcpu=cortex-m0 -mthumb -o ./out/$(basename "$fname" .c).o
done
echo "Linking"
arm-none-eabi-gcc -nostdlib -T "simple.ld" -Xlinker -print-memory-usage -o "PowerbankRegulator.axf" ./out/*.o
#arm-none-eabi-gcc -nostdlib -Xlinker -Map="PowerbankRegulator.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -T "simple.ld" -o "PowerbankRegulator.axf" ./out/*.o
echo "Generating binary file"
arm-none-eabi-objcopy -v -O binary "PowerbankRegulator.axf" "PowerbankRegulator.bin" 
if [[ $# -gt 0 && $1 == "-p" ]] 
then
    echo "sending bin file to device"
    lpc21isp -bin PowerbankRegulator.bin /dev/ttyUSB0 115200 12000
fi

