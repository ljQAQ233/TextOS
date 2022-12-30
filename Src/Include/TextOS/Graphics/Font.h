#ifndef __GRAPHICS_FONT_H__
#define __GRAPHICS_FONT_H__

RT_STATUS FontShow (
        IN UINT8       AscCode,
        IN UINT32      X,
        IN UINT32      Y,
        IN COLOR       FGColor,
        IN COLOR       BGColor,
        IN FONT_CONFIG Font
        );

#endif
