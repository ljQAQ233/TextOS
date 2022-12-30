#ifndef __FONT_H__
#define __FONT_H__

typedef struct {
  UINT8  *Base;
  UINT32 AscOff;
  UINT8  Width;
  UINT8  Height;
} FONT_CONFIG;

EFI_STATUS FontLoad (
        IN     EFI_HANDLE  ImageHandle,
        IN     CHAR16      *FileName,
           OUT FONT_CONFIG *FontConfig
        );

EFI_STATUS FontShow (
        IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
        IN FONT_CONFIG                   FontConfig,
        IN UINT8                         Code,
        IN UINT32                        X,
        IN UINT32                        Y,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL FGColor,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL BGColor
        );

#endif
