/*******************************
* Feb 2018
* https://www.geeksforgeeks.org/trie-insert-and-search/
* no template

* leetcode: 745
*****************************/
#ifndef TRIE_H
#define TRIE_H 

using namespace std;


class Trie
{
	static const size_t ALPHABET_SIZE = 26; // assume all lower case
	struct TrieNode
	{
		bool isEnd;
		TrieNode * children[ALPHABET_SIZE];
		TrieNode():isEnd(false){
			for(int i = 0; i < ALPHABET_SIZE; ++i)
				children[i] = NULL;
		}		
	};
public:
	Trie():root{new TrieNode()}{}
	~Trie(){clear();}
	bool insert(string key){
		return insert(root, key);
	}
	bool search(string key){
		return search(root, key);
	}
	bool deleteKey(string key){
		return deleteKey(root, key);
	}
	bool isLeaf(TrieNode* node){
		return node->isEnd == true;
	}

private:
	TrieNode *root;

	

	bool insert(TrieNode *root, string key){
		for(int i = 0; i < key.length(); ++i){
			int index = tolower(key[i]) - 'a';
			if(!root->children[index])
				root->children[index] = new TrieNode();
			root = root->children[index];
		}
		if(root->isEnd == true)
			return false;
		else 
			root->isEnd = true;
		return true;
	}
	bool search(TrieNode *root, string key){
		for(int i = 0; i < key.length(); ++i){
			int index = tolower(key[i]) - 'a';
			if(!root->children[index])
				return false;
			root = root->children[index];
		}
		return root != NULL && root->isEnd ;
		// return false;
	}
	bool deleteKey(TrieNode * node, string key){
		int len = key.length();
		if(len > 0)
			return delete_helper(node, key, 0, len);
		return false;
	}
	bool delete_helper(TrieNode * node, string key, int level, int len){
		if(!node) return false;

		if(level == len){
			if(isLeaf(node)){ // exist key
				node->isEnd = false;
				{
					if(isFreeNode(node)) //no children , then delete at caller
						return true;
					return false; // have children
				}
			}
			else return false; // not the end of string
		}
		else{
			int index = tolower(key[level]) - 'a';
			if(delete_helper(node->children[index], key, level + 1, len)){
				remove(node->children[index]);
				return (!isLeaf(node) && isFreeNode(node)); //  // recursively climb up, and delete eligible nodes
			}
			else return false;
		}
	}
	bool isFreeNode(TrieNode * node){
		if(!node) return false;

		for (int i = 0; i < ALPHABET_SIZE; ++i)
			if(node->children[i]) return false;
		return true;
	}

	void clear(){
		remove(root);
	}
	void remove(TrieNode * node){
		if(!node) return;
		for(int i = 0; i < ALPHABET_SIZE; ++i){
			remove(node->children[i]);
		}
		delete node;
		node = NULL;
	}

};


#endif