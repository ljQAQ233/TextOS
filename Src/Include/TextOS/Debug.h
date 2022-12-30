#ifndef __DEBUG_H__
#define __DEBUG_H__

INT32 DebugK (
        IN CONST CHAR8* File,
        IN CONST UINTN  Line,
        IN CONST CHAR8* Format,
        IN ...
        );

#define DEBUGK(Format,ARGS...) \
            DebugK(__FILE__, __LINE__, Format, ##ARGS)

#endif