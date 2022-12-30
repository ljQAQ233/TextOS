#include <TextOS/TextOS.h>
#include <TextOS/Args.h>

#include <TextOS/Graphics.h>
#include <TextOS/Console/Write.h>

#include <TextOS/Lib/VSPrintf.h>

STATIC CHAR8 Buffer[512] = {0};

INT32 PrintK (
        IN CONST CHAR8* Format,
        IN ...
        )
{
    VA_LIST Args;
    UINTN   Length;

    VA_START (Args,Format);

    Length = VSPrintf (
            Buffer,
            Format,
            Args
        );

    VA_END (Args);
    ConsoleWrite (Buffer);

    return Length;
}
