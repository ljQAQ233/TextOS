#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

struct FONT_CONFIG;

typedef struct {
  PHYSICAL_ADDRESS FrameBufferBase;
  UINTN            FrameBufferSize;
  UINT32           HorizontalResolution;
  UINT32           VerticalResolution;
  UINT32           PixelsPerScanLine;
  FONT_CONFIG      Font;
} GRAPHICS_CONFIG;

extern GRAPHICS_CONFIG *GraphicsConfig;

VOID ScreenClear ();

#endif
