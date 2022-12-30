#ifndef __GRAPHICS_BLOCK_H__
#define __GRAPHICS_BLOCK_H__

RT_STATUS BlockPut (
    COLOR  Color,
    UINT32 SrcX,
    UINT32 SrcY,
    UINT32 DestX,
    UINT32 DestY
    );

RT_STATUS BufferToScreen (
    IN COLOR  *Buffer,
    IN UINT32 BufferWidth,
    IN UINT32 BufferHeight,
    IN UINT32 ScreenX,
    IN UINT32 ScreenY
    );

#endif
