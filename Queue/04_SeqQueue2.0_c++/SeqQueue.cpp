/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: SeqQueue.cpp
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


int SeqQueue::SeqQueue_InitMutex(void)
{   
    pthread_mutex_init(&m_stSeqQueueMutex,NULL);
 
    return 0;
}
 
int SeqQueue::SeqQueue_ReleaseMutex(void)
{ 
    pthread_mutex_destroy(&m_stSeqQueueMutex);
 
    return 0;
}
 
int SeqQueue::SeqQueue_Lock(void)
{
    pthread_mutex_lock(&m_stSeqQueueMutex);
 
    return 0;
}
 
int SeqQueue::SeqQueue_UnLock(void)
{
 
    pthread_mutex_unlock(&m_stSeqQueueMutex);
 
    return 0;
}

SeqQueue::SeqQueue()
{
    m_pstSeqQueue = NULL;
    SeqQueue_InitMutex();
}
SeqQueue::~SeqQueue()
{
   SeqQueue_Destroy(); 
   SeqQueue_ReleaseMutex();
};


/******************************************************** 
Function: SeqQueue_Create    
Description: 创建队列
Input:  s32Capacity： 队列元素的最大个数
OutPut: none
Return: 0 成功；非0 失败
Others: 
Author: Caibiao Lee
Date:    2020-01-12
*********************************************************/
int SeqQueue::SeqQueue_Create(int s32Capacity)
{
    SEQ_QUEUE_S* l_pstRet = NULL;
    
    if( s32Capacity >= 0 )
    {
        l_pstRet = (SEQ_QUEUE_S*)malloc(sizeof(SEQ_QUEUE_S) + sizeof(SEQ_QUEUE_DATA_S) * s32Capacity);
    }
    
    if(l_pstRet!=NULL)
    {
        l_pstRet->s32Capacity = s32Capacity;
        l_pstRet->s32Length = 0;
        l_pstRet->s32Front = 0;
        l_pstRet->s32Rear = 0;
        l_pstRet->stQueueData = (SEQ_QUEUE_DATA_S*)(l_pstRet + 1);
    }else
    {
        printf("%s %d malloc error \n",__FUNCTION__,__LINE__);
        return -1;
    }

    SeqQueue_Lock();
    m_pstSeqQueue = l_pstRet;
    SeqQueue_UnLock();
    
    return 0;

};

/******************************************************** 
Function: SeqQueue_Destroy    
Description: 销毁队列
Input:  none
OutPut: none
Return: 0 成功；非0 失败
Others: 
Author: Caibiao Lee
Date:    2020-01-12
*********************************************************/
int SeqQueue::SeqQueue_Destroy(void)
{
    SeqQueue_Lock();

    if(NULL!=m_pstSeqQueue)
    {  
        free(m_pstSeqQueue);
        m_pstSeqQueue = NULL;   
    }

    SeqQueue_UnLock();

    return 0;
};

/******************************************************** 
Function: SeqQueue_Clear    
Description: 清除队列
Input:  none
OutPut: none
Return:  0 成功；非0 失败
Others:
Author: Caibiao Lee
Date:    2020-01-12
*********************************************************/
int SeqQueue::SeqQueue_Clear(void)
{  
    SeqQueue_Lock();
    if(m_pstSeqQueue!= NULL)
    {
        m_pstSeqQueue->s32Length = 0;
        m_pstSeqQueue->s32Front = 0;
        m_pstSeqQueue->s32Rear = 0;
    }

    SeqQueue_UnLock();
    
    return 0;
};

/******************************************************** 
Function: SeqQueue_InputNode    
Description: 进队列
Input:  
    stItem  需要存储的数据
OutPut: none
Return:  0 成功；非0 失败
Others: 
Author: Caibiao Lee
Date:    2020-01-12
*********************************************************/
int SeqQueue::SeqQueue_InputNode(SEQ_QUEUE_DATA_S stItem)
{
    int l_s32Ret = 0;

    if(NULL==m_pstSeqQueue)
    {
        printf("%s %d input data error \n",__FUNCTION__,__LINE__);
        return -1;
    };

    SeqQueue_Lock();     
    if((m_pstSeqQueue->s32Length + 1)<=m_pstSeqQueue->s32Capacity)
    {
        memcpy((char*)&m_pstSeqQueue->stQueueData[m_pstSeqQueue->s32Rear],(char*)&stItem,sizeof(SEQ_QUEUE_DATA_S));

        //printf("input data1=%d \n",m_pstSeqQueue->stQueueData[m_pstSeqQueue->s32Rear].s32Data1);

        /**循环使用队列空间**/
        m_pstSeqQueue->s32Rear = (m_pstSeqQueue->s32Rear + 1) % m_pstSeqQueue->s32Capacity;        
        m_pstSeqQueue->s32Length++;
    }else
    {
        printf("%s %d Queue is Full !! \n",__FUNCTION__,__LINE__);
        SeqQueue_UnLock();
        return -2;
    }

    SeqQueue_UnLock();
    return 0;
};

/******************************************************** 
Function: SeqQueue_OutPutNode    
Description: 出队列
Input:  
    *pstQueue 队列指针
OutPut: none
Return: 0 成功；非0 失败  
Others: 
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
int SeqQueue::SeqQueue_OutPutNode(SEQ_QUEUE_DATA_S* pstQueueData)
{
    int l_s32Ret = 0;
    SEQ_QUEUE_DATA_S l_stOutData = {0};

    if((NULL==m_pstSeqQueue)||(NULL==pstQueueData))
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    }

    l_s32Ret = SeqQueue_Header(&l_stOutData);
    if(0==l_s32Ret)
    {
        SeqQueue_Lock();
        /**循环使用队列的空间**/
        m_pstSeqQueue->s32Front = (m_pstSeqQueue->s32Front + 1) % m_pstSeqQueue->s32Capacity;
        m_pstSeqQueue->s32Length--;
        SeqQueue_UnLock();  
    }else
    {
        printf("%s %d get node error \n",__FUNCTION__,__LINE__);
        return -2;
    }

    memcpy((char*)pstQueueData,(char*)&l_stOutData,sizeof(SEQ_QUEUE_DATA_S));
    
    return 0;
};


int SeqQueue::SeqQueue_GetLength(void)
{
    int l_s32Ret = -1;

    if(NULL!=m_pstSeqQueue)
    {
        l_s32Ret = m_pstSeqQueue->s32Length;
    }
    
    return l_s32Ret;
};
int SeqQueue::SeqQueue_GetCapacity(void)
{
    int l_s32Ret = -1;

    if(NULL!=m_pstSeqQueue)
    {
        l_s32Ret = m_pstSeqQueue->s32Capacity;
    }
    
    return l_s32Ret;    
};


/******************************************************** 
Function: SeqQueue_Header    
Description: 取队列的头数据
Input:  
    *pstNodeData 数据指针
OutPut: none
Return: 0 成功；非0 失败  
    
Others: 
Author: Caibiao Lee
Date:    2019-09-28
*********************************************************/
int SeqQueue::SeqQueue_Header(SEQ_QUEUE_DATA_S *pstNodeData)
{
    SEQ_QUEUE_DATA_S* l_pstRet = NULL;

    if(NULL==pstNodeData)
    {
        printf("%s %d input para error\n",__FUNCTION__,__LINE__);
        return -1;
    }

    SeqQueue_Lock();
    if((m_pstSeqQueue != NULL) && (m_pstSeqQueue->s32Length > 0))
    {
        l_pstRet = (SEQ_QUEUE_DATA_S*)&(m_pstSeqQueue->stQueueData[m_pstSeqQueue->s32Front]);
        memcpy((char*)pstNodeData,(char*)l_pstRet,sizeof(SEQ_QUEUE_DATA_S));
        SeqQueue_UnLock();
        return 0;
    }else
    {
        return -2;
    }
    SeqQueue_UnLock();
    return 0;
}


