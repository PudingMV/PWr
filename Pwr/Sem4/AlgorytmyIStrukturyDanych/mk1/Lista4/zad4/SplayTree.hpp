#pragma once

#include <iostream>
#include <algorithm>
#include <limits>

struct OperationStats
{
    long long comparisons = 0;
    long long pointerOperations = 0;

    void reset()
    {
        comparisons = 0;
        pointerOperations = 0;
    }
};

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

    OperationStats currentStats;

    Tree()
        : root(nullptr)
    {
    }

private:

    void rotateLeft(Node* x)
    {
        currentStats.pointerOperations++;

        Node* y = x->right;

        if (y == nullptr)
            return;

        currentStats.pointerOperations++;
        x->right = y->left;

        if (y->left)
        {
            currentStats.pointerOperations++;
            y->left->parent = x;
        }

        currentStats.pointerOperations++;
        y->parent = x->parent;

        if (x->parent == nullptr)
        {
            currentStats.pointerOperations++;
            root = y;
        }
        else if (x == x->parent->left)
        {
            currentStats.pointerOperations++;
            x->parent->left = y;
        }
        else
        {
            currentStats.pointerOperations++;
            x->parent->right = y;
        }

        currentStats.pointerOperations += 2;

        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node* x)
    {
        currentStats.pointerOperations++;

        Node* y = x->left;

        if (y == nullptr)
            return;

        currentStats.pointerOperations++;
        x->left = y->right;

        if (y->right)
        {
            currentStats.pointerOperations++;
            y->right->parent = x;
        }

        currentStats.pointerOperations++;
        y->parent = x->parent;

        if (x->parent == nullptr)
        {
            currentStats.pointerOperations++;
            root = y;
        }
        else if (x == x->parent->right)
        {
            currentStats.pointerOperations++;
            x->parent->right = y;
        }
        else
        {
            currentStats.pointerOperations++;
            x->parent->left = y;
        }

        currentStats.pointerOperations += 2;

        y->right = x;
        x->parent = y;
    }

    void splay(Node* x)
    {
        while (x->parent != nullptr)
        {
            currentStats.pointerOperations += 2;

            Node* p = x->parent;
            Node* g = p->parent;

            if (g == nullptr)
            {
                currentStats.pointerOperations++;

                if (x == p->left)
                    rotateRight(p);
                else
                    rotateLeft(p);
            }
            else if (x == p->left &&
                     p == g->left)
            {
                currentStats.pointerOperations += 2;

                rotateRight(g);
                rotateRight(p);
            }
            else if (x == p->right &&
                     p == g->right)
            {
                currentStats.pointerOperations += 2;

                rotateLeft(g);
                rotateLeft(p);
            }
            else if (x == p->right &&
                     p == g->left)
            {
                currentStats.pointerOperations += 2;

                rotateLeft(p);
                rotateRight(g);
            }
            else
            {
                currentStats.pointerOperations += 2;

                rotateRight(p);
                rotateLeft(g);
            }
        }
    }

public:

    Node* find(int value)
    {
        currentStats.reset();

        Node* current = root;

        while (current)
        {
            currentStats.comparisons++;

            if (value == current->value)
            {
                splay(current);
                return current;
            }

            currentStats.comparisons++;

            if (value < current->value)
            {
                currentStats.pointerOperations++;
                current = current->left;
            }
            else
            {
                currentStats.pointerOperations++;
                current = current->right;
            }
        }

        return nullptr;
    }

    void insert(int value)
    {
        currentStats.reset();

        if (root == nullptr)
        {
            root = new Node(value);

            currentStats.pointerOperations++;
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current)
        {
            parent = current;

            currentStats.comparisons++;

            if (value < current->value)
            {
                currentStats.pointerOperations++;
                current = current->left;
            }
            else
            {
                currentStats.pointerOperations++;
                current = current->right;
            }
        }

        Node* node = new Node(value);

        currentStats.pointerOperations++;

        node->parent = parent;

        currentStats.comparisons++;

        if (value < parent->value)
        {
            currentStats.pointerOperations++;
            parent->left = node;
        }
        else
        {
            currentStats.pointerOperations++;
            parent->right = node;
        }

        splay(node);
    }

    Node* predecessor(Node* node)
    {
        if (node == nullptr ||
            node->left == nullptr)
            return nullptr;

        currentStats.pointerOperations++;

        Node* current = node->left;

        while (current->right)
        {
            currentStats.pointerOperations++;
            current = current->right;
        }

        return current;
    }

    Node* successor(Node* node)
    {
        if (node == nullptr ||
            node->right == nullptr)
            return nullptr;

        currentStats.pointerOperations++;

        Node* current = node->right;

        while (current->left)
        {
            currentStats.pointerOperations++;
            current = current->left;
        }

        return current;
    }

    void deleteValue(int value)
{
    currentStats.reset();

    Node* node = root;
    Node* parent = nullptr;

    while (node)
    {
        currentStats.comparisons++;

        if (value == node->value)
            break;

        parent = node;

        currentStats.comparisons++;

        if (value < node->value)
        {
            currentStats.pointerOperations++;
            node = node->left;
        }
        else
        {
            currentStats.pointerOperations++;
            node = node->right;
        }
    }

    if (node == nullptr)
        return;

    Node* splayNode = parent;

    if (node->left && node->right)
    {
        Node* pred = node->left;

        while (pred->right)
        {
            currentStats.pointerOperations++;
            pred = pred->right;
        }

        node->value = pred->value;

        parent = node;
        node = node->left;

        while (node->right)
        {
            parent = node;
            node = node->right;
        }

        splayNode = parent;
    }

    Node* child = (node->left) ? node->left : node->right;

    if (child)
    {
        currentStats.pointerOperations++;
        child->parent = node->parent;
    }

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