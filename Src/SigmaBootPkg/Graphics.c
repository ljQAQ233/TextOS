#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>

#include <Guid/FileInfo.h>

#include <Boot/Bmp.h>
#include <Boot/File.h>
#include <Boot/Graphics.h>

EFI_STATUS GraphicsGetGop (
	IN     EFI_HANDLE                     ImageHandle,
	   OUT EFI_GRAPHICS_OUTPUT_PROTOCOL  **Gop
)
{
    EFI_STATUS  Status      = EFI_SUCCESS;
    UINTN       HandleCount = 0;
    EFI_HANDLE  *HandleBuffer;

    Status = gBS->LocateHandleBuffer(
                 ByProtocol,
                 &gEfiGraphicsOutputProtocolGuid,
                 NULL,
                 &HandleCount,
                 &HandleBuffer
             );

    if(EFI_ERROR(Status))
    {
      DEBUG ((DEBUG_INFO ,"[  ERROR  ] Locate HandleBuffer\n"));
    }
    DEBUG ((DEBUG_INFO ,"[ SUCCESS ] Locate HandleBuffer\n"));

    Status = gBS->OpenProtocol(
                 HandleBuffer[0],
                 &gEfiGraphicsOutputProtocolGuid,
                 (VOID **)Gop,
                 ImageHandle,
                 NULL,
                 EFI_OPEN_PROTOCOL_GET_PROTOCOL
             );

    if(EFI_ERROR(Status))
    {
      DEBUG ((DEBUG_INFO ,"[  ERROR  ] Open Protocol\n"));
    }
    DEBUG ((DEBUG_INFO ,"[ SUCCESS ] Open Protocol\n"));

    FreePool(HandleBuffer);
    DEBUG ((DEBUG_INFO ,"[ SUCCESS ] Free Memories of HandleBuffer\n"));

    return Status;
}

EFI_STATUS GraphicsResolutionSet (
    IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    IN  UINTN                        MaxHor
    )
{
    EFI_STATUS Status = EFI_SUCCESS;

    UINTN ScreenMode;

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;

    for (UINTN ModeIndex = 0,SizeOfInfo;ModeIndex < Gop->Mode->MaxMode;ModeIndex++)
    {
        Status = Gop->QueryMode(Gop,ModeIndex,&SizeOfInfo,&Info);
        if (EFI_ERROR (Status))
        {
            DEBUG ((DEBUG_INFO ,"[  ERROR  ] Looked for Screen Mode\n"));
            return Status;
        }
        if (Info->HorizontalResolution <= MaxHor)
        {
            ScreenMode = ModeIndex;
        }
    }
    DEBUG ((DEBUG_INFO ,"[  SUCCESS  ] Looked for Screen Mode\n"));
    
    Status = Gop->SetMode (Gop,ScreenMode);
    if (EFI_ERROR (Status))
    {
        DEBUG ((DEBUG_INFO ,"[  ERROR  ] Set Screen Mode\n"));
        return Status;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Set Screen Mode\n"));

    FreePool (Info);
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Free Memories for Info\n"));

    return Status;
}

EFI_STATUS GraphicsShowBmpI (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
    IN BMP_CONFIG                    BmpConfig,
    IN UINTN                         X,
    IN UINTN                         Y
)
{
    EFI_STATUS  Status = EFI_SUCCESS;

    Status = Gop->Blt (
                 Gop,
                 (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)BmpConfig.PixelData,
                 EfiBltBufferToVideo,
                 0,0,
                 X,Y,
                 BmpConfig.Header.Width,
                 BmpConfig.Header.Height,
                 BmpConfig.Header.Width * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
             );
    if (EFI_ERROR (Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Graph Block Transfered\n"));
        return Status;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Graph Block Transfered\n"));

    return Status;
}

EFI_STATUS GraphicsShowBmp (
    IN EFI_HANDLE                    ImageHandle,
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
    IN CHAR16                        *BmpName,
    IN UINTN                         X,
    IN UINTN                         Y
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    BMP_CONFIG BmpConfig;
    Status = BmpGetBmpConfig (ImageHandle,BmpName,&BmpConfig);
    if (EFI_ERROR (Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Got BmpConfig\n"));
        return Status;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Got BmpConfig\n"));

    Status = GraphicsShowBmpI (Gop,BmpConfig,0,0);
    if (EFI_ERROR (Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Used BmpConfig to Show Image\n"));
        return Status;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Used BmpConfig to Show Image\n"));

    Status = BmpRemoveBmpConfig (BmpConfig);
    if (EFI_ERROR (Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Removed BmpConfig\n"));
        return Status;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Removed BmpConfig\n"));
    
    return Status;
}

EFI_STATUS GraphicsPutPixel (
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
	IN UINTN                         X,
	IN UINTN                         Y,
	IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color
	)
{
    return Gop->Blt (
	    Gop,
	    &Color,
	    EfiBltVideoFill,
	    0,0,
	    X,Y,
	    1,1,
	    0
	    );
}

EFI_STATUS GraphicsDrawLine (
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
	IN UINTN                         SourceX,
	IN UINTN                         SourceY,
	IN UINTN                         EndX,
	IN UINTN                         EndY,
	IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color
	)
{
    DEBUG ((DEBUG_INFO,"[  JOKES  ] Coo~Coo~Coo~Coo~Coo~Coo~\n"));
    return EFI_UNSUPPORTED;
    /*
    EFI_STATUS Status = EFI_SUCCESS;

    UINTN Hor = Gop->Mode->Info->HorizontalResolution;
    UINTN Ver = Gop->Mode->Info->VerticalResolution;
    INTN  XoYEndX = EndX - SourceX;
    INTN  XoYEndY = EndY - SourceY;
    INTN  PointY , PointX;

    if(ABS(XoYEndX) > Hor || ABS(XoYEndY) > Ver)
    {
	DEBUG((DEBUG_INFO,"[  ERROR  ] Draw Line - Invalid Parameters [Hor : %u,Ver : %u]\n",Hor,Ver));
	return EFI_INVALID_PARAMETER;
    }

    for(UINTN IndexX = 0;IndexX < ABS(XoYEndX) ;IndexX++)
    {
	PointY = (INTN)(((IndexX) * (XoYEndY / XoYEndX) + SourceY) + 0.5);
	PointX = IndexX + SourceX;
	Status = GraphicsPutPixel(
		Gop,
		PointX,PointY,
		Color
		);
	if(EFI_ERROR(Status))
	{
	    DEBUG((DEBUG_INFO,"[  ERROR  ] Put Pixel [PointX : %d,PointY : %d]\n",PointX,PointY));
	    return Status;
	}
    }
    return Status;
    */
}
