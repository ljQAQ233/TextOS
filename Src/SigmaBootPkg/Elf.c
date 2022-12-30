#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Boot/Boot.h>
#include <Boot/Tools.h>
#include <Boot/Kernel.h>

EFI_STATUS ElfCheck (
        IN VOID* ElfBuffer
        )
{
    ELF_HEADER *Header = (ELF_HEADER*) ElfBuffer;
    if (Header->Sign != ELF_MAGIC)
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Not an Elf File\n"));
        return EFI_UNSUPPORTED;
    }

    if (Header->Type != ET_EXEC)
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] This Elf Type was Wrong\n"));
        return EFI_UNSUPPORTED;
    }

    if (Header->Class != ELF_SUPPORTED_CLASS)
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Unsupport the Elf Class\n"));
        return EFI_UNSUPPORTED;
    }

    if (Header->Machine != ELF_SUPPORTED_ARCH)
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Unsupport Elf Machine Type - %u ; Arch : %u\n",Header->Machine,ELF_SUPPORTED_ARCH));
    }

    return EFI_SUCCESS;
}

VOID LoadSegment (
        IN     VOID        *ElfBuffer,
        IN     ELF_PHEADER ProgHdr,
           OUT VOID        *Destination
        )
{
    DEBUG ((DEBUG_INFO,"[  DEBUG  ] CopyMemory 0x%x -> 0x%x,tail 0x%x\n",ElfBuffer + ProgHdr.Offset,Destination + ProgHdr.VirtualAddr,Destination + ProgHdr.VirtualAddr + ProgHdr.FileSiz));
    gBS->CopyMem (
            Destination + ProgHdr.VirtualAddr,
            ElfBuffer + ProgHdr.Offset,
            ProgHdr.FileSiz
            );
}

EFI_STATUS LoadSegments (
        IN     VOID                 *ElfBuffer,
           OUT EFI_PHYSICAL_ADDRESS *KernelEntry
        )
{
    VOID *KernelBuffer = NULL;

    ELF_HEADER  *ElfHdr   = (ELF_HEADER*)ElfBuffer;
    ELF_PHEADER *ProgHdrs = (ELF_PHEADER*)(ElfBuffer + ElfHdr->PhOffset);
    
    /* Get Size of the Programme in Memory */
    UINTN ProgMemSiz = ((UINTN)1 << (sizeof(UINTN) * 8 - 1)) - 1;
    for (UINTN ProgHdrIdx = 0;ProgHdrIdx < ElfHdr->PhNum;ProgHdrIdx++)
    {
        if (ProgHdrs[ProgHdrIdx].Type == PT_LOAD)
        {
            ProgMemSiz = MIN (ProgMemSiz,ProgHdrs[ProgHdrIdx].VirtualAddr);
        }
    }

    for (UINTN ProgHdrIdx = 0;ProgHdrIdx < ElfHdr->PhNum;ProgHdrIdx++)
    {
        if (ProgHdrs[ProgHdrIdx].Type == PT_LOAD)
        {
            ProgMemSiz += ProgHdrs[ProgHdrIdx].MemSiz;
        }
    }
   
    KernelBuffer = AllocatePages (EFI_SIZE_TO_PAGES (ProgMemSiz));
    DEBUG ((DEBUG_INFO,"[  DEBUG  ] Programe memory size : %u\n",ProgMemSiz));

    for (UINTN ProgHdrIdx = 0;ProgHdrIdx < ElfHdr->PhNum;ProgHdrIdx++)
    {
        if (ProgHdrs[ProgHdrIdx].Type == PT_LOAD)
        {
            LoadSegment (
                    ElfBuffer,
                    ProgHdrs[ProgHdrIdx],
                    KernelBuffer
                    );
        }
    }
    DEBUG ((DEBUG_INFO,"[  DEBUG  ] Loaded Kernel\n"));

    *KernelEntry = (EFI_PHYSICAL_ADDRESS)(KernelBuffer + ElfHdr->Entry);

    return EFI_SUCCESS;
}
