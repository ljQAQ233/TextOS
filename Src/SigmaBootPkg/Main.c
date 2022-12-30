#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Protocol/DebugPort.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Guid/FileInfo.h>

#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>

#include <Boot/Boot.h>
#include <Boot/Debug.h>
#include <Boot/Setting.h>
#include <Boot/Kernel.h>
#include <Boot/File.h>
#include <Boot/Graphics.h>
#include <Boot/Tools.h>

INI_INFO Setting;

EFI_FILE_PROTOCOL *gFileProtocol;

GDB_DEBUG_SUPPORT GdbDebugInfo = {
    .KernelEntry = 0x100000,
    .KernelRuntime = FALSE,
};

typedef struct {
  PHYSICAL_ADDRESS FrameBufferBase;
  UINTN            FrameBufferSize;
  UINT32           HorizontalResolution;
  UINT32           VerticalResolution;
  UINT32           PixelsPerScanLine;
  FONT_CONFIG      Font;
} GRAPHICS_CONFIG;

typedef struct {
  GRAPHICS_CONFIG  Graphics;
  PHYSICAL_ADDRESS Logo;
  UINT64           Version;
} BOOT_CONFIG;

typedef struct
{
    UINTN BufferSize;
    VOID *Buffer;
    UINTN MapSize;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
} MEMORY_MAP;

EFI_STATUS ByeBootServices(
    IN EFI_HANDLE ImageHandle
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    Print(L"\nBye BS.\n");
    MEMORY_MAP MemMap = {4096 * 4, NULL,4096,0,0,0};

    Status = gBS->AllocatePool(
        EfiLoaderData,
        MemMap.BufferSize,
        &MemMap.Buffer
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Allocate pool for memory map failed.\n");
        return Status;
    }    
  
    Status = gBS->GetMemoryMap(
        &MemMap.BufferSize,
        (EFI_MEMORY_DESCRIPTOR *)MemMap.Buffer,
        &MemMap.MapKey,
        &MemMap.DescriptorSize,
        &MemMap.DescriptorVersion
    );

    if(EFI_ERROR(Status))
    {
        Print(L"Get memory map error.\n");
        return Status;
    }
 
    Status = gBS->ExitBootServices(
        ImageHandle, MemMap.MapKey
    );

    if(EFI_ERROR(Status))
    {
        Print(L"ExitBootServices error.\n");
        return Status;
    }

    return Status;
}

EFI_STATUS ExitBootServices (
        IN EFI_HANDLE ImageHandle
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN  MemMapSize = 0;
    UINTN  MapKey = 0;
    UINTN  DescriptorSize = 0;
    UINT32 DescriptorVersion = 0;
    EFI_MEMORY_DESCRIPTOR *MemMap = NULL;
    do {
        if (MemMap == NULL)
        {
            continue;
        }
        FreePool (MemMap);
        MemMap = AllocatePool (DescriptorSize);
    } while(EFI_ERROR(gBS->GetMemoryMap(&MemMapSize,MemMap,&MapKey,&DescriptorSize,&DescriptorVersion)));
    Status = gBS->ExitBootServices(
            ImageHandle,
            MapKey
        );

    return Status;
}

EFI_STATUS EFIAPI UefiMain (
        IN EFI_HANDLE        ImageHandle,
        IN EFI_SYSTEM_TABLE  *SystemTable
        )
{
    Break ();

    EFI_STATUS Status = EFI_SUCCESS;

    Status = InitializeFileServices (ImageHandle);
    Status = SettingInit (ImageHandle);

    EFI_PHYSICAL_ADDRESS KernelEntry;
    Status = KernelLoad (
            ImageHandle,
            SettingGetValueC16 (L"KernelPath"),
            &KernelEntry
        );

    BOOT_CONFIG BootConfig;

    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    Status = GraphicsGetGop (
            ImageHandle,
            &Gop
        );

    BMP_CONFIG LogoConfig;
    BmpGetBmpConfig (ImageHandle,SettingGetValueC16 (L"LogoPath"),&LogoConfig);

    BootConfig.Graphics.HorizontalResolution = Gop->Mode->Info->HorizontalResolution;
    BootConfig.Graphics.VerticalResolution   = Gop->Mode->Info->VerticalResolution;
    BootConfig.Graphics.PixelsPerScanLine    = Gop->Mode->Info->PixelsPerScanLine;

    BootConfig.Graphics.FrameBufferBase = Gop->Mode->FrameBufferBase;
    BootConfig.Graphics.FrameBufferSize = Gop->Mode->FrameBufferSize;

    Status = FontLoad (
            ImageHandle,
            SettingGetValueC16 (L"FontBasePath"),
            &BootConfig.Graphics.Font
            );

    BootConfig.Logo = (PHYSICAL_ADDRESS) &LogoConfig;

    BootConfig.Version = 20221029;

    UINT64 (*OSKernel)(BOOT_CONFIG *BootConfig) = (UINT64(*)(BOOT_CONFIG*))KernelEntry;
    Status = ByeBootServices (ImageHandle);
    DebugKernelSet (KernelEntry);
    UINT64 RtStatus = OSKernel (&BootConfig);
    Print(L"[ INFO ] Kernel Returned : %lld\n",RtStatus);
    Print(L"[ INFO ] Kernel Returned : 0x%llx\n",RtStatus);

    return Status;
}
