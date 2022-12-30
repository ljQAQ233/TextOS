#include <TextOS/TextOS.h>
#include <TextOS/Graphics/Block.h>
#include <TextOS/Graphics/Bitmap.h>

RT_STATUS BmpShow (
        IN BMP_CONFIG BmpConfig,
        IN UINT32     ScreenX,
        IN UINT32     ScreenY
        )
{
    return BufferToScreen (
        BmpConfig.PixelData,
        BmpConfig.Header.Width,BmpConfig.Header.Height,
        ScreenX,ScreenY
        );
}
