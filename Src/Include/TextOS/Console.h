#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#define __LOG_BUFFER_SIZE__   (1 << 14)
#define __LOG_BUFFER_IDXMAX__ (__LOG_BUFFER_SIZE__ - 1) /* Max index num,idx need minus 1,and another space is to save '\0' */

typedef struct {
  UINT32      Hor;    /* HorizontalResolution  */
  UINT32      Ver;    /* VerticalResolution    */
  UINT16      Row;    /* Row count       */
  UINT16      Col;    /* Column count    */
  UINT16      RowMem; /* Size of per row */
  UINT16      CurX;   /* Cursor address X */
  UINT16      CurY;   /* Cursor address Y */
  UINT16      Line;
  BOOLEAN     Sync;
  BOOLEAN     Log;
  CHAR8       *LogBuffer;
  UINT64      LogIndex;
  COLOR       FGColor;
  COLOR       BGColor;
  FONT_CONFIG Font;
} CONSOLE;

extern CONSOLE Console;

VOID ConsoleInit ();
VOID ConsoleClear ();

VOID ConsoleSetCursor (
        IN UINT32 X,
        IN UINT16 Y
        );

/* Allow to record text infomation that ConsoleWrited in LogBuffer */
VOID ConsoleEnableLog ();
/* Don't allow to record text infomation that ConsoleWrited in LogBuffer */
VOID ConsoleDisableLog ();
/* If ConsoleWrite something and it is enabled,that will be shown on the screen immediately */
VOID ConsoleEnableSync ();
/* If ConsoleWrite something and it is disabled,that won't be shown on the screen immediately   */
VOID ConsoleDisableSync ();
/* View last pages */
VOID ConsoleScroll_Up ();
/* If Console is in View mode and is viewing last pages,this is enabled */
// VOID ConsoleScroll_Down ();

#endif

