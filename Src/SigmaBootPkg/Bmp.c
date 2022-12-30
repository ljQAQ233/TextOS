#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/FileInfo.h>
#include <Library/DebugLib.h>

#include <Boot/File.h>
#include <Boot/Tools.h>
#include <Boot/Graphics.h>

EFI_STATUS BmpCheckFormat (
        BMP_IMAGE_HEADER BmpHeader
        )
{
    DEBUG((DEBUG_INFO,"[  DEBUG  ] Check bmp format\n"));
    if (BmpHeader.CharB != 'B' || BmpHeader.CharM != 'M')
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Invalid Bmp header format in header symbol\n"));
        return EFI_INVALID_PARAMETER;
    }

    if (BmpHeader.Width <= 0)
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Invalid Bmp header format in pixel width\n"));
        return EFI_INVALID_PARAMETER;
    }

    if (BmpHeader.ImageBits != 1 && BmpHeader.ImageBits != 4 && BmpHeader.ImageBits != 8 && BmpHeader.ImageBits != 24 && BmpHeader.ImageBits != 32)
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Invalid Bmp header format in bits\n"));
        return EFI_INVALID_PARAMETER;
    }

    if (BmpHeader.CompressionType != 0)
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Invalid Bmp CompressionType\n"));
        return EFI_UNSUPPORTED;
    }

    if (BmpHeader.ImageOffset < sizeof(BMP_IMAGE_HEADER))
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Invalid Bmp ImageOffset\n"));
        return EFI_INVALID_PARAMETER;
    }

    if (BmpHeader.ImageOffset - sizeof(BMP_IMAGE_HEADER) < sizeof(BMP_COLOR_MAP) * ((BmpHeader.ImageBits == 24) ? 0 : (uPower(2,BmpHeader.ImageBits))))
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Invalid Bmp color maps\n"));
        return EFI_INVALID_PARAMETER;
    }

    return EFI_SUCCESS;
}


VOID BmpTranslate (
    IN     VOID                           *BmpFileRaw,
       OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *Out
)
{
    BMP_IMAGE_HEADER               *BmpHeader   = (BMP_IMAGE_HEADER *)BmpFileRaw;
    DEBUG ((DEBUG_INFO,"[  GRAPH  ] Width : %d , Height : %d , ImageOffset : %d\n",BmpHeader->Width,BmpHeader->Height,BmpHeader->ImageOffset));
    UINT8                          *RawIdx     = BmpFileRaw + BmpHeader->ImageOffset;
    BMP_COLOR_MAP                  *ColorMap = (BMP_COLOR_MAP *)(BmpFileRaw + sizeof(BMP_IMAGE_HEADER));
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *OutIdx = Out;

    BOOLEAN HeightPositive = (BOOLEAN)(BmpHeader->Height > 0);

    for (UINTN HeightIndex = HeightPositive ? BmpHeader->Height - 1 : 0; HeightPositive ? (HeightIndex > 0) : (HeightIndex < ABS(BmpHeader->Height));HeightPositive ? (HeightIndex--) : (HeightIndex++))
    {
        for (UINTN WidthIdx = 0;WidthIdx < BmpHeader->Width;RawIdx++)
        {
            OutIdx = &Out[BmpHeader->Width * HeightIndex + WidthIdx];
            if (BmpHeader->ImageBits == 1)
            {
                for(UINTN BitIndex = 0;(BitIndex < 8) && WidthIdx < BmpHeader->Width;BitIndex++,WidthIdx++)
                {
                    OutIdx->Blue  = ColorMap[((*RawIdx) >> (7 - BitIndex)) & 0b1].Blue;
                    OutIdx->Green = ColorMap[((*RawIdx) >> (7 - BitIndex)) & 0b1].Green;
                    OutIdx->Red   = ColorMap[((*RawIdx) >> (7 - BitIndex)) & 0b1].Red;
                }
            }
            else if (BmpHeader->ImageBits == 4)
            {
                OutIdx->Blue  = ColorMap[(*RawIdx) >> 4].Blue;
                OutIdx->Green = ColorMap[(*RawIdx) >> 4].Green;
                OutIdx->Red   = ColorMap[(*RawIdx) >> 4].Red;
                WidthIdx++;
                if (WidthIdx < BmpHeader->Width)
                {
                    OutIdx        = &Out[BmpHeader->Width * HeightIndex + WidthIdx];
                    OutIdx->Blue  = ColorMap[(*RawIdx) & 0b1111].Blue;
                    OutIdx->Green = ColorMap[(*RawIdx) & 0b1111].Green;
                    OutIdx->Red   = ColorMap[(*RawIdx) & 0b1111].Red;
                    WidthIdx++;
                }
            }
            else if (BmpHeader->ImageBits == 8)
            {
                OutIdx->Blue  = ColorMap[*RawIdx].Blue;
                OutIdx->Green = ColorMap[*RawIdx].Green;
                OutIdx->Red   = ColorMap[*RawIdx].Red;
                WidthIdx++;
            }
            else if (BmpHeader->ImageBits == 24)
            {
                OutIdx->Blue  = *RawIdx++;
                OutIdx->Green = *RawIdx++;
                OutIdx->Red   = *RawIdx;
                WidthIdx++;
            }
            // DEBUG((DEBUG_INFO,"[  DEBUG  ] WidthIdx : %d,HeightIndex : %d\n",WidthIdx,HeightIndex));
        }
        // DEBUG((DEBUG_INFO,"[  DEBUG  ] HeightIndex : %d\n",HeightIndex));
    }
}

EFI_STATUS BmpGetBmpConfig (
    IN     EFI_HANDLE                    ImageHandle,
    IN     CHAR16                        *BmpFileName,
       OUT BMP_CONFIG                    *BmpConfig
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_FILE_PROTOCOL  *BmpFile;
    BMP_IMAGE_HEADER   Header;

    Status = FileOpen (
                 BmpFileName,
                 EFI_FILE_MODE_READ,
                 &BmpFile
             );
    if(EFI_ERROR (Status))
    {
            DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to Open File - %s\n",BmpFileName));
            return Status;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Open File Successfully - %s\n",BmpFileName));

    UINTN ReadSize = sizeof(BMP_IMAGE_HEADER);
    Status   = FileRead (
                   BmpFile,
                   (VOID *)&Header,
                   &ReadSize
               );
    if (EFI_ERROR (Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to read file to get the header data of the image\n"));
        return Status;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Read File and got the data of bmpheader\n"));

    Status = BmpCheckFormat (Header);
    if (EFI_ERROR (Status))
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Invalid bmp format\n"));
        return Status;
    }
    DEBUG((DEBUG_INFO,"[ SUCCESS ] Passed the bmp format test\n"));

    Status = BmpFile->SetPosition (
            BmpFile,
            0
            );
    if (EFI_ERROR (Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to set position to zero\n"));
        return Status;
    }

    UINTN RawPages = EFI_SIZE_TO_PAGES (Header.Size);
    VOID  *BmpFileRaw     = AllocatePages (RawPages);
    if (BmpFileRaw == NULL)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Got memory space for BmpBuffer\n"));
        return EFI_OUT_OF_RESOURCES;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Got memory space for BmpBuffer\n"));

    ReadSize = Header.Size;
    Status   = FileRead (
                   BmpFile,
                   BmpFileRaw,
                   &ReadSize
               );
    
    /*
    Status = FileClose(BmpFile);
    if (EFI_ERROR (Status))
    {
        DEBUG((DEBUG_INFO,"[  ERROR  ] Closed the file - %s\n",BmpFileName));
        return Status;
    }
    */

    UINTN OutPages = EFI_SIZE_TO_PAGES (ABS (Header.Height) * ABS (Header.Width) * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *Out    = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)AllocatePages (OutPages);
    if (Out == NULL)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Got memory space for imageOut,maybe there is no space to use [ Pages - %u ]\n",OutPages));
        return EFI_OUT_OF_RESOURCES;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Got memory space for imageOut\n"));
    
    BmpTranslate (
        BmpFileRaw,
        Out
    );
    DEBUG ((DEBUG_INFO,"[   LOG   ] Bmp image is translated\n"));
    
    BmpConfig->Header    = Header;
    BmpConfig->PixelData = Out;
    BmpConfig->PixelDataPages = OutPages;
    BmpConfig->Header.Width   = ABS (Header.Width);
    BmpConfig->Header.Height  = ABS (Header.Height);

    FreePages (BmpFileRaw,RawPages);

    return Status;
}

EFI_STATUS BmpRemoveBmpConfig (
    BMP_CONFIG BmpConfig
    )
{
    FreePages (BmpConfig.PixelData,BmpConfig.PixelDataPages);

    return EFI_SUCCESS;
}

