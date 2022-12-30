#include <Uefi.h>

#include <Boot/Debug.h>

VOID DebugKernelSet (
        PHYSICAL_ADDRESS KernelEntry
        )
{
    GdbDebugInfo.KernelEntry = KernelEntry;
    GdbDebugInfo.KernelRuntime = TRUE;
}
