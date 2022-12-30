#ifndef __BOOT_H__
#define __BOOT_H__

#ifndef __TEXT_OS__

#define SETTING_PATH L"\\Setting.ini"

#include <Boot/Basic.h>
#include <Boot/Setting.h>

extern INI_INFO Setting;

#endif

/*

#pragma pack(1)

typedef struct {
  PHYSICAL_ADDRESS FrameBufferBase;
  UINTN            FrameBufferSize;
  UINT32           HorizontalResolution;
  UINT32           VerticalResolution;
  UINT32           PixelsPerScanLine;
  struct {
      UINT8  *Base;
      UINT8  Width;
      UINT8  Height;
  } Font;
} GRAPHICS_CONFIG;

typedef struct {
  GRAPHICS_CONFIG  Graphics;
  PHYSICAL_ADDRESS Logo;
  UINT64           Version;
  PHYSICAL_ADDRESS UefiFunc;
} BOOT_CONFIG;

#pragma pack()

*/

#ifdef __TEXT_OS__

extern GRAPHICS_CONFIG *GraphicsConfig;

#endif

#endif
