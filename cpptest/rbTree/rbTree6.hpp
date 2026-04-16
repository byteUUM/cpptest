#pragma once
#include <memory>
#include <iostream>
#include <functional>
namespace rbTree_model
{
    enum Color{RED,BLACK};
    //using namespace std;
    template<class T>
    struct rbNode
    {
        rbNode(const T data)
        :_left(nullptr)
        ,_right(nullptr)
        ,_parent(nullptr)
        ,_data(data)
        ,_color(RED)
        {}
        std::shared_ptr<rbNode<T>> _left;
        std::shared_ptr<rbNode<T>> _right;
        std::shared_ptr<rbNode<T>> _parent;

        T _data;
        enum Color _color;
    };
    template<class T>
    class rbTree 
    {
        using node_t = std::shared_ptr<rbNode<T>>;
    private:
        void RotateL(node_t parent)
        {
            node_t subR = parent->_right;
            node_t subRL = subR->_left;
            node_t pparent = parent->_parent;

            parent->_right = subRL;
            subR->_left = parent;
            if(!pparent) _root = subR;
            else
            {
                if(parent == pparent->_left) pparent->_left = subR;
                else pparent->_right = subR;
            }

            subR->_parent = pparent;
            if(subRL) subRL->_parent = parent;
            parent->_parent = subR;
        }
        void RotateR(node_t parent)
        {
            node_t subL = parent->_left;
            node_t subLR = subL->_right;
            node_t pparent = parent->_parent;

            parent->_left = subLR;
            subL->_right = parent;
            if(!pparent) _root = subL;
            else
            {
                if(parent == pparent->_left) pparent->_left = subL;
                else pparent->_right = subL;
            } 

            subL->_parent = pparent;
            if(subLR) subLR->_parent = parent;
            parent->_parent = subL;
        }
        bool _IsRBTree(const node_t& root, const int count, int path)
        {
            if(!root) return count==path;
            if(root->_color == BLACK) path++;
            else
            {
                if(root->_left&&root->_left->_color==RED) return false;
                if(root->_right&&root->_right->_color==RED) return false;
            }
            return _IsRBTree(root->_left, count, path)&&_IsRBTree(root->_right, count, path);
        }
    public:
        bool Insert(const T data)
        {
            node_t newNode = node_t(new rbNode<T>(data));
            if(!_root)
            {
                _root = newNode;
                _root->_color = BLACK;
                return true;
            }
            node_t cur = _root;
            node_t prev = nullptr;
            while(cur)
            {
                prev = cur;
                if(data == cur->_data) return false;
                else if(data<cur->_data) cur = cur->_left;
                else cur = cur->_right;
            }
            if(data<prev->_data) prev->_left = newNode;
            else prev->_right = newNode;
            newNode->_parent = prev;

            //平衡调整
            cur = newNode;
            while(cur->_parent&&cur->_parent->_color==RED)
            {
                node_t parent = cur->_parent;
                node_t pparent = parent->_parent;
                node_t uncle = nullptr;
                if(parent == pparent->_left) uncle = pparent->_right;
                else uncle = pparent->_left;
                if(uncle&&uncle->_color==RED)
                {
                    parent->_color = BLACK;
                    uncle->_color = BLACK;
                    pparent->_color = RED;
                }
                else
                {
                    if(parent == pparent->_left)
                    {
                        if(cur == parent->_left)
                        {
                            //右旋+变色
                            RotateR(pparent);
                            parent->_color = BLACK;
                            pparent->_color = RED;
                        }
                        else
                        {
                            //左旋+右旋+变色
                            RotateL(parent);
                            RotateR(pparent);
                            cur->_color = BLACK;
                            pparent->_color = RED;
                        }
                    }
                    else
                    {
                        if(cur == parent->_right)
                        {
                            //左旋+变色
                            RotateL(pparent);
                            parent->_color = BLACK;
                            pparent->_color = RED;
                        }
                        else
                        {
                            //右旋+左旋+变色
                            RotateR(parent);
                            RotateL(pparent);
                            cur->_color = BLACK;
                            pparent->_color = RED;
                        }
                    }
                }
                //更新子节点
                cur = pparent;
            }
            //确保根节点是黑色
            _root->_color = BLACK;
            return true;
        }
        bool IsRBTree()
        {
            if(!_root) return true;
            if(_root->_color==RED) return false;
            int count = 0;
            node_t cur = _root;
            while(cur)
            {
                if(cur->_color == BLACK) count++;
                cur = cur->_left;
            }
            int path = 0;
            return _IsRBTree(_root, count,path);
        }
    private:
        node_t _root;
    };

}