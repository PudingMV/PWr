#pragma once

#include <iostream>
#include <algorithm>
#include <limits>

class Node
{
public:
    int value;

    Node* left;
    Node* right;
    Node* parent;

    Node(int value)
        : value(value),
          left(nullptr),
          right(nullptr),
          parent(nullptr)
    {
    }
};

class Tree
{
public:
    Node* root;

    Tree()
        : root(nullptr)
    {
    }

private:

    void rotateLeft(Node* x)
    {
        Node* y = x->right;

        if (y == nullptr)
            return;

        x->right = y->left;

        if (y->left)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
        {
            root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node* x)
    {
        Node* y = x->left;

        if (y == nullptr)
            return;

        x->left = y->right;

        if (y->right)
            y->right->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
        {
            root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }

    void splay(Node* x)
    {
        while (x->parent != nullptr)
        {
            Node* p = x->parent;
            Node* g = p->parent;

            if (g == nullptr)
            {
                if (x == p->left)
                    rotateRight(p);
                else
                    rotateLeft(p);
            }

            else if (x == p->left &&
                     p == g->left)
            {
                rotateRight(g);
                rotateRight(p);
            }

            else if (x == p->right &&
                     p == g->right)
            {
                rotateLeft(g);
                rotateLeft(p);
            }

            else if (x == p->right &&
                     p == g->left)
            {
                rotateLeft(p);
                rotateRight(g);
            }
            else
            {
                rotateRight(p);
                rotateLeft(g);
            }
        }
    }

public:

    Node* find(int value)
    {
        Node* current = root;

        while (current)
        {
            if (value == current->value)
            {
                splay(current);
                return current;
            }

            if (value < current->value)
                current = current->left;
            else
                current = current->right;
        }

        return nullptr;
    }

    void insert(int value)
    {
        if (root == nullptr)
        {
            root = new Node(value);
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current)
        {
            parent = current;

            if (value < current->value)
                current = current->left;
            else
                current = current->right;
        }

        Node* node = new Node(value);
        node->parent = parent;

        if (value < parent->value)
            parent->left = node;
        else
            parent->right = node;

        splay(node);
    }

    Node* predecessor(Node* node)
    {
        if (node == nullptr ||
            node->left == nullptr)
            return nullptr;

        Node* current = node->left;

        while (current->right)
            current = current->right;

        return current;
    }

    Node* successor(Node* node)
    {
        if (node == nullptr ||
            node->right == nullptr)
            return nullptr;

        Node* current = node->right;

        while (current->left)
            current = current->left;

        return current;
    }

    void deleteValue(int value)
{
    Node* node = root;

    while (node)
    {
        if (value == node->value)
            break;

        if (value < node->value)
            node = node->left;
        else
            node = node->right;
    }

    if (node == nullptr)
        return;

    Node* splayNode = node->parent;

    if (node->left == nullptr &&
        node->right == nullptr)
    {
        if (node->parent == nullptr)
        {
            delete node;
            root = nullptr;
            return;
        }

        if (node == node->parent->left)
            node->parent->left = nullptr;
        else
            node->parent->right = nullptr;

        delete node;
    }
    else if (node->left == nullptr)
    {
        Node* child = node->right;

        child->parent = node->parent;

        if (node->parent == nullptr)
        {
            root = child;
        }
        else if (node == node->parent->left)
        {
            node->parent->left = child;
        }
        else
        {
            node->parent->right = child;
        }

        delete node;
    }
    else if (node->right == nullptr)
    {
        Node* child = node->left;

        child->parent = node->parent;

        if (node->parent == nullptr)
        {
            root = child;
        }
        else if (node == node->parent->left)
        {
            node->parent->left = child;
        }
        else
        {
            node->parent->right = child;
        }

        delete node;
    }
    else
    {
        Node* pred = predecessor(node);

        node->value = pred->value;

        Node* predParent = pred->parent;

        if (pred == predParent->left)
        {
            predParent->left = pred->left;
        }
        else
        {
            predParent->right = pred->left;
        }

        if (pred->left)
            pred->left->parent = predParent;

        delete pred;

        splayNode = predParent;
    }

    if (splayNode)
        splay(splayNode);
}

    int height(Node* node)
    {
        if (node == nullptr)
            return 0;

        int leftHeight = height(node->left);
        int rightHeight = height(node->right);

        return 1 + std::max(leftHeight,
                            rightHeight);
    }

    int height()
    {
        return height(root);
    }

    bool isBST(
        Node* node,
        int minVal = std::numeric_limits<int>::min(),
        int maxVal = std::numeric_limits<int>::max())
    {
        if (node == nullptr)
            return true;

        if (node->value < minVal ||
            node->value > maxVal)
            return false;

        return isBST(
                   node->left,
                   minVal,
                   node->value - 1)
               &&
               isBST(
                   node->right,
                   node->value,
                   maxVal);
    }

    void printBST(
        Node* node,
        std::string prefix = "",
        bool isLeft = true)
    {
        if (node == nullptr)
            return;

        if (node->right)
        {
            printBST(
                node->right,
                prefix + (isLeft ? "│   "
                                 : "    "),
                false);
        }

        std::cout << prefix;

        if (!prefix.empty())
        {
            std::cout
                << (isLeft ? "└── "
                           : "┌── ");
        }

        std::cout << node->value << '\n';

        if (node->left)
        {
            printBST(
                node->left,
                prefix + (isLeft ? "    "
                                 : "│   "),
                true);
        }
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