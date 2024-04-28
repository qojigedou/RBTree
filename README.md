### Red-Black Tree

Red-black tree - a type of self-organizing binary search tree - a data structure used in computer science most often to implement associative arrays.

Red-black trees ensure that their height does not exceed twice the minimum height.
This is done by coloring nodes red or black and applying after each insert or deletion the appropriate tree balancing procedure, so that the following conditions are met:

    1. Each node of the tree is either red or black.
    2. Each leaf of the tree (nil empty node) is always black.
    3. The root of the tree is always black.
    4. If a node is red, both of its sons are black.
    5. Every straight path from a given node to any of its descendant leaves contains the same number of black nodes.
    6. Only leaf node can be removed

Characterized by low computational complexity of elementary operations, such as.

- insertion (O(log(n))),
- search (O(log(n))),
- removing elements from the tree (O(log(n))).

## Pseudocode of some basic operations:

# Insertion

```
rbtree_insert(Tree, node) :
    bst_insert(Tree, node) // normal BST insertion
    while node.parent.color == RED
        if node.parent == node.parent.parent.right
            uncle = node.parent.parent.left // uncle is grandfather's left child
            if uncle.color == RED // parent is red and the uncle is red too
                uncle.color = BLACK
                node.parent.color = BLACK
                node.parent.parent.color = RED
                node = node.parent.parent
             else if node == node.parent.left // parent is right child of grandparent, node is left child of parent
                    node = node.parent
                    rotate_right(Tree, node)
                node.parent.color = BLACK
                node.parent.parent.color = RED
                rotate_left(Tree, node.parent.parent)
        else (same thing just swap "left" and "right" in place)
    Tree.root.color = BLACK
```

# Deletion:

```
rbtree_delete(Tree, node) :
    bst_delete(Tree, node) // normal deletion BST
    while node != Tree.root and node.color == BLACK
        if node == node.parent.left
            sibling = node.parent.right // node's brother
            if sibling.color == RED
                sibling.color = BLACK
                node.parent.color = RED
                rotate_left(Tree, node.parent)
                sibling = node.parent.right
            if sibling.left.color == BLACK and sibling.right.color == BLACK // if brother black and his children are black
                sibling.color = RED
                node = node.parent
            else if sibling.right.color == BLACK
                    sibling.left.color = BLACK
                    sibling.color = RED
                    rotate_right(Tree, sibling)
                    sibling = node.parent.right
                sibling.color = node.parent.right // node is black, his brother is black, brother's right child is red
                node.parent.color = BLACK
                sibling.right.color = BLACK
                rotate_left(Tree, node.parent)
                node = T.root
        else (same thing just swap "left" and "right" in place)
    node.color = BLACK
```

# Interface:

```
class RedBlackTree{
    RedBlackTree(); // constructor
    ~RedBlackTree(); // destructor
    bool empty() const; // check if the tree is empty
    T& top(); // function that returns the value of the tree root
    void clear(RBNode<T> *node); // function to clean the tree
    void preorder(); // function writing out the elements in the preorder
    void inorder(); // function that prints out the elements in the inorder
    void postorder(); // function that prints out the elements in postorder
    RBNode<T>* search(T item); // function to search for an element in a tree with a given value
    void insert(T item); // insert an element into the tree
    void delete_node(T value); // function to remove a node from the tree
    void pretty_print(); // function to visualize the tree
};
```

## Launching:

```
    make; ./main.out
```
