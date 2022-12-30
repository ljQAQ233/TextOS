#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Guid/FileInfo.h>

#include <Boot/Boot.h>
#include <Boot/File.h>
#include <Boot/Setting.h>
#include <Boot/Graphics.h>

EFI_STATUS FontLoad (
        IN     EFI_HANDLE  ImageHandle,
        IN     CHAR16      *FileName,
           OUT FONT_CONFIG *FontConfig
        )
{
    EFI_STATUS Status = EFI_SUCCESS;

    EFI_FILE_PROTOCOL *FontFile;
    Status = FileOpen (
            FileName,
            EFI_FILE_MODE_READ,
            &FontFile
        );

    FileReadAuto (
            FontFile,
            (VOID**)&FontConfig->Base,
            NULL
        );

    FontConfig->AscOff = SettingGetValueU64 (L"FontAscOffset");
    FontConfig->Width  = SettingGetValueU64 (L"FontWidth");
    FontConfig->Height = SettingGetValueU64 (L"FontHeight");

    if (FontConfig->Height == 0 || FontConfig->Width == 0)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Invalid Font Width or Height\n"));
        return Status;
    }

    return Status;
}

EFI_STATUS FontShow (
        IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
        IN FONT_CONFIG                   FontConfig,
        IN UINT8                         Code,
        IN UINT32                        X,
        IN UINT32                        Y,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL FGColor,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL BGColor
        )
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (Code < 32 || Code > 126)
    {
        DEBUG ((DEBUG_INFO,"[ WARNING ] Invalid Char \"%u\" to print\n",Code));
    }
    Code += FontConfig.AscOff - 32;

    UINTN PerBmpSiz = FontConfig.Width * FontConfig.Height / 8;
    /* Locate the address of start of bitmap,this pointer points the start of bitmap data,the size of bitmap in betys is (Width * Height / 8) */
    UINT8* Base = FontConfig.Base + Code * PerBmpSiz;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Pixel = AllocatePages (EFI_SIZE_TO_PAGES (PerBmpSiz));

    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelPtr = Pixel;
    for (UINT64 Idx = 0;Idx < PerBmpSiz;Idx++)
    {
        for (UINT8 BitIdx = 0;BitIdx < 8;BitIdx++)
        {
            if (((Base[Idx] >> (7 - BitIdx)) & 0b0000001) == 1)
            {
                *PixelPtr = FGColor;
            }
            else
            {
                *PixelPtr = BGColor;
            }
            PixelPtr++;
        }
    }

    Status = Gop->Blt (
            Gop,
            Pixel,
            EfiBltBufferToVideo,
            0,0,
            X,Y,
            FontConfig.Width,FontConfig.Height,
            0
            );

    FreePages (Pixel,EFI_SIZE_TO_PAGES (PerBmpSiz));

    return Status;
}
