#ifndef UnionFind_H
#define UnionFind_H

#include <vector>
using namespace std;

class UnionFind
{
public:
	explicit UnionFind(int sz):s(sz,-1){}
	// ~UnionFind();
	void unionSet(int root1, int root2){//root1 and root2 is not in one union
		if(find(root1) == find(root2)) return;
		if(s[root1] < s[root2]){ // root1's height is deeper(negative)
			s[root2] = root1;
		}
		else { 
			if(s[root1] == s[root2]) // same deep
				--s[root1];
			s[root2] = root1;
		}
	}
	int find(int x) {
		if(s[x] < 0) return x; //self
		else return s[x] = find(s[x]);
	}

private:
	vector<int> s;
};
#endif