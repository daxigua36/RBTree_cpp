#include <iostream>
#include "Tree.h"

using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    Tree<int> * tree = new Tree<int>();
    tree->insert(3);
    tree->insert(5);
    tree->insert(7);
    tree->remove(3);
    cout << tree->getNode(5)->value << endl;
    return 0;
}