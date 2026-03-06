#pragma once
#include<iostream>
using namespace std;
enum Color{red, black};
template<class T>
struct RBNode
{
	RBNode(T key)
		:data(key)
		,color(red)
		,left(nullptr)
		,right(nullptr)
		,prev(nullptr){}
	T data;
	enum Color color;
	RBNode<T>* left;
	RBNode<T>* right;
	RBNode<T>* prev;
};
template<class T>
class RBTree
{
public:
	typedef RBNode<T> Node;
	RBTree()
		:root(nullptr){}
	bool insert(T data)
	{
		Node* newNode = new Node(data);
		if (root == nullptr)
		{
			root = newNode;
			root->color = black;
			return true;
		}
		Node* cur = root;
		Node* parent = root;
		while (cur)
		{
			parent = cur;
			if (newNode->data.first <= cur->data.first)
				cur = cur->left;
			else cur = cur->right;
		}
		if (newNode->data.first <= parent->data.first)
			parent->left = newNode;
		else parent->right = newNode;
		newNode->prev = parent;
		//需要调整
		cur = newNode;
		Node* grandfather = parent->prev;
		Node* uncle = nullptr;
		while (parent&&parent->color == red)
		{
			grandfather = parent->prev;
			if (parent == grandfather->left)
			{
				uncle = grandfather->right;
				if (uncle && uncle->color == red)
				{
					grandfather->color = red;
					parent->color = uncle->color = black;
					cur = grandfather;
					parent = cur->prev;
				}
				else
				{
					if (cur == parent->left)
					{
						ReverseR(grandfather);
						parent->color = black;
						grandfather->color = red;
					}
					else
					{
						ReverseL(parent);
						ReverseR(grandfather);
						cur->color = black;
						parent->color = grandfather->color = red;
					}
				}
			}
			else
			{
				uncle = grandfather->left;
				if (uncle && uncle->color == red)
				{
					grandfather->color = red;
					parent->color = uncle->color = black;
					cur = grandfather;
					parent = cur->prev;
				}
				else
				{
					if (cur == parent->right)
					{
						ReverseL(grandfather);
						parent->color = black;
						grandfather->color = red;
					}
					else
					{
						ReverseR(parent);
						ReverseL(grandfather);
						cur->color = black;
						parent->color = grandfather->color = red;
					}
				}
			}
		}
		root->color = black;
		return true;
	}
	void ReverseR(Node* parent)
	{
		Node* subL = parent->left;
		Node* subLR = subL->right;
		parent->left=subLR;
		subL->right = parent;
		//
		Node* pparent = parent->prev;
		subL->prev = pparent;
		if (pparent == nullptr) root = subL;
		else
		{
			if (pparent->left == parent) pparent->left = subL;
			else pparent->right = subL;
		}
		if (subLR) subLR->prev = parent;
		parent->prev = subL;
	}
	void ReverseL(Node* parent)
	{
		Node* subR = parent->right;
		Node* subRL = subR->left;
		parent->right = subRL;
		subR->left = parent;
		//
		Node* pparent = parent->prev;
		subR->prev = pparent;
		if (pparent == nullptr) root = subR;
		else
		{
			if (pparent->left == parent) pparent->left = subR;
			else pparent->right = subR;
		}
		if (subRL) subRL->prev = parent;
		parent->prev = subR;
	}
	bool IsBalanceTree()
	{
		if (root == nullptr) return true;
		if (root->color == red) return false;
		int count = 0;
		Node* cur = root;
		while (cur)
		{
			if (cur->color == black) count++;
			cur = cur->left;
		}
		return Check(root, 0, count);
	}
	bool Check(Node* root,int path,const int refNum)
	{
		if (root == nullptr) return path == refNum;
		if (root->color == red && root->prev->color == red) return false;
		if (root->color == black) path++;
		return Check(root->left, path, refNum) && Check(root->right, path, refNum);
	}
private:
	Node* root;
};
