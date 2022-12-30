#include <TextOS/TextOS.h>
#include <TextOS/Graphics.h>
#include <TextOS/Console.h>
#include <TextOS/Console/Print.h>

typedef struct {
  GRAPHICS_CONFIG  Graphics;
  PHYSICAL_ADDRESS Logo;
  UINT64           Version;
} BOOT_CONFIG;

// add-symbol-file ../Build/Kernel/Kernel.elf.symbol 0x34020e8 -s .rodata 0x3402587 -s .bss 0x3032200

GRAPHICS_CONFIG *GraphicsConfig = NULL;

extern VOID KernelInit(BOOT_CONFIG *BootConfig);

RT_STATUS KernelMain (BOOT_CONFIG *BootConfig)
{
    Break ();

    GraphicsConfig = (GRAPHICS_CONFIG*) &BootConfig->Graphics;
    RT_STATUS RtStatus;

    ConsoleInit ();

    return RtStatus;
}
