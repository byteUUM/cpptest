#pragma once
#include <iostream>
#include <memory>
using namespace std;
namespace rbTree_model
{
    enum Color
    {
        RED,
        BLACK
    };
    template<class T>
    struct rbNode
    {
        rbNode(T val=T())
        :_parent(nullptr)
        ,_left(nullptr)
        ,_right(nullptr)
        ,_val(val)
        ,_color(Color::RED)
        {}
        shared_ptr<rbNode<T>> _parent;
        shared_ptr<rbNode<T>> _left;
        shared_ptr<rbNode<T>> _right;
        T _val;
        Color _color;
    };
    template<class T>
    class rbTree
    {
        using node_t = shared_ptr<rbNode<T>>;
    private:
        void RotateL(node_t parent)
        {
            node_t pparent = parent->_parent;
            node_t subR = parent->_right;
            node_t subRL = subR->_left;

            parent->_right = subRL;
            subR->_left = parent;
            if(!pparent) _root = subR;
            else
            {
                if(parent == pparent->_left) pparent->_left = subR;
                else pparent->_right = subR;
            }

            parent->_parent = subR;
            if(subRL) subRL->_parent = parent;
            subR->_parent = pparent;
        }
        void RotateR(node_t parent)
        {
            node_t pparent = parent->_parent;
            node_t subL = parent->_left;
            node_t subLR = subL->_right;

            parent->_left = subLR;
            subL->_right = parent;
            if(!pparent) _root = subL;
            else
            {
                if(parent == pparent->_left) pparent->_left = subL;
                else pparent->_right = subL;
            }

            parent->_parent = subL;
            if(subLR) subLR->_parent = parent;
            subL->_parent = pparent;
        }
        bool _IsRBTree(node_t root, int pathCount, int count)
        {
            if(!root) return pathCount == count;
            if(root->_color==Color::BLACK)
            {
                ++pathCount;
            }
            else
            {
                if(root->_left&&root->_left->_color==Color::RED) return false;
                if(root->_right&& root->_right->_color==Color::RED) return false;
            }
            return _IsRBTree(root->_left, pathCount, count)&&_IsRBTree(root->_right, pathCount, count);
        }
    public:
        rbTree()
        :_root(nullptr)
        {}
        bool Insert(T val)
        {
            node_t newNode = node_t(new rbNode(val));
            if(_root==nullptr)
            {
                _root = newNode;
                _root->_color = Color::BLACK;
                return true;
            }
            node_t cur =  _root;
            node_t parent = nullptr;
            while(cur)
            {
                parent = cur;
                if(val<cur->_val) cur = cur->_left;
                else if(val>cur->_val) cur = cur->_right;
                else return false;
            }
            if(val<parent->_val) parent->_left = newNode;
            else parent->_right = newNode;
            newNode->_parent =  parent;

            //平衡调整
            cur = newNode;
            while(cur->_color == Color::RED&&parent&&parent->_color==Color::RED)
            {
                node_t pparent = parent->_parent;
                node_t uncle;
                if(parent==pparent->_left) uncle = pparent->_right;
                else uncle = pparent->_left;
                if(uncle&&uncle->_color==Color::RED)
                {
                    pparent->_color = Color::RED;
                    parent->_color = Color::BLACK;
                    uncle->_color = Color::BLACK;
                }
                else
                {
                    if(parent==pparent->_left)
                    {
                        if(cur==parent->_left)
                        {
                            RotateR(pparent);
                            parent->_color = Color::BLACK;
                            pparent->_color = Color::RED;
                        }
                        else
                        {
                            RotateL(parent);
                            RotateR(pparent);
                            cur->_color = Color::BLACK;
                            pparent->_color = Color::RED;
                        }
                    }
                    else
                    {
                        if(cur==parent->_right)
                        {
                            RotateL(pparent);
                            parent->_color = Color::BLACK;
                            pparent->_color = Color::RED;
                        }
                        else
                        {
                            RotateR(parent);
                            RotateL(pparent);
                            cur->_color = Color::BLACK;
                            pparent->_color = Color::RED;
                        }
                    }
                }
                cur = pparent;
                if(cur) parent = cur->_parent;
            }
            _root->_color = Color::BLACK;
            return true;
        }
            bool IsRBTree()
        {
            if(!_root) return true;
            if(_root->_color!=Color::BLACK) return false;

            int count = 0;
            node_t cur = _root;
            while(cur)
            {
                if(cur->_color == Color::BLACK) ++count;
                cur = cur->_left;
            }
            int pathCount = 1;
            return _IsRBTree(_root->_left,pathCount,count)&&_IsRBTree(_root->_right,pathCount,count);
        }
    private:
        node_t _root;
    };
}