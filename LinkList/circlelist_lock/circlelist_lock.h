/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: circlelist_lock.h
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 
*Date:     2020-01-01
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#ifndef _CIRCLE_LIST_LOCK_H_
#define _CIRCLE_LIST_LOCK_H_

#include <pthread.h>

typedef struct   
{
    unsigned int   u32Len;        /**数据长度**/
    unsigned char *pu8DataAddr;   /**数据存放地址**/
}LIST_DATA_S;

typedef struct _tag_List_Node
{
    _tag_List_Node* next;
    LIST_DATA_S stData;
}LIST_NODE_S;

typedef struct 
{
    LIST_NODE_S stHeader;
    LIST_NODE_S* p_stSlider;
    unsigned int u32Length;
}CIRCLELIST_S;

class CircleList
{
public:
    CircleList();
    ~CircleList();
    static CircleList* Get_CircleListInstall(); 
public:
    int CircleList_GetLength(void);
    int CircleList_DeleteNode(int s32Pos);
    int CircleList_InsertNode(LIST_NODE_S stNode, int s32Pos);
    LIST_NODE_S* CircleList_GetNode(int s32Pos);
    LIST_NODE_S* CircleList_GetCurrent(void);
    int CircleList_Printf(void);
private:
    int CircleList_Create(void);
    int CircleList_Destroy(void);
    
private:
    int CircleList_InitMutex(void);
    int CircleList_ReleaseMutex(void);
    int CircleList_Lock(void);
    int CircleList_Unlock(void);

private:
   pthread_mutex_t m_stCircleMutex;
   CIRCLELIST_S *m_stCircleList; 
};

#endif

