#include <TextOS/TextOS.h>
#include <TextOS/Graphics.h>
#include <TextOS/Graphics/Block.h>

#include <TextOS/Tools.h>

RT_STATUS BlockPut (
    COLOR  Color,
    UINT32 SrcX,
    UINT32 SrcY,
    UINT32 DestX,
    UINT32 DestY
    )
{
    RT_STATUS RtStatus;

    if (SrcX > DestX) {
        SwapU32 (&SrcX,&DestX);
    }

    if (SrcY > DestY) {
        SwapU32 (&SrcY,&DestY);
    }
/*
    DestX = MAX (SrcX,DestX);
    DestY = MAX (SrcY,DestY);
*/

    for (UINT32 iY = SrcY;iY < DestY && iY < GraphicsConfig->VerticalResolution;iY++) {
        for (UINT32 iX = SrcX;iX < DestX && iX < GraphicsConfig->HorizontalResolution;iX++) {
            RtStatus = PixelPut (
                Color,
                iX,iY
                );
        }
    }

    return RtStatus;
}

RT_STATUS BufferToScreen (
    IN COLOR  *Buffer,
    IN UINT32 BufferWidth,
    IN UINT32 BufferHeight,
    IN UINT32 ScreenX,
    IN UINT32 ScreenY
    )
{
    RT_STATUS RtStatus;

    for (UINT32 BufferIdxY = 0;BufferIdxY < BufferWidth;BufferIdxY++) {
        for (UINT32 BufferIdxX = 0;BufferIdxX < BufferHeight;BufferIdxX++) {
            RtStatus = PixelPut (
                    Buffer[BufferIdxY * BufferWidth + BufferIdxX],
                    ScreenX + BufferIdxX,
                    ScreenY + BufferIdxY
                );
        }
    }

    return RtStatus;
}
