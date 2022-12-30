#ifndef _LIB_STRING_H_
#define _LIB_STRING_H_

UINTN StrLen (
        IN CONST CHAR8* String
        );

VOID* MemSet (
           OUT VOID  *Buffer,
        IN     INT32 Char,
        IN     UINTN Size
        );

VOID* MemCpy (
           OUT VOID  *Destination,
        IN     VOID  *Source,
        IN     UINTN Count
        );
#endif
