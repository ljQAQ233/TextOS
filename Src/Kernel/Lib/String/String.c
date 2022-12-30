#include <TextOS/TextOS.h>
#include <TextOS/Lib/String.h>

UINTN StrLen (
        CONST CHAR8* String
        )
{
    UINTN Length = 0;
    while (*String++ != '\0')
    {
        Length++;
    }

    return Length;
}
