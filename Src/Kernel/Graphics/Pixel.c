#include <TextOS/TextOS.h>
#include <TextOS/Graphics.h>
#include <TextOS/Graphics/Pixel.h>

RT_STATUS PixelPut (
    COLOR  Color,
    UINT32 X,
    UINT32 Y
    )
{
    if (X > GraphicsConfig->HorizontalResolution || Y > GraphicsConfig->VerticalResolution) {
        return RT_OUT_OF_RANGE;
    }
    /*
    ((COLOR*)GraphicsConfig->FrameBufferBase)[((Y * GraphicsConfig->HorizontalResolution) + X)] = Color;
    */
    COLOR* Pixel = (COLOR*)GraphicsConfig->FrameBufferBase + Y * GraphicsConfig->HorizontalResolution + X;
    *Pixel = Color;

    return RT_SUCCESS;
}

