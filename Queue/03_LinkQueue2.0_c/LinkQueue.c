/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: LinkQueue.c
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 链式存储方式实现队列的功能
*Date:     2020-01-01
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkQueue.h"

LINK_QUEUE_S* LinkQueue_Create() 
{
    LINK_QUEUE_S* l_pstRet = NULL;

    l_pstRet = (LINK_QUEUE_S*)malloc(sizeof(LINK_QUEUE_S));
    if(NULL!=l_pstRet)
    {
        l_pstRet->pstFront = NULL;
        l_pstRet->pstRear = NULL;
        l_pstRet->s32Len = 0;
    }

    return l_pstRet;
}

int LinkQueue_Destroy(LINK_QUEUE_S* pstQueue) 
{
    if(NULL==pstQueue)
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    }
    
    LinkQueue_Clear(pstQueue);
    free(pstQueue);
    pstQueue = NULL;
}

int LinkQueue_Clear(LINK_QUEUE_S* pstQueue) 
{
    LINK_QUEUE_DATA_S l_stData = {0};
    
    if(NULL==pstQueue)
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    }

    while(LinkQueue_Length(pstQueue) > 0 )
    {
        LinkQueue_Retrieve(pstQueue,&l_stData);
    }

    return 0;
}

int LinkQueue_Append(LINK_QUEUE_S* pstQueue, LINK_QUEUE_DATA_S* pstItem) 
{
    LINK_QUEUE_S* l_pstLinkQueue = NULL;
    LINK_QUEUE_NODE_S* l_pstNode = NULL;

    if((NULL==pstQueue)||(NULL==pstItem))
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    }

    l_pstLinkQueue = pstQueue;
    l_pstNode = (LINK_QUEUE_NODE_S*)malloc(sizeof(LINK_QUEUE_NODE_S));
    if(NULL==l_pstNode)
    {
        printf("%s %d malloc error \n",__FUNCTION__,__LINE__);
        return -2;
    }
    
    memcpy(&l_pstNode->stData,pstItem,sizeof(LINK_QUEUE_DATA_S));
          
    if( l_pstLinkQueue->s32Len > 0 )
    {
        l_pstLinkQueue->pstRear->pstNext = l_pstNode;
        l_pstLinkQueue->pstRear = l_pstNode;
        l_pstNode->pstNext = NULL;
    }
    else
    {
        l_pstLinkQueue->pstFront = l_pstNode;
        l_pstLinkQueue->pstRear = l_pstNode;
        l_pstNode->pstNext = NULL;
    }
    
    l_pstLinkQueue->s32Len++;
    
    return 0;
}

int LinkQueue_Retrieve(LINK_QUEUE_S* pstQueue,LINK_QUEUE_DATA_S* pData) 
{
    LINK_QUEUE_S* l_pstLinkQueue = NULL;
    LINK_QUEUE_NODE_S* l_pstNode = NULL;
    
    if((NULL==pstQueue)||(NULL==pData))
    {
        printf("%s %d input data error \n",__FUNCTION__,__LINE__);
        return -1;
    }

    l_pstLinkQueue = pstQueue;
    
    
    if( (l_pstLinkQueue != NULL) && (l_pstLinkQueue->s32Len > 0) )
    {
        l_pstNode = l_pstLinkQueue->pstFront;
        
        l_pstLinkQueue->pstFront = l_pstNode->pstNext;
        
        //ret = l_pstNode->stData;
        memcpy((char *)pData,&l_pstNode->stData,sizeof(LINK_QUEUE_DATA_S));
        
        free(l_pstNode);
        
        l_pstLinkQueue->s32Len--;
        
        if(0==l_pstLinkQueue->s32Len)
        {
            l_pstLinkQueue->pstFront = NULL;
            l_pstLinkQueue->pstRear = NULL;
        }
    }else
    {
        return -2;
    }
    
    return 0;
}

int LinkQueue_Header(LINK_QUEUE_S* pstQueue,LINK_QUEUE_DATA_S* pstData)
{
    LINK_QUEUE_S* l_pstLinkQueue = (LINK_QUEUE_S*)pstQueue;

    if((NULL==pstQueue)||(NULL==pstData))
    {
        printf("%s %d input data error \n",__FUNCTION__,__LINE__);
        return -1;        
    }
    
    if((l_pstLinkQueue!=NULL)&&(l_pstLinkQueue->s32Len>0))
    {
        memcpy(pstData,&l_pstLinkQueue->pstFront->stData,sizeof(LINK_QUEUE_DATA_S));
    }else
    {
        return -2;
    }
    
    return 0;
}

int LinkQueue_Length(LINK_QUEUE_S* pstQueue) 
{
    int l_s32Len = 0;

    if(NULL==pstQueue)
    {
        printf("%s %d input data error \n",__FUNCTION__,__LINE__);
        return -1;        
    }

    if( pstQueue != NULL )
    {
        l_s32Len = pstQueue->s32Len;
    }
    
    return l_s32Len;
}
