#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "Elf.h"

typedef struct {
    VOID                 *Kernel;
    UINTN                Size;
    EFI_PHYSICAL_ADDRESS Entry;
} KERNEL_CONFIG;

EFI_STATUS
KernelLoad (
  IN     EFI_HANDLE           ImageHandle,
  IN     CHAR16               *KernelFileName,
     OUT EFI_PHYSICAL_ADDRESS *KernelEntry
  );

#endif
