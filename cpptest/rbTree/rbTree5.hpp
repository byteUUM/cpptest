#pragma once 
#include <memory>
#include <assert.h>
namespace rbTree_model
{
    using namespace std;
    enum Color {RED,BLACK};
    template<class T>
    struct rbTreeNode
    {
        rbTreeNode(const T data)
        :_left(nullptr)
        ,_right(nullptr)
        ,_parent(nullptr)
        ,_color(Color::RED)
        ,_data(data)
        {}
        rbTreeNode<T>* _left;
        rbTreeNode<T>* _right;
        rbTreeNode<T>* _parent;
        Color _color;
        T _data;
    }
    template<class T>
    class rbTree
    {
    private:
        using node_t = shared_ptr<rbTreeNode<T>>;
        void RotateL(node_t parent)
        {

        }
        void RotateR(node_t parent)
        {

        }
    public:
        bool insert(const T data)
        {
            node_t newNode = node_t(new node_t(data));
            if(_root = nullptr)
            {
                _root = newNode;
                _root._color = Color::BLACK;
                return true;
            }
            node_t cur = _root;
            node_t prev = nullptr;
            while(cur)
            {
                prev = cur;
                if(data == cur->_data) return false;
                if(data<cur->_data) cur = cur->_left;
                else cur = cur->_right;
            }
            if(data < prev->_left) newNode = prev->_left;
            else newNode = prev->_right;
            newNode->_parent = prev;

            //调整树的结构
            cur = newNode;
            while(cur&&cur->_parent&&cur->_parent->_color==RED)
            {
                node_t parent = cur->parent;
                node_t pparent = parent->parent;
                node_t uncle = nullptr;
                assert(pparent);
                if(parent == pparent->_left) uncle = pparent->_right;
                else uncle = pparent->_left;
                if(uncle&&uncle->_color==RED)
                {
                    pparent->_color = RED;
                    parent->_color = BLACK;
                    uncle->_color = BLACK;
                }
                else
                {
                    if(parent == parent->_left)
                    {
                        if(cur == parent->_left)
                        {
                            //右旋+变色
                        }
                        else
                        {
                            //左右双旋+变色
                        }
                    }
                    else
                    {
                        if(cur == parent->_right)
                        {
                            //左旋+变色
                        }
                        else
                        {
                            //右左双旋+变色
                        }
                    }
                }
                cur = pparent;
            }
            //保证根节点是黑色
            _root->_color = BLACK;
        }
    private:
        node_t _root = nullptr;
    }
}