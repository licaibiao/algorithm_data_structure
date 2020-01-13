/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: SeqQueue.c
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 
*Date:     2020-01-01
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "SeqQueue.h"


/******************************************************** 
Function: SeqQueue_Create    
Description: 创建队列
Input:  s32Capacity： 队列元素的个数
OutPut: none
Return: 队列指针
Others: 
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
SEQ_QUEUE_S* SeqQueue_Create(int s32Capacity) 
{
    SEQ_QUEUE_S* l_pstRet = NULL;
    
    if( s32Capacity >= 0 )
    {
        l_pstRet = (SEQ_QUEUE_S*)malloc(sizeof(SEQ_QUEUE_S) + sizeof(SEQ_QUEUE_DATA_S) * s32Capacity);
    }
    
    if( l_pstRet != NULL )
    {
        l_pstRet->s32Capacity = s32Capacity;
        l_pstRet->s32Length = 0;
        l_pstRet->s32Front = 0;
        l_pstRet->s32Rear = 0;
        l_pstRet->stQueueData = (SEQ_QUEUE_DATA_S*)(l_pstRet + 1);
    }
    
    return l_pstRet;
}

/******************************************************** 
Function: SeqQueue_Destroy    
Description: 销毁一条队列
Input:  pstQueue 队列指针
OutPut: none
Return: 0 成功；非0 失败
Others: 
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
int SeqQueue_Destroy(SEQ_QUEUE_S* pstQueue) 
{
    if(NULL!=pstQueue)
    {
        free(pstQueue);
        pstQueue = NULL;
    }

    return 0;
}

/******************************************************** 
Function: SeqQueue_Clear    
Description: 清除队列
Input:  pstQueue 队列指针
OutPut: none
Return:  0 成功；非0 失败
Others:
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
int SeqQueue_Clear(SEQ_QUEUE_S* pstQueue)
{
    SEQ_QUEUE_S* l_pstSQueue = (SEQ_QUEUE_S*)pstQueue;
    
    if( l_pstSQueue != NULL )
    {
        l_pstSQueue->s32Length = 0;
        l_pstSQueue->s32Front = 0;
        l_pstSQueue->s32Rear = 0;
    }

    return 0;
}

/******************************************************** 
Function: SeqQueue_Append    
Description: 进队列
Input:  
    *pstQueue 队列指针
    *pstItem  需要存储的数据
OutPut: none
Return: 队列指针
Others: 
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
int SeqQueue_Append(SEQ_QUEUE_S* pstQueue, SEQ_QUEUE_DATA_S* pstItem) 
{
    SEQ_QUEUE_S* l_pstSQueue = NULL;
    int l_s32Ret = 0;

    if((NULL==pstQueue)||(NULL==pstItem))
    {
        printf("%s %d input data error \n",__FUNCTION__,__LINE__);
        return -1;
    };

    l_pstSQueue = pstQueue;
       
    if((l_pstSQueue->s32Length + 1)<=l_pstSQueue->s32Capacity)
    {
        //l_pstSQueue->stQueueData[l_pstSQueue->s32Rear] = (SEQ_QUEUE_DATA_S)pstItem;
        memcpy((char*)&l_pstSQueue->stQueueData[l_pstSQueue->s32Rear],(char*)pstItem,sizeof(SEQ_QUEUE_DATA_S));

        /**循环使用队列空间**/
        l_pstSQueue->s32Rear = (l_pstSQueue->s32Rear + 1) % l_pstSQueue->s32Capacity;
        
        l_pstSQueue->s32Length++;
    }
    
    return l_s32Ret;
}


/******************************************************** 
Function: SeqQueue_Retrieve    
Description: 出队列
Input:  
    *pstQueue 队列指针
OutPut: none
Return: 
    SEQ_QUEUE_DATA_S * 存储的数据结构指针
Others: 
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
SEQ_QUEUE_DATA_S* SeqQueue_Retrieve(SEQ_QUEUE_S* pstQueue)
{
    SEQ_QUEUE_S* l_pstSQueue = NULL;
    SEQ_QUEUE_DATA_S* l_pstRet = NULL;

    if(NULL==pstQueue)
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return NULL;
    }

    l_pstSQueue = pstQueue;
    l_pstRet = SeqQueue_Header(pstQueue);
    
    if(l_pstRet!=NULL)
    {
        /**循环使用队列的空间**/
        l_pstSQueue->s32Front = (l_pstSQueue->s32Front + 1) % l_pstSQueue->s32Capacity;
        l_pstSQueue->s32Length--;
    }
    
    return l_pstRet;
}

/******************************************************** 
Function: SeqQueue_Header    
Description: 取队列的头数据
Input:  
    *pstQueue 队列指针
OutPut: none
Return: 
    SEQ_QUEUE_DATA_S * 存储的数据结构指针
Others: 
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
SEQ_QUEUE_DATA_S* SeqQueue_Header(SEQ_QUEUE_S* pstQueue)
{
    SEQ_QUEUE_S* l_pstSQueue = (SEQ_QUEUE_S*)pstQueue;
    SEQ_QUEUE_DATA_S* l_pstRet = NULL;
    
    if( (l_pstSQueue != NULL) && (l_pstSQueue->s32Length > 0) )
    {
        l_pstRet = (SEQ_QUEUE_DATA_S*)&(l_pstSQueue->stQueueData[l_pstSQueue->s32Front]);
    }
    
    return l_pstRet;
}

/******************************************************** 
Function: SeqQueue_Length    
Description: 队列长度的获取
Input:  
    *pstQueue 队列指针
OutPut: none
Return: 
Others: 
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
int SeqQueue_Length(SEQ_QUEUE_S* pstQueue) 
{
    SEQ_QUEUE_S* l_pstSQueue = NULL;
    int l_s32Ret = -1;

    l_pstSQueue = pstQueue;

    if(NULL!=l_pstSQueue)
    {
        l_s32Ret = l_pstSQueue->s32Length;
    }
    
    return l_s32Ret;
}

/******************************************************** 
Function: SeqQueue_Length    
Description: 获取队列的容量
Input:  
    *pstQueue 队列指针
OutPut: none
Return: 
    
Others: 
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
int SeqQueue_Capacity(SEQ_QUEUE_S* pstQueue) 
{
    SEQ_QUEUE_S* l_pstSQueue = NULL;
    int l_s32Ret = -1;

    l_pstSQueue = pstQueue;

    if(NULL!=l_pstSQueue)
    {
        l_s32Ret = l_pstSQueue->s32Capacity;
    }
    
    return l_s32Ret;
}
