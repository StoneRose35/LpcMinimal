PROJECT_NAME := MinimalBlink
CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy 
PROG := lpc21isp

LINKERSCRIPT := simple.ld
CFLAGS := -Os -ffunction-sections -fdata-sections -fno-builtin -mcpu=cortex-m0 -mthumb
LFLAGS := -nostdlib -Xlinker --gc-sections -Xlinker -print-memory-usage -T "$(LINKERSCRIPT)" 
PFLAGS :=  -bin $(PROJECT_NAME).bin /dev/ttyUSB0 115200 12000

OUTDIR := ./out
SRCDIR := ./src
SRCS := $(wildcard $(SRCDIR)/*.c)
OUTS := $(wildcard $(OUTDIR)/*.o)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OUTDIR)/%.o,$(SRCS))

OCFLAGS := -v -O binary "$(PROJECT_NAME).axf" "$(PROJECT_NAME).bin" 

.PHONY: clean

bin: clean link
	$(OBJCOPY) $(OCFLAGS)
	
link: $(OBJS)
	$(CC) $(LFLAGS) -Xlinker -Map="$(PROJECT_NAME).map" -o "$(PROJECT_NAME).axf" $^
	
$(OUTDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	rm -rf ./out/*.o
	rm -f $(PROJECT_NAME).*

program: bin
	$(PROG) $(PFLAGS)	
	
	
	