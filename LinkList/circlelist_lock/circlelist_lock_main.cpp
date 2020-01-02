/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: circlelist_lock_main.cpp
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 
*Date:     2020-01-01
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "circlelist_lock.h"

int main(int argc,char **argv)
{
    int len = 0;
    LIST_NODE_S l_stNode = {0};
    CircleList * l_pstCircleList = NULL;

    l_pstCircleList = CircleList::Get_CircleListInstall();

    /**压力测试**/
    for(int j=0;j<100;j++)
    {
        len = l_pstCircleList->CircleList_GetLength();
        
        printf("len = %d \n",len);
        
        for(int i=len;i<2048;i++)
        {
            l_stNode.stData.u32Len = i;
            l_pstCircleList->CircleList_InsertNode(l_stNode,i);
        }
        
        l_pstCircleList->CircleList_Printf();
        
        len = l_pstCircleList->CircleList_GetLength();
        for(int i=len;i>0;i--)
        {
            l_pstCircleList->CircleList_DeleteNode(i);
        }

    }

    return 0;
}


