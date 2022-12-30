#pragma once

VOID LogBufferUpdate (
        IN CHAR8 Char
        );

CHAR8 LogBufferRead (
        IN     UINT64 *Location,
           OUT CHAR8  *Char
        );

UINT64 LogBufferLocatePrevLine (
        IN     UINT64 Start,
           OUT UINT64 *Location
        );

UINT64 LogBufferLocateNextLine (
        IN     UINT64 Start,
           OUT UINT64 *Location
        );

