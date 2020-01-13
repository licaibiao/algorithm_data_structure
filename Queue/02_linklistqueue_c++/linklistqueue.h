/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: linklistqueue.h
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 用链表实现一个队列的功能
*Date:	   2019-09-28
*Author:   Caibiao Lee
*Version:  V1.1
*Others:
*History:
***********************************************************/
#ifndef __LINK_LIST_QUEUE_H__
#define __LINK_LIST_QUEUE_H__
 
 
/**存储数据**/
typedef struct link_data  
{
    unsigned int  u32Data1;        
    unsigned char u8Data2;   
}__attribute__ ((packed))LINK_DATA_S;
 
/**链队列结点结构**/
typedef struct queue_node
{
    LINK_DATA_S stData;           /**数据节点**/
    struct QUEUE_NODE_S *stNext;  /**下一个数据节点**/
}QUEUE_NODE_S;
 
/**链队列结构**/
typedef struct list_queue
{
    int s32Length;              /**线性表的实际使用长度**/
    int s32Capacity;            /**该线性表的最大长度**/
    QUEUE_NODE_S *stFront;      /**链表头**/ 
    QUEUE_NODE_S *stRear;       /**链表尾**/
}LIST_QUEUE_S;
 
class LinkListQueue
{
public:
    LinkListQueue();
    ~LinkListQueue();
    int LinkListQueue_Create(int s32Capacity);
    int LinkListQueue_Input(LINK_DATA_S stLinkData);
    int LinkListQueue_Output(LINK_DATA_S *pstLinkData);
    int LinkListQueue_Destroy();
private:
    LIST_QUEUE_S *LinkListQueue_GetNextNodeAddr();
    int LinkListQueue_InitMutex(void);
    int LinkListQueue_ReleaseMutex(void);
    int LinkListQueue_Lock(void);
    int LinkListQueue_Unlock(void);

private:
    unsigned int m_u32NodeLen;
    LIST_QUEUE_S *m_stListQueue;
    pthread_mutex_t m_stLinkListMutex;  
};


#endif

