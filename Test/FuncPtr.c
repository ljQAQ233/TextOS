#include <stdio.h>

int main()
{
    int (*Ptr)(const char* fmt,...) = (int(*)(const char* fmt,...))printf;
    Ptr("Hello World!\n");
    return 0;
}
