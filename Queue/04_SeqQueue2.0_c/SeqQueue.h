/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: SeqQueue.h
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 
*Date:     2020-01-01
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#ifndef _SEQQUEUE_H_
#define _SEQQUEUE_H_

/**队列数据**/
typedef struct
{
    int s32Data1;
    int s32Data2;
}SEQ_QUEUE_DATA_S;

/**队列节点**/
typedef struct _tag_SeqQueue
{
    int s32Capacity;    /**队列容量**/
    int s32Length;      /**队列长度**/
    int s32Front;       /**队列头**/
    int s32Rear;        /**队列尾**/
    SEQ_QUEUE_DATA_S *stQueueData;/**队列数据**/
}SEQ_QUEUE_S;


SEQ_QUEUE_S* SeqQueue_Create(int s32Capacity);
int SeqQueue_Destroy(SEQ_QUEUE_S* pstQueue);
int SeqQueue_Clear(SEQ_QUEUE_S* pstQueue);
int SeqQueue_Append(SEQ_QUEUE_S* pstQueue, SEQ_QUEUE_DATA_S* pstItem);
SEQ_QUEUE_DATA_S* SeqQueue_Retrieve(SEQ_QUEUE_S* pstQueue);
SEQ_QUEUE_DATA_S* SeqQueue_Header(SEQ_QUEUE_S* pstQueue);
int SeqQueue_Length(SEQ_QUEUE_S* pstQueue);
int SeqQueue_Capacity(SEQ_QUEUE_S* pstQueue);

#endif
