#pragma once
#include <iostream>

namespace rbTree_model
{
    enum Color(){RED, BLACK};
    template<class T>
    struct rbNode
    {
        rbNode(T& val)
        :_left(nullptr)
        ,_right(nullptr)
        ,_parent(nullptr)
        ,_val(val)
        ,_color(Color::RED)
        {}
        rbNode* _left;
        rbNode* _right;
        rbNode* _parent;
        T _val;
        Color _color;
    };
    template<class T>
    class rbTree
    {
        using node_t = rbNode<T>;
    private:
        void RotateL(node_t* parent)
        {
            subR = parent->_right;
            subRL = subR->_left;
            pparent = parent->_parent;

            parent->_right = subRL;
            subR->_left = parent;
            if(!pparent) _root = subR;
            else
            {
                if(parent==pparent->_left) pparent->_left = subR;
                else pparent->_right = subR;
            }

            parent->_parent = subR;
            subR->_parent = pparent;
            if(subRL) subRL->_parent = parent;
        }
        void RotateR(node_t* parent)
        {
            subL = parent->_left;
            subLR = subL->_right;
            pparent = parent->_parent;

            parent->_left = subLR;
            subL->_right = parent;
            if(!pparent) _root = subL;
            else
            {
                if(parent = pparent->_left) pparent->_left = subL;
                else pparent->_right = subL;
            }
            subL->_parent = pparent;
            parent->_parent = subL;
            if(subLR) subLR->_parent = parent;
        }
        bool _IsRBTree(node_t* root,int count, int& pathCount)
        {
            if(!root) return count==pathCount;
            if(root->_color==BLACK) pathCount++;
            else
            {
                if(root->_left&&root->_left->_color==RED) return false;
                if(root->_right&&root->_right->_color==RED) return false;
            }
            return _IsRBTree(root->_left,count,pathCount)&&_IsRBTree(root->_right,count,pathCount);
        }
    public:
        bool Insert(T& val)
        {
            node_t newNode = new node_t(val);
            if(!_root)
            {
                _root = newNode;
                _root->_color = Color::BLACK;
                return;
            }
            node_t cur = _root, parent = nullptr;
            while(cur)
            {
                parent = cur;
                if(val<cur->_val) cur = cur->_left;
                else if(val>cur->_val) cur = cur->_right;
                else
                {
                    delete newNode;
                    return false;
                }
            }
            if(val<parent->_val) parent->_left = newNode;
            else parent->_rigth = newNode;
            newNode->_parent = parent;
            
            cur = newNode;
            while(parent&&parent->_parent&&parent->_color==Color::RED)
            {
                pparent = parent->_parent;
                node_t uncle = nullptr;
                if(parent == pparent->_left) uncle = pparent->_right;
                else uncle = pparent->_left;

                if(uncle&&uncle->_color==Color::RED)
                {
                    pparent->_color = Color::RED;
                    parent->_color = Color::BLACK;
                    uncle->_color = BLACK;
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
            _root->_color = Color::BRACK;
            return true;
        }
        bool IsRBTree()
        {
            if(!root) return true;
            if(root->_color == RED) return false;
            int count = 0;
            node_t cur = _root;
            while(cur)
            {
                if(cur->_color == BLACK)
                    count++;
                cur = cur->_left;
            }
            return _IsRBTree(_root,count,pathCount)&&_isRBTree(_root,count,pathCount)
        }
    private:
        node_t* _root=nullptr;
    };
}