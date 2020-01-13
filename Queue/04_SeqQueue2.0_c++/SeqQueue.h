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
#include <pthread.h>

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

class SeqQueue
{
public:
    SeqQueue();
    ~SeqQueue();
    int SeqQueue_Create(int s32Capacity);
    int SeqQueue_Destroy(void);
    int SeqQueue_Clear(void);
    int SeqQueue_InputNode(SEQ_QUEUE_DATA_S pstItem);
    int SeqQueue_OutPutNode(SEQ_QUEUE_DATA_S* pstQueue);
    int SeqQueue_GetLength(void);
    int SeqQueue_GetCapacity(void);
private:
    int SeqQueue_Header(SEQ_QUEUE_DATA_S *pstNodeData);
    int SeqQueue_InitMutex(void);
    int SeqQueue_ReleaseMutex(void);
    int SeqQueue_Lock(void);
    int SeqQueue_UnLock(void);
private:
    SEQ_QUEUE_S *m_pstSeqQueue;    
    pthread_mutex_t m_stSeqQueueMutex;
};




#endif

