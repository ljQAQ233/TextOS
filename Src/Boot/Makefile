SRC_DIR   ?= ..
SHELL     := bash

OUTPUT    ?= .

include $(SRC_DIR)/Config/Boot.mk

.PHONY: BootLoader
BootLoader:
	@source ./BaseTools/BuildEnv && if ! build --help > /dev/null;then \
	rm -rf Conf/tools_def.txt \
		   Conf/target.txt \
		   Conf/build_rule.txt \
	;fi
	
	source ./BaseTools/BuildEnv && \
		build $(FLAGS) -p $(DSC) \
		-a $(ARCH) \
		-t $(TOOLCHAIN) \
		-b $(TARGET) \
		-DOUTPUT=$(BOOT_OUTPUT) \
		-DCFLAGS="$(CFLAGS)"
	$(SUDO) cp -rf "`find $(BOOT_OUTPUT) -iname $(_PLATFORM_NAME).efi | grep "$(TARGET)" | grep "$(TOOLCHAIN)" | grep "$(ARCH)" | awk 'NR==1{print $0}'`" $(BOOT_EXEC)

.PHONY:clean 
clean:
	rm -rf $(BOOT_OUTPUT)/RELEASE* \
		$(BOOT_OUTPUT)/DEBUG* \
		$(BOOT_OUTPUT)/NOOPT*
	if ! grep Src <<< $(abspath ${BOOT_OUTPUT}) 2>&1 >/dev/null;then \
		rm -rf $(BOOT_OUTPUT) \
	;fi

