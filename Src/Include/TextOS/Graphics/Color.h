#ifndef __GRAPHICS_COLOR_H__
#define __GRAPHICS_COLOR_H__

#pragma pack(1)

typedef struct {
  UINT8   Blue;
  UINT8   Green;
  UINT8   Red;
  UINT8   Reserved;
} COLOR_MAP;

#pragma pack()

#define BGR_TO_COLOR(Blue,Green,Red) \
    (COLOR)(((UINT8)Blue)|((UINT8)Green << 8)|((UINT8)Red << 16))

#define Blue  BGR_TO_COLOR(0xFF,0x00,0x00)
#define Green BGR_TO_COLOR(0x00,0xFF,0x00)
#define Red   BGR_TO_COLOR(0x00,0x00,0xFF)

#define Black BGR_TO_COLOR(0x00,0x00,0x00)
#define White BGR_TO_COLOR(0xFF,0xFF,0xFF)

#endif
