#include <iostream>

class Node
{
public:
    int value;
    Node *left;
    Node *right;

    Node(int value) : value(value), left(nullptr), right(nullptr) {}
};

class Tree
{
public:
    Node *root;

    Tree() : root(nullptr) {}

    void insert(int value)
    {
        if (root == nullptr)
        {
            root = new Node(value);
            return;
        }

        Node *current = root;

        while (true)
        {
            if (value < current->value)
            {
                if (current->left == nullptr)
                {
                    current->left = new Node(value);
                    break;
                }
                else
                {
                    current = current->left;
                }
            }
            else
            {
                if (current->right == nullptr)
                {
                    current->right = new Node(value);
                    break;
                }
                else
                {
                    current = current->right;
                }
            }
        }
    }

    Node* predecessor(Node *node)
    {
        if (node->left == nullptr)
            return nullptr;

        Node *current = node->left;

        while (current->right != nullptr)
        {
            current = current->right;
        }

        return current;
    }

    Node* successor(Node *node)
    {
        if (node->right == nullptr)
            return nullptr;

        Node *current = node->right;

        while (current->left != nullptr)
        {
            current = current->left;
        }

        return current;
    }

    void deleteValue(Node*& node, int value)
    {
    if (node == nullptr)
        return;

    if (value < node->value)
    {
        deleteValue(node->left, value);
    }
    else if (value > node->value)
    {
        deleteValue(node->right, value);
    }
    else
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            node = nullptr;
        }
        else if (node->left == nullptr)
        {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        else if (node->right == nullptr)
        {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        else
        {
            Node* pred = predecessor(node);

            node->value = pred->value;

            deleteValue(node->left, pred->value);
        }
    }
}

    int height(Node *node)
    {
        if (node == nullptr)
            return 0;

        int leftHeight = height(node->left);
        int rightHeight = height(node->right);

        return 1 + std::max(leftHeight, rightHeight);
    }

    bool isBST(Node* node, int minVal, int maxVal)
    {
        if (node == nullptr)
            return true;

        if (node->value < minVal ||
            node->value > maxVal)
            return false;

        return isBST(node->left,
                     minVal,
                    node->value - 1)
           &&
               isBST(node->right,
                     node->value,
                     maxVal);
    }
    
    void printBST(Node* node, std::string prefix = "", bool isLeft = true)
    {
        if (node == nullptr)
            return;

        if (node->right)
            printBST(node->right,
                     prefix + (isLeft ? "│   " : "    "),
                     false);

        std::cout << prefix;

        if (!prefix.empty())
            std::cout << (isLeft ? "└── " : "┌── ");

        std::cout << node->value << '\n';

        if (node->left)
            printBST(node->left,
                     prefix + (isLeft ? "    " : "│   "),
                     true);
    }
    ~Tree()
    {
        destroy(root);
    }

private:
    void destroy(Node* node)
    {
        if (node == nullptr)
            return;

        destroy(node->left);
        destroy(node->right);

        delete node;
    }
};