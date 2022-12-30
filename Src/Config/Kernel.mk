ARCH         ?= X64

VERSION := 1.0

CC      := gcc
LD      := ld
OBJCOPY := objcopy
NASM    := nasm

INCLUDE := $(SRC_DIR)/Include
INCLUDE += $(abspath Arch/$(ARCH))

CFLAGS   := -static
CFLAGS   += -nostdlib
CFLAGS   += -nostdinc
CFLAGS   += -fshort-wchar
CFLAGS   += -fno-builtin
CFLAGS   += -ffreestanding
CFLAGS   += -fno-stack-protector
CFLAGS   += $(addprefix -I,${INCLUDE})
CFLAGS   += -g -O0
CFLAGS   += -std=c11

LDFLAGS  := -nostdlib
LDFLAGS  += -static

export CC LD OBJCOPY NASM CFLAGS LDFLAGS
