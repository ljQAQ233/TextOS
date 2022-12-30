QEMU_BINARY = /prefix/qemu/bin
# Set Qemu Executable File Path

OVMF        = ./Base/OVMF_$(TARGET)_$(ARCH).fd
# the BIOS floppy for Qemu to Run

MEM = 64M

QEMU_FLAGS  += -hda $(IMG_OUTPUT) \
			   -bios $(OVMF) \
			   -net none \
			   -m $(MEM)
# Qemu Args

export QEMU_BINARY QEMU_FLAGS OVMF 
