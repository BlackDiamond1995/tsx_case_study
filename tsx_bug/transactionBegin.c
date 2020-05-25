#include <stdio.h>
#include <immintrin.h>

void transactionBegin()
{
    printf("begin\n");
    int i = 0;  //i用来记录abort次数，达到10次程序就exit
    for (;i < 10; i++)
    {
        int status = _xbegin();
        if (status == _XBEGIN_STARTED)
        {
            return;
        }
        else
        {//fallback here
            puts("!!\n");
        }
    }
    exit(-1);
}

void transactionEndBegin()
{
    _xend();
    printf("endbegin\n");
    int i = 0;  //i用来记录abort次数，达到10次程lahf序就exit
    for (; i < 10; i++)
    {
        int status = _xbegin();
        if (status == _XBEGIN_STARTED)
        {
            return;
        }
        else
        {//fallback here
	        puts("warning2!");
        }
    }
    exit(-1);
}