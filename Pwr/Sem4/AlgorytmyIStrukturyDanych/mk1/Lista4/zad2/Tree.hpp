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
    int height;

    Node* left;
    Node* right;

    Node(int value)
        : value(value),
          height(1),
          left(nullptr),
          right(nullptr)
    {
    }
};

class Tree
{
public:
    Node* root;

    OperationStats currentStats;

    Tree() : root(nullptr) {}

    int getHeight(Node* node)
    {
        return node ? node->height : 0;
    }

    void updateHeight(Node* node)
    {
        if (node)
        {
            currentStats.pointerOperations += 2;

            node->height =
                1 + std::max(getHeight(node->left),
                             getHeight(node->right));
        }
    }

    void insert(int value)
    {
        currentStats.reset();
        insert(root, value);
    }

    void insert(Node*& node, int value)
    {
        if (node == nullptr)
        {
            currentStats.pointerOperations++;
            node = new Node(value);
            return;
        }

        currentStats.comparisons++;

        if (value < node->value)
        {
            currentStats.pointerOperations++;

            insert(node->left, value);
        }
        else
        {
            currentStats.pointerOperations++;

            insert(node->right, value);
        }

        updateHeight(node);
    }

    Node* predecessor(Node* node)
    {
        currentStats.pointerOperations++;

        if (node == nullptr || node->left == nullptr)
            return nullptr;

        currentStats.pointerOperations++;

        Node* current = node->left;

        while (true)
        {
            currentStats.pointerOperations++;

            if (current->right == nullptr)
                break;

            currentStats.pointerOperations++;

            current = current->right;
        }

        return current;
    }

    Node* successor(Node* node)
    {
        currentStats.pointerOperations++;

        if (node == nullptr || node->right == nullptr)
            return nullptr;

        currentStats.pointerOperations++;

        Node* current = node->right;

        while (true)
        {
            currentStats.pointerOperations++;

            if (current->left == nullptr)
                break;

            currentStats.pointerOperations ++;

            current = current->left;
        }

        return current;
    }

    void deleteValue(int value)
    {
        currentStats.reset();
        deleteValue(root, value);
    }

    void deleteValue(Node*& node, int value)
    {
        if (node == nullptr)
            return;

        currentStats.comparisons++;

        if (value < node->value)
        {
            currentStats.pointerOperations++;

            deleteValue(node->left, value);
        }
        else
        {
            currentStats.comparisons++;

            if (value > node->value)
            {
                currentStats.pointerOperations++;

                deleteValue(node->right, value);
            }
            else
            {
                currentStats.pointerOperations += 2;

                if (node->left == nullptr &&
                    node->right == nullptr)
                {
                    delete node;

                    currentStats.pointerOperations++;

                    node = nullptr;
                    return;
                }

                currentStats.pointerOperations++;

                if (node->left == nullptr)
                {
                    Node* temp = node;

                    currentStats.pointerOperations++;
                    currentStats.pointerOperations++;

                    node = node->right;

                    delete temp;
                }

                else
                {
                    currentStats.pointerOperations++;

                    if (node->right == nullptr)
                    {
                        Node* temp = node;

                        currentStats.pointerOperations++;
                        currentStats.pointerOperations++;

                        node = node->left;

                        delete temp;
                    }

                    else
                    {
                        Node* pred = predecessor(node);

                        node->value = pred->value;

                        currentStats.pointerOperations++;

                        deleteValue(node->left,
                                    pred->value);
                    }
                }
            }
        }

        if (node)
            updateHeight(node);
    }

    int height(Node* node)
    {
        return node ? node->height : 0;
    }

    int height()
    {
        return root ? root->height : 0;
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

        return isBST(node->left,
                     minVal,
                     node->value - 1)
            &&
               isBST(node->right,
                     node->value,
                     maxVal);
    }

    void printBST(Node* node,
                  std::string prefix = "",
                  bool isLeft = true)
    {
        if (node == nullptr)
            return;

        if (node->right)
        {
            printBST(node->right,
                     prefix + (isLeft ? "│   " : "    "),
                     false);
        }

        std::cout << prefix;

        if (!prefix.empty())
            std::cout << (isLeft ? "└── "
                                 : "┌── ");

        std::cout << node->value
                  << " (h=" << node->height
                  << ")"
                  << '\n';

        if (node->left)
        {
            printBST(node->left,
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