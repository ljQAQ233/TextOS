#include <TextOS/TextOS.h>

VOID KernelInit (BOOT_CONFIG *BootConfig)
{
    GraphicsConfig = &BootConfig->Graphics;
}
