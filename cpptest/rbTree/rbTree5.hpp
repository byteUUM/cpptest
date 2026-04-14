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
        rbTreeNode(T data)
        :_left(nullptr)
        ,_right(nullptr)
        ,_parent(nullptr)
        ,_color(Color::RED)
        ,_data(data)
        {}
        shared_ptr<rbTreeNode<T>> _left;
        shared_ptr<rbTreeNode<T>> _right;
        shared_ptr<rbTreeNode<T>> _parent;
        Color _color;
        T _data;
    };
    template<class T>
    class rbTree
    {
    private:
        using node_t = shared_ptr<rbTreeNode<T>>;
        void RotateL(node_t parent)
        {
            node_t subr = parent->_right;
            node_t subrl = subr->_left;
            node_t pparent = parent->_parent;
            
            parent->_right = subrl;
            subr->_left = parent;
            if(!pparent) _root = subr;
            else
            {
                if(pparent->_left == parent) pparent->_left = subr;
                else pparent->_right = subr;
            }

            subr->_parent = pparent;
            if(subrl) subrl->_parent = parent;
            parent->_parent = subr;
        }
        void RotateR(node_t parent)
        {
            node_t subl = parent->_left;
            node_t sublr = subl->_right;
            node_t pparent = parent->_parent;

            subl->_right = parent;
            parent->_left = sublr;
            if(!pparent) _root = subl;
            else
            {
                if(pparent->_left == parent) pparent->_left = subl;
                else pparent->_right = subl;
            }

            subl->_parent = pparent;
            if(sublr) sublr->_parent = parent;
            parent->_parent = subl;
        }
        bool _IsRBTree(node_t root, int path, const int count)
        {
            if(!root)
            {
                if(path==count) return true;
                else return false;
            }
            if(root->_color == BLACK) ++path;
            else
            {
                if(root->_left&&root->_left->_color == RED) return false;
                if(root->_right&&root->_right->_color == RED) return false;
            }
            return _IsRBTree(root->_left,path,count)&&_IsRBTree(root->_right,path,count);
        }
    public:
        bool Insert(T data)
        {
            node_t newNode = node_t(new rbTreeNode<T>(data));
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
                if(data<cur->_data) cur = cur->_left;
                else cur = cur->_right;
            }
            if(data < prev->_data) prev->_left = newNode;
            else prev->_right = newNode;
            newNode->_parent = prev;

            //调整树的结构
            cur = newNode;
            while(cur&&cur->_parent&&cur->_parent->_color==RED)
            {
                node_t parent = cur->_parent;
                node_t pparent = parent->_parent;
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
                            //左右双旋+变色
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
                            //右左双旋+变色
                            RotateR(parent);
                            RotateL(pparent);
                            cur->_color = BLACK;
                            pparent->_color = RED;
                        }
                    }
                }
                cur = pparent;
            }
            //保证根节点是黑色
            _root->_color = BLACK;
            return true;
        }
        bool IsRBTree()
        {
            if(!_root) return true;
            if(_root->_color == RED) return false;
            node_t cur = _root;
            int count = 0;
            while(cur)
            {
                if(cur->_color == BLACK) ++count;
                cur = cur->_left;
            }
            int path = 0;
            return _IsRBTree(_root,path,count);
        }
    private:
        node_t _root = nullptr;
    };
}