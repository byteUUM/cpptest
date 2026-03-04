#include <iostream>
#include <assert.h>
using namespace std;

template<class T>
struct AVLTreeNode
{
    AVLTreeNode(T val=T())
    :_val(val)
    ,_bf(0)
    ,_left(nullptr)
    ,_right(nullptr)
    ,_parent(nullptr)
    {}
    AVLTreeNode<T> _left;
    AVLTreeNode<T> _right;
    AVLTreeNode<T> _parent;
    T _val;
    int _bf;
}

template<class T>
class AVLTree
{
    using Node = AVLTreeNode<T>;
public:
    insert(T val)
    {
        Node* newNode = new Node(val);
        if(_root==nullptr)
        {
            _root = newNode;
            return true;
        }
        Node* parent = nullptr;
        Node* cur = _root;
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
        //更新bf
        while(parent)
        {  
            if(parent->_bf==1||parent->_bf==-1)
            {
                cur = parent;
                parent = parent->_parent;
            }
            else if(parent->_bf==2||parent->_bf==-2)
            {
                if(parent->_bf==2)
                {
                    if(cur->_bf==1) RotateL(parent);
                    else RotateRL(parent);
                }
                else
                {
                    if(cur->_bf==-1) RotateR(parent);
                    else RotateLR(parent); 
                }
            }
            else
            {
                assert(0);
            }
        }
    }
private:
    void RotateL(Node* parent)
    {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        Nodeo* ppNode = parent->_parent;

        parent->_right = subRL;
        subR->_left = parent;
        if(!ppNode) _root = subR;
        else
        {
            if(ppNode->_left == parnet) ppNode->_left = subR;
            else ppNode->_right = subR;
        }

        if(subRL) subRL->_parent = parent;
        subR->_parent = ppNode;
        parent->_parent = subR;

        parent->_bf = 0;
        subR->_bf = 0;
    }
    void RotateR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        Node* ppNode = parent->_parent;

        parent->_left = subLR;
        subL->_right = parent;
        if(!ppNode) _root = subL;
        else
        {
            if(ppNode->_left == parent) ppNode->_left = subL;
            else ppNode->_right = subL;
        } 

        if(subLR) subLR->_parent = parent;
        subL->_parent = ppNode;
        parent->_parent = subL;

        parent->_bf = 0;
        subL->_bf = 0;
    }
    void RotateLR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;

        int bf = subLR->_bf;

        RotateL(subL);
        RotateR(parent);

        //......
    }
    void RotateRL(Node* parent)
    {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;

        int bf = subRL->_bf;

        RotateR(subR);
        RotateL(parent);

        if(bf == 1) parent->_bf = -1;
        else subR_bf = 1;
    }
    void RotateLR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;

        int bf = subLR->_bf;

        RotateL(subL);
        RotateR(parent);

        if(bf == -1) parent->_bf = 1;
        else subL->_bf = -1;
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

    Node*  _root;
}