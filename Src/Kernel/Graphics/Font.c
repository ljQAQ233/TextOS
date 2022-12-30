#include <TextOS/TextOS.h>
#include <TextOS/Graphics.h>
#include <TextOS/Graphics/Font.h>
#include <TextOS/Graphics/Pixel.h>

RT_STATUS FontShow (
	IN UINT8       AscCode,
    IN UINT32      X,
    IN UINT32      Y,
    IN COLOR       FGColor,
    IN COLOR       BGColor,
    IN FONT_CONFIG Font
    )
{
    if (AscCode < 32 || AscCode > 126) {
        return RT_INVALID_ARGS;
    }
    AscCode += Font.AscOff - 32;
    UINTN PerBmpSiz = Font.Width * Font.Height / 8;

    UINT8 *Base  = Font.Base + AscCode * PerBmpSiz;
    UINT8 Source = 0;
    for (UINT32 YIdx = 0;YIdx < Font.Height;YIdx++) {
        for (UINT32 XIdx = 0;XIdx < Font.Width;XIdx++) {
            Source = Base[(YIdx * Font.Width + XIdx) / 8];
            if ((Source >> (7 - XIdx % 8) & 0b0000001) == 1) {
                PixelPut (
                        FGColor,
                        X + XIdx,Y + YIdx
                    );
            }
            else
            {
                PixelPut (
                        BGColor,
                        X + XIdx,Y + YIdx
                    );
            }
        }
    }

	return RT_SUCCESS;
}
