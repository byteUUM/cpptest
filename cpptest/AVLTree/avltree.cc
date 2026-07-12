#include <iostream>
#include <algorithm>

struct Node {
    int val;
    int height;
    Node* left;
    Node* right;
    Node(int v) : val(v), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    Node* root = nullptr;

    int height(Node* n) { return n ? n->height : 0; }

    int balanceFactor(Node* n) { return n ? height(n->left) - height(n->right) : 0; }

    void updateHeight(Node* n) {
        n->height = 1 + std::max(height(n->left), height(n->right));
    }

    // 右旋
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // 左旋
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* insert(Node* node, int val) {
        if (!node) return new Node(val);

        if (val < node->val)
            node->left = insert(node->left, val);
        else if (val > node->val)
            node->right = insert(node->right, val);
        else
            return node; // 不允许重复值

        updateHeight(node);
        int bf = balanceFactor(node);

        // LL
        if (bf > 1 && val < node->left->val)
            return rotateRight(node);
        // RR
        if (bf < -1 && val > node->right->val)
            return rotateLeft(node);
        // LR
        if (bf > 1 && val > node->left->val) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // RL
        if (bf < -1 && val < node->right->val) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void inorder(Node* n) {
        if (!n) return;
        inorder(n->left);
        std::cout << n->val << " ";
        inorder(n->right);
    }

public:
    void insert(int val) { root = insert(root, val); }
    void printInorder() { inorder(root); std::cout << std::endl; }
};

int main() {
    AVLTree tree;
    int vals[] = {10, 20, 30, 40, 50, 25};
    for (int v : vals) tree.insert(v);

    tree.printInorder(); // 中序遍历验证结果（应为有序序列）
    return 0;
}
