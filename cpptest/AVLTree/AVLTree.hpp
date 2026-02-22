#include <iostream>
#include <memory>
#include <cassert>
using namespace std;

namespace my_avltree
{
    template<class T>
    struct AVLTreeNode
    {
        AVLTreeNode() = default;
        AVLTreeNode(const T& val)
        :_val(val),
        _left(nullptr),
        _right(nullptr),
        _parent(nullptr),
        _bf(0)  // 平衡因子：右边高度-左边高度
        {}
        
        shared_ptr<AVLTreeNode<T>> _left;
        shared_ptr<AVLTreeNode<T>> _right;
        shared_ptr<AVLTreeNode<T>> _parent;
        T _val;
        int _bf;  // 平衡因子 balance factor
    };
    
    template<class T>
    class AVLTree
    {
        using Node = AVLTreeNode<T>;
        using pNode = shared_ptr<Node>;
        
    private:
        // 左旋
        void RotateL(pNode parent)
        {
            pNode subR = parent->_right;
            pNode subRL = subR->_left;
            pNode pparent = parent->_parent;
            
            // 重新链接
            parent->_right = subRL;
            if(subRL) subRL->_parent = parent;
            
            subR->_left = parent;
            parent->_parent = subR;
            
            subR->_parent = pparent;
            if(pparent == nullptr)
            {
                _root = subR;
            }
            else
            {
                if(pparent->_left == parent)
                    pparent->_left = subR;
                else
                    pparent->_right = subR;
            }
            
            // 更新平衡因子
            parent->_bf = subR->_bf = 0;
        }
        
        // 右旋
        void RotateR(pNode parent)
        {
            pNode subL = parent->_left;
            pNode subLR = subL->_right;
            pNode pparent = parent->_parent;
            
            // 重新链接
            parent->_left = subLR;
            if(subLR) subLR->_parent = parent;
            
            subL->_right = parent;
            parent->_parent = subL;
            
            subL->_parent = pparent;
            if(pparent == nullptr)
            {
                _root = subL;
            }
            else
            {
                if(pparent->_left == parent)
                    pparent->_left = subL;
                else
                    pparent->_right = subL;
            }
            
            // 更新平衡因子
            parent->_bf = subL->_bf = 0;
        }
        
        // 左右双旋
        void RotateLR(pNode parent)
        {
            pNode subL = parent->_left;
            pNode subLR = subL->_right;
            int bf = subLR->_bf;
            
            RotateL(parent->_left);
            RotateR(parent);
            
            // 更新平衡因子
            if(bf == 1)
            {
                parent->_bf = 0;
                subL->_bf = -1;
                subLR->_bf = 0;
            }
            else if(bf == -1)
            {
                parent->_bf = 1;
                subL->_bf = 0;
                subLR->_bf = 0;
            }
            else
            {
                parent->_bf = subL->_bf = subLR->_bf = 0;
            }
        }
        
        // 右左双旋
        void RotateRL(pNode parent)
        {
            pNode subR = parent->_right;
            pNode subRL = subR->_left;
            int bf = subRL->_bf;
            
            RotateR(parent->_right);
            RotateL(parent);
            
            // 更新平衡因子
            if(bf == 1)
            {
                parent->_bf = -1;
                subR->_bf = 0;
                subRL->_bf = 0;
            }
            else if(bf == -1)
            {
                parent->_bf = 0;
                subR->_bf = 1;
                subRL->_bf = 0;
            }
            else
            {
                parent->_bf = subR->_bf = subRL->_bf = 0;
            }
        }
        
    public:
        AVLTree()
        :_root(nullptr)
        {}
        
        bool insert(const T& val)
        {
            // 创建新节点
            pNode newNode = make_shared<Node>(val);
            
            if(!_root)
            {
                _root = newNode;
                return true;
            }
            
            // 查找插入位置
            pNode cur = _root;
            pNode parent = nullptr;
            while(cur)
            {
                if(val < cur->_val)
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else if(val > cur->_val)
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else
                {
                    return false; // 不允许重复值
                }
            }
            
            // 插入新节点
            if(val < parent->_val)
            {
                parent->_left = newNode;
            }
            else
            {
                parent->_right = newNode;
            }
            newNode->_parent = parent;
            
            // 更新平衡因子并检查平衡
            cur = newNode;
            while(parent)
            {
                // 更新平衡因子
                if(cur == parent->_left)
                    parent->_bf--;
                else
                    parent->_bf++;
                
                // 检查平衡
                if(parent->_bf == 0)
                {
                    break;  // 高度不变，不影响上层
                }
                else if(parent->_bf == 1 || parent->_bf == -1)
                {
                    // 高度变化，继续向上更新
                    cur = parent;
                    parent = parent->_parent;
                }
                else if(parent->_bf == 2 || parent->_bf == -2)
                {
                    // 不平衡，需要旋转
                    if(parent->_bf == 2)
                    {
                        if(cur->_bf == 1)
                            RotateL(parent);      // 左左 -> 右旋
                        else
                            RotateRL(parent);     // 左右 -> 右左双旋
                    }
                    else
                    {
                        if(cur->_bf == -1)
                            RotateR(parent);      // 右右 -> 左旋
                        else
                            RotateLR(parent);     // 右左 -> 左右双旋
                    }
                    break;
                }
            }
            return true;
        }
        
        void InOrder()
        {
            _InOrder(_root);
            cout << endl;
        }
        
    private:
        void _InOrder(pNode root)
        {
            if(root)
            {
                _InOrder(root->_left);
                cout << root->_val << "(" << root->_bf << ") ";
                _InOrder(root->_right);
            }
        }
        
    private:
        pNode _root;
    };
}

// 测试代码
int main()
{
    my_avltree::AVLTree<int> avl;
    
    // 测试插入
    int arr[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
    for(auto e : arr)
    {
        avl.insert(e);
        avl.InOrder();
    }
    
    return 0;
}