#ifndef __DEBUG_H__
#define __DEBUG_H__

typedef struct {
  PHYSICAL_ADDRESS KernelEntry;
  BOOLEAN          KernelRuntime;
} GDB_DEBUG_SUPPORT;

extern GDB_DEBUG_SUPPORT GdbDebugInfo;

VOID DebugKernelSet (
        PHYSICAL_ADDRESS KernelEntry
        );

#endif
