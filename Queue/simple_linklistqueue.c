/*=============================================================================    
 *FileName: simple_linklistqueue.c
 *Desc: 最简单队列实现
 *Author: Caibiao Lee    
 * LastChange: 2019.01.01 
 * ============================================================================*/ 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
 
//存储数据
typedef struct        
{
    unsigned char u8Channel;                     
    unsigned char u8Event;                            
    unsigned char u8Operate;                     
    unsigned char arru8BCDTime[6];         
    char arru8FileName[128];        
}__attribute__ ((packed))LinkData;
 
//链队列结点结构
typedef struct QNode
{
	LinkData data;
	struct QNode *next;
}QNode;
 
//链队列结构
typedef struct LiQueue
{
	QNode *front;
	QNode *rear;
}LiQueue;
 
 
//创建链队列
LiQueue *initQueue(void)
{
	LiQueue *lq=(LiQueue *)malloc(sizeof(LiQueue));
	if(lq ==NULL)
	{
		return lq;
	}
	else
	{
		lq->front=lq->rear=NULL;
	}
 
	return lq;
}
 
//判断链队列是否为空,0为空
int isEmpty(LiQueue *lq)
{
	if(lq->front==NULL || lq->rear==NULL)
	{
		return 0;
	}else
	{
		return 1;
	}
}
 
//元素进链队列
int InputQueue(LiQueue *lq, LinkData x)
{
	QNode *p;
	p=(QNode *)malloc(sizeof(QNode));
 
	memcpy(&p->data,&x,sizeof(LinkData));
	p->next=NULL;
	if(lq->rear==NULL)
	{
		lq->front=lq->rear=p;//如果第一次插入则设置头指针和尾指针为p
	}else
	{
		lq->rear->next=p;//链队列的尾部插入p
		lq->rear=p;//设置链队列的尾指针指向p
	}
	return 0;
}
 
//元素出链队列
int OutputQueue(LiQueue *lq,LinkData *y)
{
	QNode *p=lq->front;
	if(lq->rear==NULL || lq->front==NULL)
	{
		printf("%s %d error\n",__FUNCTION__,__LINE__);
		return 0;
	}
	if(lq->front==lq->rear)
	{
		lq->front=lq->rear=NULL;
	}else
	{
		lq->front=lq->front->next;
	}
	*y=p->data;
	memcpy((unsigned char*)y,&p->data,sizeof(LinkData));
	free(p);
	return 1;
}
 
 
//打印链队列
void printQueue(LiQueue *lq)
{
	LiQueue p ;
 
	p.front = lq->front;
 
	if(lq->front==NULL || lq->rear==NULL)
	{
		return;
	}
	while(lq->front!=NULL)
	{
		printf("%d\n",lq->front->data.u8Channel);
		lq->front=lq->front->next;
	}
 
	lq->front = p.front;
}
 
int main(void)
{
	LinkData test;	
	LiQueue* lq=initQueue();
	for(int i=0;i<10;i++)
	{
		test.u8Channel = i;
		InputQueue(lq,test);
	}
	printQueue(lq);
	printf(".......................\n");
	OutputQueue(lq,&test);
	printf("Out Put u8Channel = %d \n",test.u8Channel);
	printf(".......................\n");
	printQueue(lq);
 
	return 0;
}
 