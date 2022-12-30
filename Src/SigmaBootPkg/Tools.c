#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Boot/Tools.h>

INTN uPower (
    IN INTN x,
    IN UINTN y
    ) // x^y
{
    INTN result = x;
    if (y == 0 && x != 0)
    {
        return 1;
    }

    for (int i = 1;i < y;i++)
    {
        result = result * x;
    }
    return result;
}

UINT64 Char16ToU64 (
    IN CHAR16 *String
    )
{
    UINT64  xNum = 1,
            Result = 0;
    for(UINTN CharAddress = StrLen(String) - 1;CharAddress >= 0;CharAddress--,xNum=xNum*10)
    {
        Result = Result + xNum*( String[CharAddress] - 48 );
    }
    return Result;
}

CHAR16 *U64ToChar16(
    IN  UINT64 Num
    )
{
    UINTN powNum = 0;

    for(;!((Num >= uPower(10,powNum) && (uPower(10,powNum+1) > Num)));powNum++)
    {
        /* Get the length of the number ,
        Only use a recycling,
        but there is nothing to do in here
        */
    }

    UINT64 passVar;
    CHAR16 *Str = (CHAR16*)AllocatePool((powNum+1+1) * sizeof(CHAR16));
    // 在AllocatePool中"powNum +1 +1":第一个加1代表数字长度，第二个是在数字长度的基础上为'\0'保留位置，使用完之后务必FreePool
    UINTN i;
    for(i = 0;powNum > 0;i++,powNum--)
    {
        passVar = (Num - Num % uPower(10,powNum)) / uPower(10,powNum);
        Str[i] = passVar + '0';
        Num = Num % uPower(10,powNum);
    }

    Str[i++] = Num + '0';
    // 紧跟上一步未完成的操作，补充 数字 的最后一位，在使用 i 之后递增 i
    Str[i] = '\0';

    return Str;
}
