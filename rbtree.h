#include <iostream>
#include <cassert>
#include <string>
#include <iomanip>

enum Color
{
	RED,
	BLACK
};

template <typename T>
struct RBNode
{
	T value;
	RBNode *parent;
	RBNode *left;
	RBNode *right;
	Color color;
	RBNode() : value(T()), left(nullptr), right(nullptr), color(BLACK), parent(nullptr) {}
	RBNode(const T &item) : value(item), left(nullptr), right(nullptr), color(RED), parent(nullptr) {}
	~RBNode() {}
};

template <typename T>
class RedBlackTree
{
private:
	RBNode<T> *root;
	RBNode<T> *nill;
	void delete_node_help(RBNode<T> *node, T item);
	void preorder(RBNode<T> *node);
	void inorder(RBNode<T> *node);
	void postorder(RBNode<T> *node);
	void pretty_print_help(RBNode<T> *root, std::string level_control, bool islast);
	RBNode<T> *search_help(RBNode<T> *node, T item);
	void clear()
	{
		clear(root);
		root = nullptr;
	}
	void deletion_fix(RBNode<T> *node);
	void insertion_fix(RBNode<T> *node);
	virtual void visit(RBNode<T> *node)
	{
		assert(node != nullptr);
		std::cout << "visiting " << node->value << std::endl;
	}

	RBNode<T> *find_successor(RBNode<T> *node);
	RBNode<T> *find_predecessor(RBNode<T> *node);
	void rotate_left(RBNode<T> *node_to_rotate);
	void rotate_right(RBNode<T> *node_to_rotate);
	void transplant(RBNode<T> *node1, RBNode<T> *node2); // helps move subtrees within the rb tree
	RBNode<T> *find_min(RBNode<T> *node);
	RBNode<T> *find_max(RBNode<T> *node);
	RBNode<T> *get_root() { return root; }

public:
	RedBlackTree()
	{
		nill = new RBNode<T>;
		nill->left = nullptr;
		nill->right = nullptr;
		nill->color = BLACK;
		root = nill;
	}
	~RedBlackTree() { clear(); }

	bool empty() const { return root == nullptr; }
	T &top()
	{
		assert(root != nullptr);
		return root->value;
	}

	void clear(RBNode<T> *node);

	void preorder() { preorder(root); }
	void inorder() { inorder(root); }
	void postorder() { postorder(root); }

	RBNode<T> *search(T item) { return search_help(root, item); }

	void insert(T item);

	void delete_node(T value) { delete_node_help(root, value); }

	void pretty_print();
};

template <typename T>
void RedBlackTree<T>::delete_node_help(RBNode<T> *node, T item)
{
	RBNode<T> *found_node = nill;
	RBNode<T> *trans;
	RBNode<T> *temp;
	while (node != nill)
	{
		if (node->value == item)
		{
			found_node = node;
		}

		if (node->value <= item)
		{
			node = node->right;
		}
		else
		{
			node = node->left;
		}
	}

	if (found_node == nill)
	{
		std::cout << "Couldn't find item " << item << " in the tree" << std::endl;
		return;
	}

	temp = found_node;
	Color temp_original_color = temp->color;
	if (found_node->left == nill)
	{
		trans = found_node->right;
		transplant(found_node, found_node->right);
	}
	else if (found_node->right == nill)
	{
		trans = found_node->left;
		transplant(found_node, found_node->left);
	}
	else
	{
		temp = find_min(found_node->right);
		temp_original_color = temp->color;
		trans = temp->right;
		if (temp->parent == found_node)
		{
			trans->parent = temp;
		}
		else
		{
			transplant(temp, temp->right);
			temp->right = found_node->right;
			temp->right->parent = temp;
		}

		transplant(found_node, temp);
		temp->left = found_node->left;
		temp->left->parent = temp;
		temp->color = found_node->color;
	}
	delete found_node;
	if (temp_original_color == BLACK)
	{
		deletion_fix(trans);
	}
}

template <typename T>
void RedBlackTree<T>::pretty_print_help(RBNode<T> *root, std::string level_control, bool islast)
{
	if (root != nill)
	{
		std::cout << level_control;
		if (islast)
		{
			std::cout << "R->";
			level_control += "     ";
		}
		else
		{
			std::cout << "L->";
			level_control += "|    ";
		}

		std::string color;
		if (root->color == RED)
		{
			color = "RED";
		}
		else
		{
			color = "BLACK";
		}

		std::cout << root->value << "{" << color << "}" << std::endl;
		pretty_print_help(root->left, level_control, false);
		pretty_print_help(root->right, level_control, true);
	}
}

template <typename T>
void RedBlackTree<T>::insert(T item)
{
	RBNode<T> *node = new RBNode<T>;
	node->parent = nullptr;
	node->value = item;
	node->left = nill;
	node->right = nill;
	node->color = RED; // nowe wezel zawsze czerwony

	RBNode<T> *parent_node = nullptr;
	RBNode<T> *current = root;

	while (current != nill)
	{
		parent_node = current;
		if (node->value < current->value)
		{
			current = current->left;
		}
		else
		{
			current = current->right;
		}
	}

	node->parent = parent_node;
	if (parent_node == nullptr)
	{
		root = node;
	}
	else if (node->value < parent_node->value)
	{
		parent_node->left = node;
	}
	else
	{
		parent_node->right = node;
	}

	if (node->parent == nullptr)
	{ // jesli node to korzen dzrewa
		node->color = BLACK;
		return;
	}

	if (node->parent->parent == nullptr)
	{ // jesli dziadek null
		return;
	}

	insertion_fix(node);
}

template <typename T>
void RedBlackTree<T>::pretty_print()
{
	if (root)
	{
		pretty_print_help(this->root, "", true);
	}
}

template <typename T>
void RedBlackTree<T>::rotate_left(RBNode<T> *node_to_rotate)
{
	RBNode<T> *rotation_node = node_to_rotate->right;
	node_to_rotate->right = rotation_node->left;

	if (rotation_node->left != nill)
	{
		rotation_node->left->parent = node_to_rotate;
	}

	rotation_node->parent = node_to_rotate->parent;

	if (node_to_rotate->parent == nullptr)
	{
		root = rotation_node;
	}
	else if (node_to_rotate == node_to_rotate->parent->left)
	{
		node_to_rotate->parent->left = rotation_node;
	}
	else
	{
		node_to_rotate->parent->right = rotation_node;
	}

	rotation_node->left = node_to_rotate;
	node_to_rotate->parent = rotation_node;
}

template <typename T>
void RedBlackTree<T>::rotate_right(RBNode<T> *node_to_rotate)
{
	RBNode<T> *rotation_node = node_to_rotate->left;
	node_to_rotate->left = rotation_node->right;

	if (rotation_node->right != nill)
	{
		rotation_node->right->parent = node_to_rotate;
	}

	rotation_node->parent = node_to_rotate->parent;

	if (node_to_rotate->parent == nullptr)
	{
		root = rotation_node;
	}
	else if (node_to_rotate == node_to_rotate->parent->right)
	{
		node_to_rotate->parent->right = rotation_node;
	}
	else
	{
		node_to_rotate->parent->left = rotation_node;
	}
	rotation_node->right = node_to_rotate;
	node_to_rotate->parent = rotation_node;
}

template <typename T>
void RedBlackTree<T>::transplant(RBNode<T> *node1, RBNode<T> *node2)
{
	if (node1->parent == nullptr)
	{ // node1 is root
		root = node2;
	}
	else if (node1 == node1->parent->left)
	{ // node1 is a left child
		node1->parent->left = node2;
	}
	else
	{ // node1 is a right child
		node1->parent->right = node2;
	}
	node2->parent = node1->parent;
}

template <typename T>
void RedBlackTree<T>::clear(RBNode<T> *node)
{
	if (node == nullptr)
	{
		return;
	}
	if (node == nill)
	{
		return;
	}
	clear(node->left);
	clear(node->right);
	delete node;
	node = nullptr;
}

template <typename T>
RBNode<T> *RedBlackTree<T>::find_min(RBNode<T> *node)
{
	if (root == nullptr)
	{
		return nullptr;
	}
	while (node->left != nill)
	{
		node = node->left;
	}
	return node;
}

template <typename T>
RBNode<T> *RedBlackTree<T>::find_max(RBNode<T> *node)
{
	if (root == nullptr)
	{
		return nullptr;
	}
	while (node->right != nill)
	{
		node = node->right;
	}
	return node;
}

template <typename T>
void RedBlackTree<T>::insertion_fix(RBNode<T> *node)
{
	RBNode<T> *uncle;

	while (node->parent->color == RED)
	{
		if (node->parent == node->parent->parent->right)
		{
			// uncle is left child of grandparent
			uncle = node->parent->parent->left;
			if (uncle->color == RED)
			{
				// parent is red, uncle is red
				uncle->color = BLACK;
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->left)
				{
					// parent is right child of grandparent, node is left child of parent
					node = node->parent;
					rotate_right(node);
				}
				// parent is right child of grandparent, node is right child of parent
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				rotate_left(node->parent->parent);
			}
		}
		else
		{
			// uncle is right child of grandparent
			uncle = node->parent->parent->right;
			if (uncle->color == RED)
			{
				// parent is red, uncle is red
				uncle->color = BLACK;
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->right)
				{
					// parent is right child of grandparent, node is left child of parent
					node = node->parent;
					rotate_left(node);
				}
				// parent is right child of grandparent, node is right child of parent
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				rotate_right(node->parent->parent);
			}
		}
		if (node == root)
		{
			break;
		}
	}
	root->color = BLACK;
}

template <typename T>
RBNode<T> *RedBlackTree<T>::find_successor(RBNode<T> *node)
{
	if (node == nullptr)
	{
		return nullptr;
	}
	if (node->right != nill)
	{
		return find_min(node->right);
	}
	RBNode<T> *current = node;
	RBNode<T> *parent = current->parent;
	while (parent != nill && current == parent->right)
	{
		current = parent;
		parent = parent->parent;
	}
	return parent;
}

template <typename T>
RBNode<T> *RedBlackTree<T>::find_predecessor(RBNode<T> *node)
{
	if (node->left != nill)
	{
		return find_max(node->left);
	}
	RBNode<T> *current = node;
	RBNode<T> *parent = current->parent;
	while (parent != nill && current == parent->left)
	{
		current = parent;
		parent = parent->parent;
	}

	return parent;
}

template <typename T>
void RedBlackTree<T>::preorder(RBNode<T> *node)
{
	if (node != nill)
	{
		visit(node);
		preorder(node->left);
		preorder(node->right);
	}
}

template <typename T>
void RedBlackTree<T>::inorder(RBNode<T> *node)
{
	if (node != nill)
	{
		inorder(node->left);
		visit(node);
		inorder(node->right);
	}
}

template <typename T>
void RedBlackTree<T>::postorder(RBNode<T> *node)
{
	if (node != nill)
	{
		postorder(node->left);
		postorder(node->right);
		visit(node);
	}
}

template <typename T>
RBNode<T> *RedBlackTree<T>::search_help(RBNode<T> *node, T item)
{
	if (node == nill || item == node->value)
	{
		return node;
	}

	if (item < node->value)
	{
		return search_help(node->left, item);
	}

	if (item > node->value)
	{
		return search_help(node->right, item);
	}

	if (node == nullptr)
	{
		std::cout << "Can't find item " << item << " in the tree \n";
		return nullptr;
	}
}

template <typename T>
void RedBlackTree<T>::deletion_fix(RBNode<T> *node)
{
	RBNode<T> *sibling;
	while (node != root && node->color == BLACK)
	{
		if (node == node->parent->left)
		{
			sibling = node->parent->right;
			if (sibling->color == RED)
			{
				// sibling is red
				sibling->color = BLACK;
				node->parent->color = RED;
				rotate_left(node->parent);
				sibling = node->parent->right;
			}
			// sibling is black, sibling's children are black
			if (sibling->left->color == BLACK && sibling->right->color == BLACK)
			{
				sibling->color = RED;
				node = node->parent;
			}
			else
			{
				if (sibling->right->color == BLACK)
				{
					sibling->left->color = BLACK;
					sibling->color = RED;
					rotate_right(sibling);
					sibling = node->parent->right;
				}

				// sibling is black, sibling right child is red
				sibling->color = node->parent->color;
				node->parent->color = BLACK;
				sibling->right->color = BLACK;
				rotate_left(node->parent);
				node = root;
			}
		}
		else
		{
			sibling = node->parent->left;
			if (sibling->color == RED)
			{
				// sibling is red
				sibling->color = BLACK;
				node->parent->color = RED;
				rotate_right(node->parent);
				sibling = node->parent->left;
			}

			if (sibling->right->color == BLACK && sibling->right->color == BLACK)
			{
				// / sibling is black, sibling's children are black
				sibling->color = RED;
				node = node->parent;
			}
			else
			{
				if (sibling->left->color == BLACK)
				{
					sibling->right->color = BLACK;
					sibling->color = RED;
					rotate_left(sibling);
					sibling = node->parent->left;
				}

				// // sibling is black, sibling right child is red
				sibling->color = node->parent->color;
				node->parent->color = BLACK;
				sibling->left->color = BLACK;
				rotate_right(node->parent);
				node = root;
			}
		}
	}
	node->color = BLACK;
}
