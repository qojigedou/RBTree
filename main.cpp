#include "rbtree.h"

int main() {

	RedBlackTree<double> mytree;
	std::cout << "RED-BLACK TREE IMPLEMENTATION\n";
	std::cout << "\n\nTREE AFTER INSERTION THE ELEMENTS:\n\n";
	
	mytree.insert(10);
	mytree.insert(18);
	mytree.insert(7);
	mytree.insert(15);
	mytree.insert(16);
	mytree.insert(30);
    mytree.insert(6);
    mytree.insert(13);

	mytree.pretty_print();


    mytree.delete_node(7);
	std::cout << "\n\n...meanwhile I deleted 7...\n";
	mytree.delete_node(13);
	std::cout << "...meanwhile I deleted 13...\n";
	mytree.delete_node(6);
	std::cout << "...meanwhile I deleted 6...\n";
	std::cout << "\n\nTREE AFTER DELETION THE ELEMENTS:\n\n";
    mytree.pretty_print();

	std::cout << "\ninorder :\n";
	mytree.inorder();
	std::cout << "\npostorder :\n";
	mytree.postorder();
	std::cout << "\npreorder :\n";
	mytree.preorder();
	return 0;
}