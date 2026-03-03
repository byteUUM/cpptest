// #pragma once
// #include <iostream>
// #include <assert.h>
// using namespace std;
// template<class T>
// struct AVLTreeNode
// {
//     AVLTreeNode(const T& val = T())
//     :_val(val)
//     ,_bf(0)
//     ,_left(nullptr)
//     ,_right(nullptr)
//     ,_prev(nullptr)
//     {}
//     AVLTreeNode<T>* _left;
//     AVLTreeNode<T>* _right;
//     AVLTreeNode<T>* _prev;
//     T _val;
//     int _bf;
// };
// template<class T>
// class AVLTree
// {
//     using Node = AVLTreeNode<T>;
// public:
//     bool insert(const T& val)
//     {
//         Node* cur = _root;
//         Node* newNode = new Node(val);
//         if(_root == nullptr)
//         {
//             _root = newNode;
//             return true;
//         }
//         //查找新节点挂入位置
//         Node* parent = nullptr;
//         while(cur)
//         {
//             parent = cur;
//             if(val<=cur->_val) cur = cur->_left;
//             else cur = cur->_right;
//         }
//         //挂入新节点
//         if(val<=parent->_val) parent->_left = newNode;
//         else parent->_right = newNode;
//         newNode->_prev = parent;

//         cur = newNode;
//         while(parent!=nullptr)
//         {
//             if(cur == parent->_left) parent->_bf--;
//             else parent->_bf++;
            
//             if(parent->_bf==0) return true;//新插入的节点使得原来不平衡部分变得平衡，不用往上更新
//             else if(parent->_bf==1||parent->_bf==-1)
//             {
//                 cur = parent;
//                 parent = parent->_prev;
//             }
//             else if(parent->_bf==2||parent->_bf==-2)
//             {
//                 if(parent->_bf==-2&&cur->_bf==-1) RotateR(parent);
//                 else if(parent->_bf==2&&cur->_bf==1) RotateL(parent);
//                 else if(parent->_bf==-2&&cur->_bf==1)
//                 {
//                     if(cur->_right->_bf==-1) parent->_bf=1;
//                     else cur->_bf=-1;
//                     RotateLR(parent);
//                 }
//                 else if(parent->_bf==2&&cur->_bf==-1)
//                 {
//                     if(cur->_left->_bf==1) cur->_bf=1;
//                     else parent->_bf = -1;
//                     RotateRL(parent);
//                 }
//                 else assert(0);
//                 return true;
//             }
//             else
//             {
//                 assert(0);
//             }
//         }
//         return false;
//     }
//     void RotateL(Node* parent)
//     {
//         Node* subR = parent->_right;
//         Node* subRL = subR->_left;
//         Node* pparent = parent->_prev;

//         parent->_right = subRL;
//         subR->_left = parent;

//         parent->_prev = subR;
//         if(subRL) subRL->_prev = parent;
//         subR->_prev = pparent;
//         if(pparent == nullptr) _root = subR;
//         else
//         {
//             if(pparent->_left == parent) pparent->_left = subR;
//             else pparent->_right = subR;
//         }
//         parent->_bf = 0;
//         subR->_bf = 0;
//     }
//     void RotateR(Node* parent)
//     {
//         Node* subL = parent->_left;
//         Node* subLR = subL->_right;
//         Node* pparent = parent->_prev;

//         parent->_left = subLR;
//         subL->_right = parent;

//         parent->_prev = subL;
//         if(subLR) subLR->_prev = parent;
//         if(pparent == nullptr) _root = subL;
//         else
//         {
//             if(pparent->_left == parent) pparent->_left = subL;
//             else pparent->_right = subL;
//         }
//         parent->_bf = 0;
//         subL->_bf =0;
//     }
//     void RotateLR(Node* parent)
//     {
//         RotateL(parent->_left);
//         RotateR(parent);
//     }
//     void RotateRL(Node* parent)
//     {
//         RotateR(parent->_right);
//         RotateL(parent);
//     }
//     int Height(Node* root)
//     {
//         if(root==nullptr) return 0;
//         int l = Height(root->_left);
//         int r = Height(root->_right);
//         return l>r?l+1:r+1;
//     }
//     bool IsAVLTree()
//     {
//         return _IsAVLTree(_root);
//     }
//     bool _IsAVLTree(Node* root)
//     {
//         if(root==nullptr) return true;
//         int LHeight = Height(root->_left);
//         int RHeight = Height(root->_right);
//         if(LHeight-RHeight>=2||LHeight-RHeight<=-2) return false;
//         return _IsAVLTree(root->_left) && _IsAVLTree(root->_right);
//     }
// private:
//     Node* _root = nullptr;
// };


#pragma once
#include <iostream>
#include <assert.h>
using namespace std;

template<class T>
struct AVLTreeNode
{
    AVLTreeNode(const T& val)
        : _val(val)
        , _left(nullptr)
        , _right(nullptr)
        , _parent(nullptr)
        , _bf(0)
    {}

    T _val;
    AVLTreeNode<T>* _left;
    AVLTreeNode<T>* _right;
    AVLTreeNode<T>* _parent;
    int _bf; // 右高为正，左高为负
};

template<class T>
class AVLTree
{
    using Node = AVLTreeNode<T>;
public:
    bool insert(const T& val)
    {
        if (_root == nullptr)
        {
            _root = new Node(val);
            return true;
        }

        // 1️⃣ 找插入位置
        Node* parent = nullptr;
        Node* cur = _root;

        while (cur)
        {
            if (val == cur->_val)
                return false;

            parent = cur;
            if (val < cur->_val)
                cur = cur->_left;
            else
                cur = cur->_right;
        }

        // 2️⃣ 挂节点
        cur = new Node(val);
        cur->_parent = parent;

        if (val < parent->_val)
            parent->_left = cur;
        else
            parent->_right = cur;

        // 3️⃣ 更新平衡因子
        while (parent)
        {
            if (cur == parent->_left)
                parent->_bf--;
            else
                parent->_bf++;

            if (parent->_bf == 0)
                break; // 高度未变化

            else if (parent->_bf == 1 || parent->_bf == -1)
            {
                cur = parent;
                parent = parent->_parent;
            }
            else if (parent->_bf == 2 || parent->_bf == -2)
            {
                Rebalance(parent);
                break;
            }
        }

        return true;
    }

    bool IsAVLTree()
    {
        return _IsAVLTree(_root);
    }

private:

    // ================== 旋转 ==================

    void RotateL(Node* parent)
    {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        Node* pparent = parent->_parent;

        parent->_right = subRL;
        if (subRL)
            subRL->_parent = parent;

        subR->_left = parent;
        parent->_parent = subR;

        subR->_parent = pparent;

        if (pparent == nullptr)
            _root = subR;
        else
        {
            if (pparent->_left == parent)
                pparent->_left = subR;
            else
                pparent->_right = subR;
        }
    }

    void RotateR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        Node* pparent = parent->_parent;

        parent->_left = subLR;
        if (subLR)
            subLR->_parent = parent;

        subL->_right = parent;
        parent->_parent = subL;

        subL->_parent = pparent;

        if (pparent == nullptr)
            _root = subL;
        else
        {
            if (pparent->_left == parent)
                pparent->_left = subL;
            else
                pparent->_right = subL;
        }
    }

    void Rebalance(Node* parent)
    {
        if (parent->_bf == -2)
        {
            Node* subL = parent->_left;
            if (subL->_bf == -1)  // LL
            {
                RotateR(parent);
                parent->_bf = 0;
                subL->_bf = 0;
            }
            else // LR
            {
                Node* subLR = subL->_right;
                int bf = subLR->_bf;

                RotateL(subL);
                RotateR(parent);

                if (bf == 0)
                {
                    parent->_bf = 0;
                    subL->_bf = 0;
                }
                else if (bf == -1)
                {
                    parent->_bf = 1;
                    subL->_bf = 0;
                }
                else // 1
                {
                    parent->_bf = 0;
                    subL->_bf = -1;
                }

                subLR->_bf = 0;
            }
        }
        else if (parent->_bf == 2)
        {
            Node* subR = parent->_right;
            if (subR->_bf == 1) // RR
            {
                RotateL(parent);
                parent->_bf = 0;
                subR->_bf = 0;
            }
            else // RL
            {
                Node* subRL = subR->_left;
                int bf = subRL->_bf;

                RotateR(subR);
                RotateL(parent);

                if (bf == 0)
                {
                    parent->_bf = 0;
                    subR->_bf = 0;
                }
                else if (bf == 1)
                {
                    parent->_bf = -1;
                    subR->_bf = 0;
                }
                else // -1
                {
                    parent->_bf = 0;
                    subR->_bf = 1;
                }

                subRL->_bf = 0;
            }
        }
    }

    // ================== 校验 ==================

    int Height(Node* root)
    {
        if (!root) return 0;
        int lh = Height(root->_left);
        int rh = Height(root->_right);
        return max(lh, rh) + 1;
    }

    bool _IsAVLTree(Node* root)
    {
        if (!root) return true;

        int lh = Height(root->_left);
        int rh = Height(root->_right);

        if (abs(lh - rh) > 1)
            return false;

        return _IsAVLTree(root->_left)
            && _IsAVLTree(root->_right);
    }

private:
    Node* _root = nullptr;
};