#pragma once
#include <iostream>
#include <assert.h>

enum Color {RED,BLACK};
template<class T>
struct rbTreeNode
{
    rbTreeNode(T val=T())
    :_left(nullptr)
    ,_right(nullptr)
    ,_parent(nullptr)
    ,_val(val)
    ,_color(RED)
    {}
    rbTreeNode<T>* _left;
    rbTreeNode<T>* _right;
    rbTreeNode<T>* _parent;
    T _val;
    Color _color;
};

template<class T>
class rbTree
{
    using Node = rbTreeNode<T>;
private:
    void RotateL(Node* parent)
    {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        Node* pparent = parent->_parent;

        parent->_right = subRL;
        subR->_left = parent;
        if(!pparent) _root = subR;
        else if(pparent->_left == parent) pparent->_left = subR;
        else pparent->_right = subR;

        subR->_parent = pparent;
        if(subRL) subRL->_parent = parent;
        parent->_parent = subR;
    }
    void RotateR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        Node* pparent = parent->_parent;

        parent->_left = subLR;
        subL->_right = parent;
        if(!pparent) _root = subL;
        else if(pparent->_left == parent) pparent->_left = subL;
        else pparent->_right = subL;

        subL->_parent = pparent;
        if(subLR) subLR->_parent = parent;
        parent->_parent = subL;
    }
    bool _IsRBTree(Node* root,int count,int pathcount)
    {
        if(!root) return count==pathcount;
        if(root->_color==BLACK) pathcount++;
        else
        {
            if(root->_left&&root->_left->_color==RED) return false;
            if(root->_right&&root->_right->_color==RED) return false;
        }
        return _IsRBTree(root->_left,count,pathcount)&&_IsRBTree(root->_right,count,pathcount);
    }
public:
    bool insert(T val)
    {
        Node* newNode = new Node(val);
        if(!_root)
        {
            _root = newNode;
            _root->_color = BLACK;
            return true;
        }
        Node* cur = _root;
        Node* parent = nullptr;
        while(cur)
        {
            parent = cur;
            if(val==cur->_val) return false;
            else if(val<cur->_val) cur = cur->_left;
            else cur = cur->_right;
        }
        if(val<parent->_val) parent->_left = newNode;
        else parent->_right = newNode;
        newNode->_parent = parent;
        cur = newNode;
        while(parent&&parent->_color==RED)
        {
            Node* pparent = parent->_parent;
            if(!pparent)
            {
                _root = parent;
                _root->_color = BLACK;
                return true;
            }
            Node* uncle = nullptr;
            if(pparent->_left == parent) uncle = pparent->_right;
            else uncle = pparent->_left;
            if(uncle&&uncle->_color==RED)
            {
                parent->_color=BLACK;
                uncle->_color=BLACK;
                pparent->_color=RED;
            }
            else
            {
                if(parent==pparent->_left)
                {
                    if(cur==parent->_left)
                    {
                        RotateR(pparent);
                        parent->_color = BLACK;
                        pparent->_color = RED;
                    }
                    else
                    {
                        RotateL(parent);
                        RotateR(pparent);
                        cur->_color = BLACK;
                        pparent->_color = RED;
                    }
                }
                else
                {
                    if(cur==parent->_right)
                    {
                        RotateL(pparent);
                        parent->_color = BLACK;
                        pparent->_color = RED;
                    }
                    else
                    {
                        RotateR(parent);
                        RotateL(pparent);
                        cur->_color = BLACK;
                        pparent->_color = RED;
                    }
                }
            }
            cur = pparent;
            parent = cur->_parent;
        }
        _root->_color = BLACK;
        return true;
    }
    bool IsRBTree()
    {
        if(!_root) return false;
        if(_root->_color==RED) return false;
        int count = 0,pathcount = 0;
        Node* cur = _root;
        while(cur)
        {
            if(cur->_color==BLACK) count++;
            cur = cur->_left;
        }
        return _IsRBTree(_root,count,pathcount);
    }
private:
    Node* _root = nullptr;
};
