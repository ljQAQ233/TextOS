#ifndef __TYPE_H__
#define __TYPE_H__

#include <TypeArc.h>

typedef UINT32 COLOR;

typedef struct {
  UINT8  *Base;
  UINT32 AscOff;
  UINT8  Width;
  UINT8  Height;
} FONT_CONFIG;

#endif
