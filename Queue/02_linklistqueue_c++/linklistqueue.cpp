/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: LinkListQueue.c
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 用链表实现一个队列的功能
*Date:       2019-06-22
*Author:   Caibiao Lee
*Version:  V1.1
*Others:
*History:
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "linklistqueue.h" 

int LinkListQueue::LinkListQueue_InitMutex(void)
{   
    pthread_mutex_init(&m_stLinkListMutex,NULL);
 
    return 0;
}
 
int LinkListQueue::LinkListQueue_ReleaseMutex(void)
{ 
    pthread_mutex_destroy(&m_stLinkListMutex);
 
    return 0;
}
 
int LinkListQueue::LinkListQueue_Lock(void)
{
    pthread_mutex_lock(&m_stLinkListMutex);
 
    return 0;
}
 
int LinkListQueue::LinkListQueue_Unlock(void)
{
 
    pthread_mutex_unlock(&m_stLinkListMutex);
 
    return 0;
}


LinkListQueue::LinkListQueue()
{
    m_u32NodeLen = sizeof(LIST_QUEUE_S) + sizeof(QUEUE_NODE_S);
    LinkListQueue_InitMutex();
}

LinkListQueue::~LinkListQueue()
{
    LinkListQueue_ReleaseMutex();
}


int LinkListQueue::LinkListQueue_Create(int s32Capacity)
{
    if((s32Capacity<=0)||(m_u32NodeLen==0))
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    }

    LinkListQueue_Lock();

     m_stListQueue = (LIST_QUEUE_S *)calloc((s32Capacity + 1), sizeof(LIST_QUEUE_S)+sizeof(QUEUE_NODE_S));
    if(NULL==m_stListQueue)
    {
        printf("%s %d calloc error \n",__FUNCTION__,__LINE__);
        return -2;
    }
    
    m_stListQueue->s32Length = 0;
    m_stListQueue->s32Capacity = s32Capacity;

    LinkListQueue_Unlock();

    return 0;
}

int LinkListQueue::LinkListQueue_Input(LINK_DATA_S stLinkData)
{
    QUEUE_NODE_S *l_pstQueueNode = NULL;
    
    if(NULL==m_stListQueue)
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    }


    LinkListQueue_Lock();
    l_pstQueueNode=(QUEUE_NODE_S *)malloc(sizeof(QUEUE_NODE_S));

    memcpy(&l_pstQueueNode->stData,&stLinkData,sizeof(LINK_DATA_S));
 
    l_pstQueueNode->stNext=NULL;
    
    if(m_stListQueue->stRear==NULL)
    {    
        /**如果第一次插入则设置头指针和尾指针为l_pstQueueNode**/
        m_stListQueue->stFront=m_stListQueue->stRear=l_pstQueueNode;
    }else
    {
        /**链队列的尾部插入l_pstQueueNode**/
        m_stListQueue->stRear->stNext=l_pstQueueNode;
 
        /**设置链队列的尾指针指向l_pstQueueNode**/
        m_stListQueue->stRear=l_pstQueueNode;        
    }
    m_stListQueue->s32Length++;
    
    LinkListQueue_Unlock();

}

int LinkListQueue::LinkListQueue_Output(LINK_DATA_S *pstLinkData)
{
    QUEUE_NODE_S *l_pstQueueNode=NULL;
    
    if((NULL==m_stListQueue)||(NULL==pstLinkData))
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    };
 
    l_pstQueueNode = m_stListQueue->stFront;
    if((NULL==m_stListQueue->stRear)||(NULL==m_stListQueue->stFront))
    {
        printf("%s %d error\n",__FUNCTION__,__LINE__);
        return -2;
    }
 
    LinkListQueue_Lock(m_stListQueue->s8QueueNum);
    if(m_stListQueue->stFront==m_stListQueue->stRear)
    {
        m_stListQueue->stFront=m_stListQueue->stRear=NULL;
    }else
    {
        m_stListQueue->stFront=m_stListQueue->stFront->stNext;
    }
 
    m_stListQueue->s32Length--;
 
    LinkListQueue_Unlock(m_stListQueue->s8QueueNum);
 
    *pstLinkData=l_pstQueueNode->stData;
    memcpy((unsigned char*)pstLinkData,&l_pstQueueNode->stData,sizeof(LINK_DATA_S));
    free(l_pstQueueNode);
}

/******************************************************** 
Function: LinkListQueue_GetNextNodeAddr    
Description: 获取队列下一个节点的起始地址
Input:  none
OutPut: 下一个节点的起始地址
Return: 
Others: 0 成功；非0 失败
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
LIST_QUEUE_S *LinkListQueue::LinkListQueue_GetNextNodeAddr(void)
{
    LIST_QUEUE_S * l_pstNetNodeAddr;
    
    if(((m_stListQueue.s32Length+1)>=m_stListQueue.s32Capacity)||(NULL==m_stListQueue))
    {
        printf("%s %d Link List is Full !!!!\n",__FUNCTION__,__LINE__);
        return -1;
    };

    l_pstNetNodeAddr = m_stListQueue + (m_stListQueue.s32Length+1)*(m_u32NodeLen);
    
    return 

}



