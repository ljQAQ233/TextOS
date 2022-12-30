#include <TextOS/TextOS.h>
#include <TextOS/Debug.h>
#include <TextOS/Graphics.h>
#include <TextOS/Graphics/Block.h>

#include <TextOS/Console.h>
#include <TextOS/Console/Write.h>

#include <TextOS/Lib/String.h>

#include "Buffer.h"

UINT64  LineIdx,ReadIdx,HistIdx;

VOID ConsoleScroll_LineUp ()
{
    UINT32 *DestIdx = (UINT32*)GraphicsConfig->FrameBufferBase;
    UINT32 *SrcIdx  = (VOID*)DestIdx + Console.RowMem;

    UINTN  FillSiz  = GraphicsConfig->FrameBufferSize - Console.RowMem;
//    PrintK ("FillSiz : %d",)
    while (FillSiz--)
    {
        *DestIdx++ = *SrcIdx++;
    }
}

VOID ConsoleScroll_Up ()
{
    CHAR8   Char;
    UINT16  LineBack   = Console.CurY,
            ScrollIdx  = Console.Col - 1;
    CONSOLE ConsoleBak = Console; /* Make a backup of 'Console' */

    ConsoleDisableLog();    /* Disable LogBuffer's recording */
    ConsoleClear();         /* Clean screen */

    if (Console.LogIndex != HistIdx) {
        LineIdx = Console.LogIndex;
        HistIdx = LineIdx;  /* Update history index */
        while (LineBack--) {
            LogBufferLocatePrevLine (LineIdx,&LineIdx);
        }
    }

    while (ScrollIdx--) {
        ConsoleSetCursor (0,ScrollIdx);
        ReadIdx = LogBufferLocatePrevLine (LineIdx,&LineIdx) + 1;
        while (LogBufferRead (&ReadIdx,&Char) != '\n') {
            ConsoleWriteChar (Char);
        }
        ConsoleWriteChar ('\n');
    }

    Console = ConsoleBak; /* Recovery */
}

VOID ConsoleScroll_Down ()
{
    CHAR8   Char;
    UINT16  ScrollIdx  = 0;
    CONSOLE ConsoleBak = Console; /* Make a backup of 'Console' */

    ConsoleDisableLog();    /* Disable LogBuffer's recording */
    ConsoleClear();         /* Clean screen */

    if (Console.LogIndex != HistIdx) {
        LineIdx = Console.LogIndex;
        HistIdx = LineIdx;  /* Update history index */
    }

    while (ScrollIdx < Console.Col) {
        ConsoleSetCursor (0,ScrollIdx++);
        ReadIdx = LogBufferLocateNextLine (LineIdx,&LineIdx) + 1;
        while (LogBufferRead (&ReadIdx,&Char) != '\n') {
            ConsoleWriteChar (Char);
        }
        ConsoleWriteChar ('\n');
    }

    Console = ConsoleBak; /* Recovery */
}
