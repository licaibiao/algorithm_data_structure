/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: circlelist.h
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 
*Date:	   2020-01-01
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#ifndef _CIRCLE_LIST_H_
#define _CIRCLE_LIST_H_

typedef void CircleList;
typedef struct _tag_CircleListNode CircleListNode;
struct _tag_CircleListNode
{
    CircleListNode* next;
};

CircleList* CircleList_Create();

void CircleList_Destroy(CircleList* list);

void CircleList_Clear(CircleList* list);

int CircleList_Length(CircleList* list);

int CircleList_Insert(CircleList* list, CircleListNode* node, int pos);

CircleListNode* CircleList_Get(CircleList* list, int pos);

CircleListNode* CircleList_Delete(CircleList* list, int pos);

CircleListNode* CircleList_DeleteNode(CircleList* list, CircleListNode* node);

CircleListNode* CircleList_Reset(CircleList* list);

CircleListNode* CircleList_Current(CircleList* list);

CircleListNode* CircleList_Next(CircleList* list);

#endif
