#include <TextOS/TextOS.h>
#include <TextOS/Graphics.h>
#include <TextOS/Graphics/Color.h>

#include <TextOS/Console.h>

#include <TextOS/Lib/String.h>

CHAR8 LogBuffer[__LOG_BUFFER_SIZE__] = {'\0'};

CONSOLE Console;

VOID ConsoleInit ()
{
    Console.Font    = GraphicsConfig->Font;

    Console.Hor  = GraphicsConfig->HorizontalResolution;
    Console.Ver  = GraphicsConfig->VerticalResolution;

    Console.Row  = Console.Hor / Console.Font.Width;
    Console.Col  = Console.Ver / Console.Font.Height;
    Console.RowMem = Console.Hor * Console.Font.Height * sizeof(COLOR);

    Console.CurX = 0;
    Console.CurY = 0;
    Console.Line = 0;

    Console.FGColor = White;
    Console.BGColor = Black;

    Console.Log       = TRUE;
    Console.LogIndex  = 0;
    Console.LogBuffer = LogBuffer;

    Console.Sync = TRUE;
}

VOID ConsoleClear ()
{
    Console.CurX = 0;
    Console.CurY = 0;
    ScreenClear ();
}

VOID ConsoleSetCursor (
        IN UINT32 X,
        IN UINT16 Y
        )
{
    if (X < Console.Row) {
        Console.CurX = X;
    }
    if (Y < Console.Col) {
        Console.CurY = Y;
    }
}

VOID ConsoleEnableLog ()
{
    Console.Log = TRUE;
}

VOID ConsoleDisableLog ()
{
    Console.Log = FALSE;
}

VOID ConsoleEnableSync ()
{
    Console.Sync = TRUE;
}

VOID ConsoleDisableSync ()
{
    Console.Sync = FALSE;
}
