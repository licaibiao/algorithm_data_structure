# 链表高级使用
## 1.使用分层的概念
&emsp;使用分层的改变，对于链表的基础操作，稳定之后，基础操作将不再需要修改。
&emsp;使用钩子函数，钩子函数在应用层实现，可以根据不同的应用实现不同的操作

## 2.迭代器
&emsp;为了简化部分代码，这里使用了迭代器，实际上也就是一个游标节点，用户可以根据不同的业务需求，实现从链表头或是链表尾开始的游标检索功能。

## 3.多链表的使用
&emsp; 每一条链表有独立的锁，list.c中的函数都是可重入的，所以支持多多链表的使用


