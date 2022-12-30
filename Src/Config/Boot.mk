ARCH           ?= X64
TOOLCHAIN      ?= GCC5

# For EDK2 build command

INCLUDE   := $(SRC_DIR)/Include
FLAGS     :=
# For EDK2 Build Toolchains
CFLAGS    := $(addprefix -I,${INCLUDE})
ifeq (${GDB_DEBUG},true)
  CFLAGS  += -D__SRC_LEVEL_DEBUG__
endif
# For Gcc

ifeq (${DEBUG},true)
  FLAGS += -DBOOT_DEBUG
 # For Source Level Debug and Others,Also a Macro in .dsc file
endif

DSC     := SigmaBootPkg/Boot.dsc
# For EDK2 build command,means what to build
_PLATFORM_NAME := $(shell bash ${BASE}/BootGetPlatformName.sh ${DSC})
# Get PlatformName
VERSION := $(_PLATFORM_NAME)-$(TARGET)-$(shell LANG= date +"%Y%m%d")
# For Project Description File (.dsc), ThisFmt : Name + Date

export VERSION DSC TOOLCHAIN FLAGS CFLAGS OUTPUT TARGET SRC_DEBUG _PLATFORM_NAME
# export Options
