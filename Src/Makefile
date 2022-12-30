ARCH   := X64

ifneq ($(findstring debug,${MAKECMDGOALS}),)
  DEBUG   := true
else ifneq ($(findstring gdb,${MAKECMDGOALS}),)
  DEBUG   := true
else ifneq ($(findstring TestGdb,${MAKECMDGOALS}),)
  DEBUG   := true
else
  DEBUG   := false
endif

ifeq (${DEBUG},true)
 TARGET = DEBUG
else
 TARGET = RELEASE
endif

export ARCH DEBUG TARGET

include Config/Master.mk
include Config/Gdb.mk
include Config/Qemu.mk
include Config/Image.mk

ifneq ($(findstring gdb,${MAKECMDGOALS}),)
  GDB_DEBUG := true
  PIPE      := $(shell cat $(UDK_DBG_CFG) | grep -E "^[P ]ort" | awk '{print $$NF}')
#else ifneq ($(findstring VscodeGdb,${MAKECMDGOALS}),)
#  PIPE      := $(shell cat $(UDK_DBG_CFG) | grep -E "^[P ]ort" | awk '{print $$NF}')
else ifneq ($(findstring TestGdb,${MAKECMDGOALS}),)
  GDB_DEBUG := true
  PIPE      := $(shell cat $(UDK_DBG_CFG) | grep -E "^[P ]ort" | awk '{print $$NF}')
endif

export GDB_DEBUG DEBUG

.PHONY: all release debug
all: Boot Kernel Image
release: all
debug: export DEBUG := true
debug: all

ifneq ($(findstring TestGdb,${MAKECMDGOALS}),)
gdb: Test Image
  OUTPUT    := ../Build/Test
  OUTPUT    := $(abspath ${OUTPUT})
else
gdb: debug
endif

#ifneq ($(findstring TestGdb,${MAKECMDGOALS}),)
#gdb: Test Image
#else
#gdb: debug
#endif

.PHONY: Boot
Boot:
	@echo "---------------"
	@echo "Start to Compile Boot Module..."
	make -C Boot

.PHONY: Test
Test:
	@echo "---------------"
	@echo "Start to Compile Boot Module(Test)..."
	make -C Boot DSC=TestPkg/TestPkg.dsc

.PHONY: Kernel
Kernel:
	@echo "---------------"
	@echo -e "Start to Compile Kernel...\n"
	make -C Kernel

.PHONY: Image
Image:
	@echo "---------------"
	@echo -e "Making Image for Running...\n"
	
	dd if=/dev/zero of=$(IMG_OUTPUT) bs=1M count=$(IMG_SIZE)
	mkfs.fat -F 16 $(IMG_OUTPUT)
	mkdir -p $(IMG_MDIR)
	$(SUDO) mount $(IMG_OUTPUT) $(IMG_MDIR)
	
	$(SUDO) mkdir -p $(IMG_MDIR)/EFI/Boot
	$(SUDO) cp -rf $(BOOT_EXEC) $(IMG_MDIR)/EFI/Boot/$(OUT_EFI)
	$(SUDO) cp -rf $(KERNEL_EXEC) $(IMG_MDIR)
	
	$(SUDO) cp ./Res/bedrock.bmp $(IMG_MDIR)
	$(SUDO) cp ./Res/Fonts.bmp $(IMG_MDIR)
	$(SUDO) cp ./Res/ASC16 $(IMG_MDIR)
	$(SUDO) cp ./Res/Setting.ini $(IMG_MDIR)
	
	$(SUDO) umount $(IMG_MDIR)
	$(SUDO) rm -rf Image
	$(SUDO) chmod a+rw $(IMG_OUTPUT)

.PHONY: compile_commands.json
compile_commands.json:
	make -C ./Boot clean
	make -C ./Kernel clean
	bear -o ./Base/compile_commands_raw.json make -C ./Boot BOOT_FLAGS=-q
	bear -a -o ./Base/compile_commands_raw.json make -C ./Kernel
	grep -v "\-maccumulate-outgoing-args" ./Base/compile_commands_raw.json  > ./Base/compile_commands.json
	rm ./Base/compile_commands_raw.json

.PHONY: run
run:
	@if ! ls $(OUTPUT) 2>&1| grep $(notdir ${IMG_OUTPUT}) > /dev/null;then \
		echo "No that Image to Run" \
		;exit 1 \
	;fi
	$(QEMU) $(QEMU_FLAGS)

.PHONY: gdb
gdb:
	mkfifo $(PIPE).in $(PIPE).out
	$(UDK_DBG_EXEC) > $(SRC_DIR)/Base/udkdebugger.log &
	$(SHELL) $(SRC_DIR)/Base/gQemuModuleMonitor.sh > /dev/null 2>&1 &
	$(QEMU) $(subst DEBUG,NOOPT,${QEMU_FLAGS}) -serial pipe:$(PIPE) &

	echo -e "#!/usr/bin/env bash \n \
	while ! LANG= $(SUDO) netstat -tnlp | grep udk-| grep -v 20715 >/dev/null 2>&1;do \n \
	  sleep 1 \n \
	done \n" > $(SRC_DIR)/Base/GdbStarter.sh
	echo $(GDB_STARTER_CMD) >> $(SRC_DIR)/Base/GdbStarter.sh
	echo "exit 0" >> $(SRC_DIR)/Base/GdbStarter.sh

	$(SHELL) $(SRC_DIR)/Base/gGdbModuleMonitor.sh > /dev/null 2>&1 &
	$(SHELL) $(SRC_DIR)/Base/GdbStarter.sh > /dev/null 2>&1

#.PHONY: VscodeGdb
#VscodeGdb: debug
#	$(QEMU) ${QEMU_FLAGS} -s -S

.PHONY: TestGdb
TestGdb: gdb

.PHONY: diskmu
diskmu:
	@mkdir -p ${IMG_MDIR}
	@if ! $(SUDO) mount $(IMG_OUTPUT) $(IMG_MDIR) > /dev/null 2>&1;then \
		$(SUDO) umount ${IMG_MDIR} \
		;rm -rf ${IMG_MDIR} \
		;echo "Umounted the disk and removed the dir of the disk." \
	;else \
		echo "Created an dir for the disk and mounted the disk." \
	;fi

.PHONY: clean
clean:
	make -C Boot clean
	make -C Kernel clean
	rm -rf $(IMG_OUTPUT) $(OUTPUT)

