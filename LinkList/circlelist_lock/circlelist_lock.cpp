/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: circlelist_lock.cpp
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 
*Date:     2020-01-01
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "circlelist_lock.h"

int CircleList::CircleList_InitMutex(void)
{
    pthread_mutex_init(&m_stCircleMutex,NULL);
};
int CircleList::CircleList_ReleaseMutex(void)
{
    pthread_mutex_destroy(&m_stCircleMutex);
};
int CircleList::CircleList_Lock(void)
{   
    pthread_mutex_lock(&m_stCircleMutex);
}
int CircleList::CircleList_Unlock(void)
{
    pthread_mutex_unlock(&m_stCircleMutex);
}


CircleList::CircleList()
{
    m_stCircleList = NULL;
    CircleList_InitMutex();
    CircleList_Create();  
}

CircleList::~CircleList()
{
    CircleList_Destroy();
    CircleList_ReleaseMutex();
}
    
CircleList* CircleList::Get_CircleListInstall()
{

    static CircleList * pClsCircleList = NULL;

    if (pClsCircleList == NULL)
    {
        pClsCircleList = new CircleList();
    }

    return pClsCircleList;
}

int CircleList::CircleList_Create(void)
{
    if(NULL==m_stCircleList)
    {
        m_stCircleList = (CIRCLELIST_S*)malloc(sizeof(CIRCLELIST_S));
        if(m_stCircleList!=NULL)
        {
            m_stCircleList->u32Length = 0;
            m_stCircleList->stHeader.next = NULL;
            m_stCircleList->p_stSlider = NULL;
        }else
        {
            printf("%s %d malloc error \n",__FUNCTION__,__LINE__);
            return -1;
        }
    }

    return 0;
};

int CircleList::CircleList_Destroy(void)
{
    int l_s32Len = 0;
    int l_s32Ret = 0;
    
    if(m_stCircleList!=NULL)
    {
        l_s32Len = m_stCircleList->u32Length;
        for(int i=l_s32Len;i>0;i--)
        {
            if(0==CircleList_DeleteNode(l_s32Len))
            {
                printf("delete pos %d OK \n",i);
            }else
            {
                printf("delete pos %d error \n",i);
            }
        }
    }

    if(NULL!=m_stCircleList)
    {
        free(m_stCircleList);
        m_stCircleList = NULL;
    }

    return 0;
};

int CircleList::CircleList_GetLength(void)
{
    int s32Ret = -1;
    
    if(m_stCircleList != NULL)
    {
        s32Ret = m_stCircleList->u32Length;
    }
     
    return s32Ret;
};

int CircleList::CircleList_InsertNode(LIST_NODE_S stNode, int s32Pos)
{
    LIST_NODE_S *l_pstInsetNode = NULL;
    LIST_NODE_S *l_pstCurrent = NULL;
    
    if((NULL==m_stCircleList)||(s32Pos<0))
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    }

    l_pstInsetNode = (LIST_NODE_S*)malloc(sizeof(LIST_NODE_S));
    if(NULL==l_pstInsetNode)
    {
        printf("%s %d malloc error \n",__FUNCTION__,__LINE__);
        return -2;
    }

    memcpy(l_pstInsetNode,&stNode,sizeof(LIST_NODE_S)); 

    CircleList_Lock();
    
    l_pstCurrent =  (LIST_NODE_S *)&m_stCircleList->stHeader;   
        
    for(int i=0; (i<s32Pos)&&(l_pstCurrent->next);i++)
    {
        l_pstCurrent = l_pstCurrent->next;
    }
    
    l_pstInsetNode->next = l_pstCurrent->next;
    l_pstCurrent->next = l_pstInsetNode;
    
    if( m_stCircleList->u32Length == 0 )
    {
        m_stCircleList->p_stSlider = l_pstInsetNode;
        l_pstInsetNode->next = l_pstInsetNode;
    }
  
    m_stCircleList->u32Length++;

    CircleList_Unlock();
    
    return 0;
};

LIST_NODE_S* CircleList::CircleList_GetNode(int s32Pos)
{
    LIST_NODE_S* l_pstRet = NULL;
    LIST_NODE_S* l_pstCurrent = NULL;

    if((m_stCircleList!=NULL)&&(s32Pos>=0))
    {
       CircleList_Lock();
       l_pstCurrent = (LIST_NODE_S*)&m_stCircleList->stHeader;
        
        for(int i=0; i<s32Pos; i++)
        {
            l_pstCurrent = l_pstCurrent->next;
        }
        
        l_pstRet = l_pstCurrent->next;
        CircleList_Unlock();
    }else
    {
        printf("%s %d input para error ! \n",__FUNCTION__,__LINE__);
    }
    
    return l_pstRet;

}

int CircleList::CircleList_DeleteNode(int s32Pos)
{
    int i = 0;
    LIST_NODE_S* l_pstCurrent = NULL;
    LIST_NODE_S* l_pstFirst   = NULL;
    LIST_NODE_S* l_pstLast    = NULL;
    LIST_NODE_S* l_pstNext    = NULL;

    if((NULL==m_stCircleList)||(s32Pos<0))
    {
        printf("%s %d input para error \n",__FUNCTION__,__LINE__);
        return -1;
    }

    CircleList_Lock();
    
    l_pstCurrent = m_stCircleList->p_stSlider;
    l_pstFirst = m_stCircleList->stHeader.next;
    l_pstLast =  CircleList_GetNode(m_stCircleList->u32Length - 1);
    
    for(i=0;i<s32Pos;i++)
    {
        l_pstCurrent = l_pstCurrent->next;
    }
    
    l_pstNext = l_pstCurrent->next;
    l_pstCurrent->next = l_pstNext->next;
    
    m_stCircleList->u32Length--;
    
    if(l_pstFirst == l_pstNext)
    {
        m_stCircleList->stHeader.next = l_pstNext->next;
        l_pstLast->next = l_pstNext->next;
    }
    
    if(m_stCircleList->p_stSlider==l_pstNext)
    {
        m_stCircleList->p_stSlider = l_pstNext->next;
    }

    free(l_pstNext);
    l_pstNext = NULL;

    printf("delete node %d OK \n",s32Pos);
    
    if(0==m_stCircleList->u32Length)
    {
        m_stCircleList->stHeader.next = NULL;
        m_stCircleList->p_stSlider = NULL;
    }

    CircleList_Unlock();
    

    return 0;

}


LIST_NODE_S* CircleList::CircleList_GetCurrent(void)
{

    LIST_NODE_S *l_stpRet = NULL;

    if( m_stCircleList!=NULL)
    {
        l_stpRet = m_stCircleList->p_stSlider;
    }
    
    return l_stpRet;
}

int CircleList::CircleList_Printf(void)
{
    int l_s32Len = 0;
    LIST_NODE_S* l_pstCurrent = NULL;
    
    l_s32Len = m_stCircleList->u32Length ;

    l_pstCurrent = m_stCircleList->stHeader.next;

    for(int i=0;i<l_s32Len;i++)
    {
        if(NULL!=l_pstCurrent)
        {
           printf("Node %d DataLen=%d \n",i,l_pstCurrent->stData.u32Len); 
        }

        l_pstCurrent = l_pstCurrent->next;
    }

    return 0;
}


