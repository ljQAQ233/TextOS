SRC_DIR  ?= ..
SHELL    := bash

#(udb) add-symbol-file ../Build/Kernel/Kernel.elf.symbol 0x740d0e8

OUTPUT   := $(KERNEL_OUTPUT)

export OUTPUT

include $(SRC_DIR)/Config/Kernel.mk

ifeq (${GDB_DEBUG},true)
  CFLAGS += -D__TEXTOS_DEBUG__
endif

.PHONY: Kernel
Kernel: $(KERNEL_OUTPUT) $(KERNEL_EXEC)

.PHONY:$(KERNEL_EXEC)
$(KERNEL_EXEC): $(OUTPUT)/Main.o \
				$(OUTPUT)/Debug.o \
				$(OUTPUT)/Graphics/Graphics.o \
				$(OUTPUT)/Console/Console.o \
				$(OUTPUT)/Tools/Tools.o \
				$(OUTPUT)/Sys/Sys.o \
				$(OUTPUT)/Lib/Lib.o
	$(LD) $(LDFLAGS) $^ -e KernelMain -o $@
	if [[ $(DEBUG) == "true" ]];then \
	  $(OBJCOPY) --only-keep-debug $@ $@.symbol \
	;fi
	$(OBJCOPY) -g $@ $@
	echo -e "\
define kernelSym\n\
  if GdbDebugInfo.KernelRuntime == 1\n\
    set variable \$$KernelEntry = GdbDebugInfo.KernelEntry\n\
    add-symbol-file $@.symbol \$$KernelEntry\n\
  end\n\
end\n\
define hook-stop\n\
  kernelSym\n\
end\n" > ${SRC_DIR}/Base/GdbKernelDebugEnvSet.gdb

$(OUTPUT)/Main.o: Main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT)/Init.o: Init.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: $(OUTPUT)/Graphics/Graphics.o
$(OUTPUT)/Graphics/Graphics.o:
	make -C Graphics

.PHONY: $(OUTPUT)/Console/Console.o
$(OUTPUT)/Console/Console.o:
	make -C Console

.PHONY: $(OUTPUT)/Tools/Tools.o
$(OUTPUT)/Tools/Tools.o:
	make -C Tools

.PHONY: $(OUTPUT)/Sys/Sys.o
$(OUTPUT)/Sys/Sys.o:
	make -C Sys

.PHONY: $(OUTPUT)/Lib/Lib.o
$(OUTPUT)/Lib/Lib.o:
	make -C Lib

.PHONY: $(OUTPUT)/Debug/Debug.o
$(OUTPUT)/Debug.o: Debug.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_OUTPUT):
	mkdir -p $(KERNEL_OUTPUT)

.PHONY: clean
clean:
	if [[ "$(abspath $(OUTPUT))" != "$$(pwd)" ]];then \
	  rm -rf $(OUTPUT) \
	;fi
	if [[ "$(abspath $(KERNEL_OUTPUT))" != "$$(pwd)" ]];then \
	  rm -rf $(KERNEL_OUTPUT) \
	;fi
	
	make -C Sys clean
	make -C Lib clean
	make -C Tools clean
	make -C Console clean
	make -C Graphics clean

