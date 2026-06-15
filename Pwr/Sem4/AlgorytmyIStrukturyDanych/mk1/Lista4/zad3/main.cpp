#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <climits>

#include "SplayTree.hpp"

using namespace std;

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

void printState(Tree& tree)
{
    if (tree.root == nullptr)
    {
        cout << "(empty tree)\n";
    }
    else
    {
        tree.printBST(tree.root);
    }

    cout << "height = "
         << tree.height()
         << '\n';

    cout << "BST valid = "
         << boolalpha
         << isBST(tree.root,
                  INT_MIN,
                  INT_MAX)
         << "\n\n";
}

int main()
{
    constexpr int n = 30;

    random_device rd;
    mt19937 gen(rd());

    cout << "SCENARIO 1\n";
    cout << "Insert 1..30\n";
    cout << "Delete random permutation\n";

    Tree tree1;

    for (int i = 1; i <= n; i++)
    {
        cout << "insert "
             << i
             << "\n\n";

        tree1.insert(i);

        printState(tree1);
    }

    vector<int> deleteOrder1;

    for (int i = 1; i <= n; i++)
    {
        deleteOrder1.push_back(i);
    }

    shuffle(deleteOrder1.begin(),
            deleteOrder1.end(),
            gen);

    cout << "Deletion order:\n";

    for (int x : deleteOrder1)
    {
        cout << x << ' ';
    }

    cout << "\n\n";

    for (int x : deleteOrder1)
    {
        cout << "delete "
             << x
             << "\n\n";

        tree1.deleteValue(x);

        printState(tree1);
    }

    cout << "\n\n";
    cout << "SCENARIO 2\n";
    cout << "Insert random permutation\n";
    cout << "Delete random permutation\n";

    Tree tree2;

    vector<int> values;

    for (int i = 1; i <= n; i++)
    {
        values.push_back(i);
    }

    shuffle(values.begin(),
            values.end(),
            gen);

    cout << "Insertion order:\n";

    for (int x : values)
    {
        cout << x << ' ';
    }

    cout << "\n\n";

    for (int x : values)
    {
        cout << "insert "
             << x
             << "\n\n";

        tree2.insert(x);

        printState(tree2);
    }

    shuffle(values.begin(),
            values.end(),
            gen);

    cout << "Deletion order:\n";

    for (int x : values)
    {
        cout << x << ' ';
    }

    cout << "\n\n";

    for (int x : values)
    {
        cout << "delete "
             << x
             << "\n\n";

        tree2.deleteValue(x);

        printState(tree2);
    }

    return 0;
}