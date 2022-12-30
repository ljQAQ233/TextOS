#ifndef __TOOLS_H__
#define __TOOLS_H__

#define SIZ_TO_PAGE(Size) \
    (Size % EFI_PAGE_SIZE == 0) \
    ? (Size / EFI_PAGE_SIZE) \
    : ((Size - (Size % EFI_PAGE_SIZE)) / EFI_PAGE_SIZE + 1)

INTN uPower(
    INTN x,
    UINTN y
    );

UINT64 Char16ToU64(
    IN CHAR16 *String
    );

CHAR16 *U64ToChar16(
    IN  UINT64 Num
    );

#endif
