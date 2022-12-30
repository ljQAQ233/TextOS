#include <TextOS/TextOS.h>
#include <TextOS/Debug.h>
#include <TextOS/Console.h>

#include "Buffer.h"

VOID LogBufferUpdate (
        IN CHAR8 Char
        )
{
    if (!Console.Log) {
        return;
    }

    Console.LogIndex++;
    Console.LogIndex %= __LOG_BUFFER_IDXMAX__;

    Console.LogBuffer[Console.LogIndex] = Char;
    Console.LogBuffer[Console.LogIndex+1] = '\0';
}

CHAR8 LogBufferRead (
        IN     UINT64 *Location,
           OUT CHAR8  *Char
        )
{
    *Location %= __LOG_BUFFER_IDXMAX__;
    *Char = Console.LogBuffer[(*Location)++];

    return *Char;
}

UINT64 LogBufferLocatePrevLine (
        IN     UINT64 Start,
           OUT UINT64 *Location
        )
{
    UINT64 Idx       = Start - 1;
    UINT64 CharCount = __LOG_BUFFER_IDXMAX__;

    do {
        if (Idx == 0) {
            Idx = __LOG_BUFFER_IDXMAX__;
        } else {
            Idx = Idx - 1;
        }
    } while (Console.LogBuffer[Idx] != '\0' && Console.LogBuffer[Idx] != '\n' && --CharCount);

    *Location = Idx;
    return Idx;
}

UINT64 LogBufferLocateNextLine (
        IN     UINT64 Start,
           OUT UINT64 *Location
        )
{
    UINT64 Idx       = Start + 1;
    UINT64 CharCount = 0;

    do {
        Idx++;
        Idx %= __LOG_BUFFER_IDXMAX__;
    } while (Console.LogBuffer[Idx] != '\0' && Console.LogBuffer[Idx] != '\n' && --CharCount);

    *Location = Idx;
    return Idx;
}
