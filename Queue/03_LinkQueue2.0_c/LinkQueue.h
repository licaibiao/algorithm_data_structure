/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: LinkQueue.h
*BlogAddr: https://blog.csdn.net/li_wen01
*Description:链式存储方式实现队列的功能 
*Date:     2020-01-01
*Author:   Caibiao Lee
*Version:  V1.0
*Others:   
*History:
***********************************************************/
#ifndef _LINKQUEUE_H_
#define _LINKQUEUE_H_

typedef struct
{
    int s32Data1;
    int s32Data2;
}LINK_QUEUE_DATA_S;

typedef struct _tag_LinkQueueNode LINK_QUEUE_NODE_S;
struct _tag_LinkQueueNode
{
    LINK_QUEUE_NODE_S* pstNext;
    LINK_QUEUE_DATA_S stData;
};

typedef struct _tag_LinkQueue
{
    LINK_QUEUE_NODE_S* pstFront;
    LINK_QUEUE_NODE_S* pstRear;
    int s32Len;
} LINK_QUEUE_S;


LINK_QUEUE_S* LinkQueue_Create() ;

int LinkQueue_Destroy(LINK_QUEUE_S* pstQueue); 

int LinkQueue_Clear(LINK_QUEUE_S* pstQueue); 

int LinkQueue_Append(LINK_QUEUE_S* pstQueue, LINK_QUEUE_DATA_S* pstItem); 

int LinkQueue_Retrieve(LINK_QUEUE_S* pstQueue,LINK_QUEUE_DATA_S* pData); 

int LinkQueue_Header(LINK_QUEUE_S* pstQueue,LINK_QUEUE_DATA_S* pstData);

int LinkQueue_Length(LINK_QUEUE_S* pstQueue); 


#endif

