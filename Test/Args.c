#include <../Kernel/Arch/X64/Type.h>
#include <TextOS/TextOS.h>
#include <TextOS/Args.h>

#include <stdio.h>

void TestArg(int Num,...)
{
    VA_LIST Args;
    VA_START (Args,Num);

    int Arg;
    for (int Idx = 0;Idx < Num;Idx++) {
        Arg = VA_ARG (Args,int);
        printf ("Arg %d : %x\n",Idx,Arg);
    }

    VA_END (Args);
}

int main()
{
    TestArg (5,0x1111,0x2222,0x3333,0x4444,0x5555);
    return 0;
}
