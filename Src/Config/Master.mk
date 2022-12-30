SRC_DIR  := $(shell pwd)
# Set Source Root

OUTPUT   ?= ../Build
OUTPUT   := $(abspath ${OUTPUT})
# Get the RealPath

BOOT_OUTPUT     := $(OUTPUT)/Boot
# For a Macro in .dsc file I defined,the output directory
ifeq (${ARCH},X64)
  BOOT_EXEC     := $(BOOT_OUTPUT)/BootX64.efi
else
  BOOT_EXEC     := $(BOOT_OUTPUT)/BootIa32.efi
endif
# Boot Built Executable EFI File Output File on Host

KERNEL_OUTPUT := $(OUTPUT)/Kernel
# Kernel Build Output Directory
KERNEL_EXEC   := $(KERNEL_OUTPUT)/Kernel.elf
# Kernel Built Executable ELF File Output File on Host

ifeq (${ARCH},X64)
  IMG_EFI = $(IMG_MDIR)/EFI/Boot/bootX64.efi
  QEMU    = $(QEMU_BINARY)/qemu-system-x86_64
else
  IMG_EFI = $(IMG_MDIR)/EFI/Boot/bootia32.efi
  QEMU    = $(QEMU_BINARY)/qemu-system-i386
endif

SHELL  := bash
SUDO   := echo q | sudo -S
BASE   := Base

BASE   := $(abspath ${BASE})

export SRC_DIR CC NASM SHELL SUDO BASE BOOT_OUTPUT BOOT_EXEC KERNEL_OUTPUT KERNEL_EXEC IMG_EFI IMG_KERNEL
