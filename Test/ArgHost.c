#include <stdio.h>
#include <stdarg.h>

void TestArg(int Num,...)
{
    va_list Args;
    va_start (Args,Num);

    int Arg;
    for (int Idx = 0;Idx < Num;Idx++) {
        Arg = va_arg (Args,int);
        printf ("Arg %d : %x\n",Idx,Arg);
    }

    va_end (Args);
}

int main()
{
    TestArg (5,0x1111,0x2222,0x3333,0x4444,0x5555);
    return 0;
}
