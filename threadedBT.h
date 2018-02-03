/*******************************
* Feb 2018
* https://www.geeksforgeeks.org/threaded-binary-tree-insertion/
* https://www.geeksforgeeks.org/convert-binary-tree-threaded-binary-tree/
  Convert a Binary Tree to Threaded binary tree | Set 1 (Using Queue)
*****************************/
#ifndef TREADED_H
#define TREADED_H 

using namespace std;

template < typename Comparable >
class ThreadedBST
{
	struct Node
	{
		Comparable value;
		Node * left, * right;
		bool lthread, rthread;
		Node(const Comparable v): value{v}, left{NULL}, right{NULL}, lthread{true}, rthread{true} { }

	};
	
public:
	ThreadedBST(){root = NULL;}
	~ThreadedBST(){
		remove(root);
	}
	//copy constructor, assignment construct

	Node * insert(const Comparable & key){
		if(root == NULL) {
			root = new Node(key);
			root->lthread = false;
			root->rthread = false; // no precedessor or ancestor
			// cout << "no root, only create a root\n";
			return root;
		}
		// cout << key << "   this time insert to root\n";
		return insert(root, key);
	}
	
	void inorder_print(){
		if(root == NULL)
			cout << "Tree is empty" << endl;
		else{
			cout << "Start to print in inorder:" << endl;
			Node *nd = root;
			
			while(!nd->lthread)
				nd = nd->left;

			while(nd){
				cout << nd->value << ' ';
				nd = inorderSuccessor(nd);
			}
		}
	}
	Node * inorderSuccessor(Node * ptr){
		if(ptr != NULL){
			if(ptr->rthread)
				return ptr->right;
			else{
				ptr = ptr->right;
				while(! ptr->lthread){
					ptr = ptr->left;
				}
			}
		}
		if(ptr == NULL) cout << "null next" << endl;
		return ptr;
	}

private:
	Node *root;
	Node * insert(Node * node, Comparable key){

		Node *nd = node;
		Node * par = nd;

		while(nd != NULL){ //!n doesn't work
			
			if(nd->value == key){
				cout << "duplicate key inserted!\n";
				return nd;
			}
			par = nd; // have parent
			if(key < nd->value){
				if(nd->lthread == false) //have left child
					nd = nd->left;
				else break; // find the spot
			}
			else {
				if( nd->rthread == false)
					nd = nd->right;
					// nd = nd->right;
				else 
					break;
			}
		}// find the spot and terminate loop

		// Node *tmp = new Node(key);
		Node *tmp = new Node(key); //nd is parent
		// if(!par) {// node == NULL, not possible after creating the root ahead
		// 	nd = tmp;
		// }else 
		if( key < par->value){
			// tmp->rthread = true; 
			tmp->right = par;
			tmp->left = par->left;
			par->lthread = false;
			par->left = tmp;
		}else{
			tmp->left = par;
			tmp->right = par->right;
			par->rthread = false;
			par->right = tmp;
		}
		return tmp;
	}
	
	void remove(Node *nd){
		if(nd == NULL) return;
		if(nd->lthread == false) remove(nd->left);
		if(nd->rthread == false) remove(nd->right);
		delete nd;
		nd = NULL;
	}

};


#endif