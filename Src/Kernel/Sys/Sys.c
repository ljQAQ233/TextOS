#include <TextOS/TextOS.h>

VOID SysStop ()
{
    while (TRUE); 
}

VOID Break ()
{
    #ifdef __TEXTOS_DEBUG__
        __asm__ ("int $3");
    #endif
}
