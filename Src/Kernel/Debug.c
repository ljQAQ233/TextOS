#include <TextOS/TextOS.h>
#include <TextOS/Args.h>
#include <TextOS/Console.h>
#include <TextOS/Console/Print.h>

#include <TextOS/Lib/VSPrintf.h>

STATIC CHAR8 Buffer[128] = {0};

INT32 DebugK (
        IN CONST CHAR8* File,
        IN CONST UINTN  Line,
        IN CONST CHAR8* Format,
        IN ...
        )
{
    VA_LIST Args;
    UINTN   Length;

    CONSOLE ConsoleBak = Console;

    ConsoleDisableLog();
    ConsoleEnableSync();

    VA_START (Args,Format);

    VSPrintf (
            Buffer,
            Format,
            Args
        );

    VA_END (Args);

    Length = PrintK ("[DEBG] [File:%s,Line:%u] %s",File,Line,Buffer);
    Console = ConsoleBak;

    return Length;
}
