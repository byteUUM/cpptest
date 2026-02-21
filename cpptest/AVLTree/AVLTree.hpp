#include <iostream>
#include <memory>
#include <assert>
using namespace std;
namespace my_avltree
{
    template<class T>
    struct AVLTreeNode
    {
        AVLTreeNode()
        {}
        AVLTreeNode(int val)
        :_val(val),
        _left(nullptr),
        _right(nullptr),
        _parent(nullptr)
        {}
        shared_ptr<AVLTreeNode<T>> _left;
        shared_ptr<AVLTreeNode<T>> _right;
        shared_ptr<AVLTreeNode<T>> _parent;
        T _val;
        int count;
    }
    template<class T>
    class AVLTree
    {
        using Node=AVLTreeNode<T>;
    private:
        void LRotate(Node parent)
        {
            Node lnode = parent->_left;
            Node pparent = parent->_parent;

            pparent->_left = lnode;
            parent->_left = pparent;
            if(pparent->_parent==nullptr)
            {
                _root = parent;
                parent->_pparent = nullptr;
            }
            else
            {
                if(pparent->_parent->_left == pparent)
                    pparent->_parent->_left = parent;
                else
                    pparent->_parent->_right = parent;    
            }

            lnode->_parent = pparent;
            pparent->_parent = parent;
        }
        void RRotate(Node parent)
        {
            Node rnode = parent->_right;
            Node pparent = parent->_parent;

            pparent->_right = rnode;
            parent->_right = pparent;
            if(pparent->_parent==nullptr)
            {
                _root = parent;
                parent->_pparent = nullptr;
            }
            else
            {
                if(pparent->_parent->_left == pparent)
                    pparent->_parent->_left = parent;
                else
                    pparent->_parent->_right = parent;    
            }

            rnode->_parent = pparent;
            pparent->_parent = parent;
        }
    public:
        AVLTree()
        :_root(nullptr)
        {}
        bool insert(T val)
        {
            Node newNode = shared_ptr<Node>(new node(val));
            if(!_root)
            {
                _root = newNode;
                return true;
            }
            //查找插入位置
            Node cur = _root;
            Node prev = nullptr;
            while(!cur)
            {
                if(val<cur->_val)
                {
                    prev = cur;
                    cur = cur->_left;
                }
                else
                {
                    prev = cur;
                    cur = cur->_right;
                }
            }
            //插入，并更新计数
            if(val < prev->_val)
            {
                prev->_left = newNode;
                prev->_count--;
            }
            else
            {
                prev->_right = newNode;
                prev->_count++;
            }
            //检查高等
            if(prev->_count==1||prev->_count==-1)
            {
                //高度合法
                return true;
            }
            //调整高度
            if(prev->_count==-2)
            {
                assert(prev->_left);
                if(prev->_left->_count==-1)
                {
                    //右旋
                }
                else
                {
                    //左右双旋
                }
            }
            else
            {
                assert(prev->right);
                if(prev->_right->_count==1)
                {
                    //左旋
                }
                else
                {
                    //右左双旋
                }

            }

        }

    private:
        Node _root;
    }
}