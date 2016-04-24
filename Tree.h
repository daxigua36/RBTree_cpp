#ifndef RBTREE_TREE_H
#define RBTREE_TREE_H

using namespace std;

template <typename T>
class Tree {
private:

    enum class Color : bool {BLACK = false, RED = true};

    class Node {
    public:
        int key;
        T value;
        Node * left;
        Node * right;
        Node * parent;
        Color color;

        Node(int key, T value) {
            this->key = key;
            this->value = value;
            this->color = Color::RED;
        }
    };

    Node * root;
    Node * nil_node;

    void rotateLeft(Node * node);
    void rotateRight(Node * node);
    void transplant(Node * destNode, Node * sourceNode);
    void insertFixup(Node * node);
    void deleteFixup(Node * node);

public:
    explicit Tree() {
        this->nil_node = new Node(0, 0);
        this->nil_node->color = Color::BLACK;
        this->nil_node->parent = this->nil_node;
        this->root = this->nil_node;
    }

    ~Tree() {
        delete this->root;
        delete this->nil_node;
    }

    Node * getTreeMinimum(Node * root);
    Node * getTreeMaximum(Node * root);
    Node * getNode(T value);
    void insert(T value);
    void remove(T value);
};

template <typename T>
typename Tree<T>::Node * Tree<T>::getNode(T value) {
    Node * node = root;
    while (node != this->nil_node && value != node->value) {
        if (value < node->value)
            node = node->left;
        else
            node = node->right;
    }
    return node;
}

template <typename T>
void Tree<T>::insert(T value) {
    Node * node = new Node(0,value);
    Node * y = this->nil_node;
    Node * x = this->root;
    while (x != this->nil_node) {
        y = x;
        if (node->value < x->value)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;
    if (y == this->nil_node)
        this->root = node;
    else if (node->value < y->value)
        y->left = node;
    else
        y->right = node;

    node->left = this->nil_node;
    node->right = this->nil_node;
    node->color = Color::RED;
    insertFixup(node);
}

template <typename T>
void Tree<T>::remove(T value) {
    Node * node = getNode(value);
    Node * temp = node;
    Node * temp2;
    Color origColor = temp->color;
    if (node->left == this->nil_node) {
        temp2 = node->right;
        transplant(node, node->right);
    } else if (node->right == this->nil_node) {
        temp2 = node->left;
        transplant(node, node->left);
    } else {
        temp = getTreeMinimum(node->right);
        origColor = temp->color;
        temp2 = temp->right;
        if (temp->parent == node) {
            temp2->parent = temp;
        } else {
            transplant(temp, temp->right);
            temp->right = node->right;
            temp->right->parent = temp;
        }
        transplant(node, temp);
        temp->left = node->left;
        temp->left->parent = temp;
        temp->color = node->color;
    }
    if (origColor == Color::BLACK)
        deleteFixup(temp2);
}

template <typename T>
typename Tree<T>::Node * Tree<T>::getTreeMinimum(Node * root) {
    while (root->left != this->nil_node)
        root = root->left;
    return root;
}

template <typename T>
typename Tree<T>::Node * Tree<T>::getTreeMaximum(Node * root) {
    while (root->right != this->nil_node)
        root = root->right;
    return root;
}

template <typename T>
void Tree<T>::rotateLeft(Node * node)  {
    Node * temp = node->right;
    node->right = temp->left;

    if (temp->left != this->nil_node)
        temp->left->parent = node;
    temp->parent = node->parent;

    if (node->parent == this->nil_node)
        this->root = temp;
    else if (node == node->parent->left)
        node->parent->left = temp;
    else
        node->parent->right = temp;
    temp->left = node;
    node->parent = temp;
}

template <typename T>
void Tree<T>::rotateRight(Node * node)  {
    Node * temp = node->left;
    node->left = temp->right;

    if (temp->right != this->nil_node)
        temp->right->parent = node;
    temp->parent = node->parent;

    if (node->parent == this->nil_node)
        this->root = temp;
    else if (node == node->parent->right)
        node->parent->right = temp;
    else
        node->parent->left = temp;
    temp->right = node;
    node->parent = temp;
}

template <typename T>
void Tree<T>::transplant(Node * destNode, Node *sourceNode)  {
    if (destNode->parent == this->nil_node)
        this->root = sourceNode;
    else if (destNode == destNode->parent->left)
        destNode->parent->left = sourceNode;
    else
        destNode->parent->right = sourceNode;
    sourceNode->parent = destNode->parent;
}

template <typename T>
void Tree<T>::insertFixup(Node * node)  {
    Node * temp;
    while (node->parent->color == Color::RED) {
        if (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;
            if (temp->color == Color::RED) {
                node->parent->color = Color::BLACK;
                temp->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rotateLeft(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateRight(node->parent->parent);
            }
        }
        else {
            temp = node->parent->parent->left;
            if (temp->color == Color::RED) {
                node->parent->color = Color::BLACK;
                temp->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rotateRight(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateLeft(node->parent->parent);
            }
        }
    }
    this->root->color = Color::BLACK;
}

template <typename T>
void Tree<T>::deleteFixup(Node * node) {
    Node * temp;
    while (node != this->root && node->color == Color::BLACK) {
        if (node == node->parent->left) {
            temp = node->parent->right;
            if (temp->color == Color::RED) {
                temp->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateLeft(node->parent);
                temp = node->parent->right;
            }
            if (temp->left->color == Color::BLACK && temp->right->color == Color::BLACK) {
                temp->color = Color::RED;
                node = node->parent;
            } else {
                if (temp->right->color == Color::BLACK) {
                    temp->left->color = Color::BLACK;
                    temp->color = Color::RED;
                    rotateRight(temp);
                    temp = node->parent->right;
                }
                temp->color = node->parent->color;
                node->parent->color = Color::BLACK;
                temp->right->color = Color::BLACK;
                rotateLeft(node->parent);
                node = this->root;
            }
        } else {
            temp = node->parent->left;
            if (temp->color == Color::RED) {
                temp->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateRight(node->parent);
                temp = node->parent->left;
            }
            if (temp->right->color == Color::BLACK && temp->left->color == Color::BLACK) {
                temp->color = Color::RED;
                node = node->parent;
            } else {
                if (temp->left->color == Color::BLACK) {
                    temp->right->color = Color::BLACK;
                    temp->color = Color::RED;
                    rotateLeft(temp);
                    temp = node->parent->left;
                }
                temp->color = node->parent->color;
                node->parent->color = Color::BLACK;
                temp->left->color = Color::BLACK;
                rotateRight(node->parent);
                node = this->root;
            }
        }
    }
    node->color = Color::BLACK;
}
#endif //RBTREE_TREE_H
