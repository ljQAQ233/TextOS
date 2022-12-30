CC     := gcc
NASM   := nasm
CFLAGS := -std=c11 $(ARGS) -I../Src/Include -g
NFLAGS += $(ARGS)

SHELL  := bash

DSC    := TestPkg/TestPkg.dsc
FLAGS  := 
BOOT_OUTPUT := ./Build

BOOT_OUTPUT := $(abspath ${BOOT_OUTPUT})

export DSC BOOT_OUTPUT ARCH TARGET FLAGS

.PHONY : boot
boot:
	make -C ../Src Boot

%: %.c Build
	$(CC) $(CFLAGS) -o ./Build/$@ $<

%.bin: %.asm Build
	$(NASM) $(NFLAGS) -f bin -o ./Build/$@ $<

Build:
	mkdir -p Build

.PHONY: clean
clean:
	rm -rf ./Build
