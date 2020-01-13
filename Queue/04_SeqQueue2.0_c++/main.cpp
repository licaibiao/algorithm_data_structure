/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: main.cpp
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
    SEQ_QUEUE_DATA_S l_stData = {0};
    SEQ_QUEUE_DATA_S l_stDataOut = {0};
    SEQ_QUEUE_DATA_S *l_pstData = &l_stData;
    SEQ_QUEUE_DATA_S *l_pstDataOut = &l_stDataOut;

    SeqQueue *l_clsSeqQueue;
    
    l_clsSeqQueue = new SeqQueue();

    if(0!=l_clsSeqQueue->SeqQueue_Create(30))
    {
        printf("%s %d create queue error \n",__FUNCTION__,__LINE__);
        return 0;
    }

    for(int i=0; i<300; i++)
    {
        l_stData.s32Data1++;
        l_clsSeqQueue->SeqQueue_InputNode(l_stData);
    }
    
    printf("Length: %d\n", l_clsSeqQueue->SeqQueue_GetLength());
    printf("Capacity: %d\n", l_clsSeqQueue->SeqQueue_GetCapacity());
    
    while(l_clsSeqQueue->SeqQueue_GetLength()>0)
    {
        l_clsSeqQueue->SeqQueue_OutPutNode(l_pstDataOut);
        printf("out: data1=%d data2=%d \n",l_pstDataOut->s32Data1,l_pstDataOut->s32Data2);
    }
    
    printf("\n");
    
    for(int i=0; i<100; i++)
    {
        l_stData.s32Data1++;
        
        l_clsSeqQueue->SeqQueue_InputNode(l_stData);

        if(0==l_clsSeqQueue->SeqQueue_OutPutNode(l_pstDataOut))
        {
            printf("out: data1=%d data2=%d \n",l_pstDataOut->s32Data1,l_pstDataOut->s32Data2);   
        }else
        {
            printf("%s %d output data error \n",__FUNCTION__,__LINE__);
        }
    }
    
    l_clsSeqQueue->SeqQueue_Destroy();
    
    return 0;
}

