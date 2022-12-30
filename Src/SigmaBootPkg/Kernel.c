#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/SimpleFileSystem.h>

#include <Guid/FileInfo.h>

#include <Boot/Elf.h>
#include <Boot/Kernel.h>
#include <Boot/File.h>
#include <Boot/Tools.h>

EFI_STATUS KernelLoad (
        IN     EFI_HANDLE           ImageHandle,
        IN     CHAR16               *KernelFileName,
           OUT EFI_PHYSICAL_ADDRESS *KernelEntry
        )
{
    EFI_STATUS Status;

    VOID *Kernel = NULL;
    EFI_FILE_PROTOCOL *KernelFile;

    Status = FileOpen (
            KernelFileName,
            EFI_FILE_MODE_READ,
            &KernelFile
            );
    if (EFI_ERROR(Status))
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Open Kernel File\n"));
        return Status;
    }
    DEBUG((DEBUG_INFO,"[ SUCCESS ] Open Kernel File\n"));

    EFI_FILE_INFO *Info = NULL;
    Status = FileGetInfo (
            KernelFile,
            &Info
            );
    if (EFI_ERROR(Status))
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Got Kernel File Info\n"));
        return Status;
    }
    DEBUG((DEBUG_INFO,"[ SUCCESS ] Got Kernel File Info\n"));
    
    UINTN Size  = Info->FileSize;
    DEBUG((DEBUG_INFO,"[  DEBUG  ] Kernel File Size : %u\n",Size));

    Kernel = AllocatePages (
            EFI_SIZE_TO_PAGES(Size)
            );
    if (Kernel == NULL)
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Got Memory Space for Kernel File [Size : %u]\n",Info->Size));
        return Status;
    }
    DEBUG((DEBUG_INFO,"[ SUCCESS ] Got Memory Space for Kernel File [Addr : %p]\n",Kernel));
    FreePool (Info);
    DEBUG((DEBUG_INFO,"[ SUCCESS ] Free Memory of File Info\n"));

    Status = FileRead (
            KernelFile,
            Kernel,
            &Size
            );
    if (EFI_ERROR(Status))
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Read Kernel File [Addr : %p]\n",Kernel));
        return Status;
    }
    DEBUG((DEBUG_INFO,"[ SUCCESS ] Read Kernel File [Addr : %p]\n",Kernel));
    
    Status = LoadSegments (
            Kernel,
            (VOID*)KernelEntry
            );
    DEBUG((DEBUG_INFO,"[ SUCCESS ] Load Segs and Get the Addr of Entry Point [Addr : 0x%x]\n",*KernelEntry));

    return Status;
}
