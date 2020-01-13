/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: main.h
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 链式存储方式实现队列的功能
*Date:     2020-01-01
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "LinkQueue.h"



int main(int argc, char *argv[]) 
{
    LINK_QUEUE_S* l_pstQueue = NULL;
    LINK_QUEUE_DATA_S l_stData = {0};
    LINK_QUEUE_DATA_S l_stOutData = {0};

    l_pstQueue = LinkQueue_Create();
    for(int i=0; i<10; i++)
    {
        l_stData.s32Data1++;
        
        LinkQueue_Append(l_pstQueue, &l_stData);
    }
    
    printf("Length: %d\n", LinkQueue_Length(l_pstQueue));

    if(0==LinkQueue_Header(l_pstQueue,&l_stOutData))
    {
        printf("out put data1=%d \n",l_stOutData.s32Data1);
    }
    
    LinkQueue_Clear(l_pstQueue);
    
    while( LinkQueue_Length(l_pstQueue) > 0 )
    {
        if(0==LinkQueue_Header(l_pstQueue,&l_stOutData))
        {
            printf("out put data1=%d \n",l_stOutData.s32Data1);
        }
    }
    
    LinkQueue_Destroy(l_pstQueue);
    
	return 0;
}
