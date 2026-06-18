// #include <iostream>
// #include <memory>
// #include <functional>
// namespace my_list
// {
//     template<class T>
//     struct ListNode
//     {
//         ListNode(T val=T())
//         :_val(val),
//         _next(nullptr)
//         {}
//         T _val;
//         std::shared_ptr<ListNode> _next;
//     };
//     template<class>
//     class ListHead
//     {
//         using node_t = std::shared_ptr<ListNode>;
//     public:
//         ListHead()
//         :_head(new ListNode)
//         {}
        
//     private:
//         node_t _head;
//     };
// }

#pragma once
#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
typedef int SLDatatype;
typedef struct SLNode
{
	SLDatatype val;
	struct SLNode* next;
}SLNode;
void SLN_HeadAdd(SLNode** pphead, SLDatatype x);//插入结点(头插)
void SLN_EndAdd(SLNode** pphead, SLDatatype x);//插入结点(尾插)
SLNode* SLN_Find(SLNode** pphead, SLDatatype x);//查找元素所在的结点
void SLN_fLocAdd(SLNode** pphead, SLNode* Loc, SLDatatype x);//指定位置后插入
void SLN_pLocAdd(SLNode* Loc, SLDatatype x);//指定位置后插入
 
void SLN_Print(SLNode* phead);//打印链表
 
void SLN_HeadDele(SLNode** pphead);//删除结点(头删)
void SLN_EndDele(SLNode** pphead);//删除结点(尾删)
void SLN_LocDele(SLNode** pphead, SLNode* Loc);//指定位置删除
 
void SLN_Free(SLNode** pphead);//销毁链表释放内存

SLNode* SLNAdd(SLDatatype x)//
{
	SLNode* pk = (SLNode*)malloc(sizeof(SLNode));
	assert(pk);
	pk->val = x;
	pk->next = NULL;
	return pk;
}
void SLN_HeadAdd(SLNode** pphead, SLDatatype x)
{
	assert(pphead);
	SLNode* ps = *pphead;
	SLNode* padd = SLNAdd(x);
	if (!ps)//相当于if(pk==NULL)
		*pphead = padd;
	else
	{
		padd->next = ps;
		*pphead = padd;
	}
}
//SLNode* SLN_HeadAdd(SLNode* phead, SLDatatype x)
//{
//	SLNode* ph = SLNAdd(x);
//	if (phead == NULL)
//	{
//		return ph;
//	}
//	else
//	{
//		ph->next = phead;
//		return ph;
//	}
//
//}
void SLN_EndAdd(SLNode** pphead, SLDatatype x)
{
	assert(pphead);
	SLNode* ps = *pphead;
	SLNode* padd = SLNAdd(x);
	if (!ps)
		*pphead = padd;
	else
	{
		while (ps->next)
		{
			ps=ps->next;
		}
		ps->next = padd;
	}
}
void SLN_Print(SLNode* phead)
{
	while (phead)
	{
		printf("%d->", phead->val);
		phead = phead->next;
	}
	printf("NULL\n");
}
//void SLN_Print(SLNode head)
//{
//	while (head.next)
//	{
//		printf("%d->", head.val);
//		 head = *(head.next);
//	}
//	printf("NULL\n");
//}
SLNode* SLN_Find(SLNode** pphead, SLDatatype x)
{
	assert(pphead);
	SLNode* ph = *pphead;
	while (ph)
	{
		if (ph->val == x)
			return ph;
		ph = ph->next;
	}
	return NULL;
}
void SLN_fLocAdd(SLNode** pphead, SLNode* Loc, SLDatatype x)
{
	assert(pphead);
	SLNode* ph = *pphead;
	SLNode* pd = ph, * padd = SLNAdd(x);
	if (Loc==ph)
	{
		SLN_HeadAdd(pphead, x);
		return;
	}
	while (ph->next)
	{
		if (ph == Loc)
		{
			padd->next = Loc;
			pd->next = padd;
			return;
		}
		pd = ph;
		ph = ph->next;
	}
}
void SLN_pLocAdd(SLNode* Loc, SLDatatype x)
{
	SLNode* padd = SLNAdd(x);
	padd->next = Loc->next;
	Loc->next = padd;
}
void SLN_HeadDele(SLNode** pphead)
{
	assert(pphead);
	if (*pphead == NULL)
		return;
	SLNode* ph = (*pphead)->next;//陷阱
	free(*pphead);
	*pphead = ph;
}
void SLN_EndDele(SLNode** pphead)
{
	assert(pphead);
	SLNode* ph = *pphead;
	if (!ph)
		return;
	SLNode* pk = ph;
	while (ph->next)
	{
		pk = ph;
		ph = ph->next;
	}
	pk->next = NULL;
	free(ph);
	ph = NULL;
}
//void SLN_EndDele(SLNode* phead)
//{
//	SLNode* ph = phead;
//	if (!ph)
//		return;
//	SLNode* pk = ph;
//	while (ph->next)
//	{
//		pk = ph;
//		ph = ph->next;
//	}
//	pk->next = NULL;
//	free(ph);
//	ph = NULL;
//}
void SLN_LocDele(SLNode** pphead, SLNode* Loc)
{
	assert(pphead);
	if (*pphead == NULL)
		return;
	SLNode* ph=*pphead,*pk=ph;
	if (Loc == ph)
	{
		SLN_HeadDele(pphead);
		return;
	}
	while (ph)
	{
		if (ph == Loc)
		{
			pk->next = ph->next;
			free(ph);
			ph = NULL;
			return;
		}
		pk = ph;
		ph = ph->next;
	}
}
void SLN_Free(SLNode** pphead)
{
	assert(pphead);
	while (*pphead)
	{
		SLNode* ph = (*pphead)->next;
		free(*pphead);
		*pphead = ph;
	}
	
}