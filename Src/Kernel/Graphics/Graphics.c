#include <TextOS/TextOS.h>
#include <TextOS/Graphics.h>
#include <TextOS/Graphics/Block.h>
#include <TextOS/Graphics/Color.h>

VOID ScreenClear ()
{
    BlockPut (
            Black,
            0,0,
            GraphicsConfig->HorizontalResolution,GraphicsConfig->VerticalResolution
        );
}
