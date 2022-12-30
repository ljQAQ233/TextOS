#include <TextOS/TextOS.h>

VOID* MemSet (
           OUT VOID  *Buffer,
        IN     INT32 Char,
        IN     UINTN Size
        )
{
    while (Size--)
    {
        *(CHAR8*)Buffer++ = Char;
    }

    return Buffer;
}

VOID* MemCpy (
           OUT VOID  *Destination,
        IN     VOID  *Source,
        IN     UINTN Count
        )
{
    while (Count--)
    {
        *(UINT8*)Destination++ = *(UINT8*)Source++;
    }

    return Destination;
}
