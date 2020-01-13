/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: main.c
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 
*Date:     2020-01-01
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "SeqQueue.h"

int main(int argc, char *argv[]) 
{
    SEQ_QUEUE_S* l_pstQueue = NULL;
    SEQ_QUEUE_DATA_S l_stData = {0};
    SEQ_QUEUE_DATA_S l_stDataOut = {0};
    SEQ_QUEUE_DATA_S *l_pstData = NULL;
    SEQ_QUEUE_DATA_S *l_pstDataOut = NULL;

    l_pstQueue = SeqQueue_Create(30);

    for(int i=0; i<300; i++)
    {
        l_stData.s32Data1++;
        
        SeqQueue_Append(l_pstQueue,&l_stData);
    }
    
    printf("Length: %d\n", SeqQueue_Length(l_pstQueue));
    printf("Capacity: %d\n", SeqQueue_Capacity(l_pstQueue));
    
    while(SeqQueue_Length(l_pstQueue)>0)
    {
        l_pstDataOut = SeqQueue_Retrieve(l_pstQueue);
        printf("out: data1=%d data2=%d \n",l_pstDataOut->s32Data1,l_pstDataOut->s32Data2);
    }
    
    printf("\n");
    
    for(int i=0; i<100; i++)
    {
        l_stData.s32Data1++;
        
        SeqQueue_Append(l_pstQueue, &l_stData);

        l_pstDataOut = SeqQueue_Retrieve(l_pstQueue);
        printf("out: data1=%d data2=%d \n",l_pstDataOut->s32Data1,l_pstDataOut->s32Data2);
    }
    
    SeqQueue_Destroy(l_pstQueue);
    
    return 0;
}
