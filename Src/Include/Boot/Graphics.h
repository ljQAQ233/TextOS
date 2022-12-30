#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <Boot/Bmp.h>
#include <Boot/Font.h>

EFI_STATUS
GraphicsGetGop (
  IN     EFI_HANDLE                    ImageHandle,
     OUT EFI_GRAPHICS_OUTPUT_PROTOCOL  **Gop
  );

EFI_STATUS
GraphicsResolutionSet (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
  IN  UINTN                        MaxHor
  );

EFI_STATUS
GraphicsShowBmp (
  IN EFI_HANDLE                    ImageHandle,
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
  IN CHAR16                        *BmpName,
  IN UINTN                         X,
  IN UINTN                         Y 
  );

EFI_STATUS
GraphicsShowBmpI (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
  IN BMP_CONFIG                    BmpConfig,
  IN UINTN                         X,
  IN UINTN                         Y
  );

EFI_STATUS
GraphicsPutPixel (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
  IN UINTN                         X,
  IN UINTN                         Y,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color
  );

EFI_STATUS
GraphicsDrawLine(
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop,
  IN UINTN                         SourceX,
  IN UINTN                         SourceY,
  IN UINTN                         EndX,
  IN UINTN                         EndY,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color
  );

#endif
