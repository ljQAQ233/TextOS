#include <TextOS/TextOS.h>

VOID SwapU32 (
        IN OUT UINT32 *ArgX,
        IN OUT UINT32 *ArgY
        )
{
    if (ArgX == NULL || ArgY == NULL) {
        return;
    }

    UINT32 Tmp = *ArgX;
    *ArgX = *ArgY;
    *ArgY = Tmp;
}
