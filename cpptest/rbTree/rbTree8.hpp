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

        }
        void RotateR(node_t* parent)
        {

        }
        bool _IsRBTree(node_t* root)
        {

        }
    public:
        bool Insert(T& val)
        {

        }
        bool IsRBTree()
        {

        }
    private:
        node_t* _root;
    };
}