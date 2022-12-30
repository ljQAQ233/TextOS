#include <TextOS/TextOS.h>
#include <TextOS/Console.h>
#include <TextOS/Graphics/Font.h>
#include <TextOS/Graphics/Color.h>

#include <TextOS/Lib/String.h>

#include "Scroll.h"

RT_STATUS ConsoleWriteChar (
        IN CONST CHAR8 Letter
        )
{
    LogBufferUpdate (Letter);
    if (Letter == '\n') {
        Console.CurX = 0;
        if (++Console.CurY == Console.Col) {
            Console.CurY--;
            ConsoleScroll_LineUp ();
        }
        return RT_SUCCESS;
    }

    UINT32 X = Console.CurX * Console.Font.Width,
           Y = Console.CurY * Console.Font.Height;
    if (++Console.CurX == Console.Row) {
        LogBufferUpdate ('\n');
        Console.CurX = 0;
        if (++Console.CurY == Console.Col) {
            Console.CurY--;
            ConsoleScroll_LineUp ();
        }
    }

    if (Console.Sync) {
        return FontShow (Letter,X,Y,Console.FGColor,Console.BGColor,Console.Font);
    }
}

RT_STATUS ConsoleWrite (
        IN CONST CHAR8* String
        )
{
    RT_STATUS RtStatus = RT_SUCCESS;

    for (UINTN Idx = 0;Idx < StrLen (String);Idx++) {
        RtStatus = ConsoleWriteChar (String[Idx]);
    }

    return RtStatus;
}

VOID ConsoleUpdate ()
{
    /* If sync is opened,then exit */
    if (Console.Sync) {
        return;
    }
}
