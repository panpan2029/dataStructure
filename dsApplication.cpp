#include "../yuanHeadAll.h"
#include "../yuanFun.h"
#include "../CPUTimer.h"
#include "trie.h"
#include "threadedBT.h"
#include "segTree.h"
#include "unionFind.h"
using namespace std;


// 211. Add and Search Word - Data structure design 
// use multiple maps/sets

// 4.8.4 An Example That Uses Several Maps
// version1, using map; order doesn't matter, change to unordered_map

void readDict(string filename, vector<string> & dict){
	ifstream inf;
	inf.open(filename);
	if (inf.is_open()) {
		string word;
		// int iteration = 0; 
		while(inf >> word )//&& iteration < 100){
		{	
			dict.push_back(word);
			// cout << word << ' ';
			// ++iteration;
		}		
	}
	else cout << "open fail...\n";
	inf.close();

}
void printHighChangeables(const map<string, vector<string>> & adjacentWords, int minWords = 15 ){
	for(auto & entry : adjacentWords)
	{
		const vector<string> & words = entry.second;
		if(words.size() >= minWords)
		{
			cout << entry.first << '(' << words.size() << "):";
			for(auto & word : words)
				cout << " " << word;
			cout << endl;
		}
	}
}

bool oneCharOff(const string s1, const string s2){
	if(s1.length() != s2.length()) return false;

	int diffs = 0; 
	for(int i = 0; i < s1.length(); ++i)
		if(s1[i] != s2[i])
			if(++diffs > 1) return false;

	return diffs == 1;
}

//brute-force
//to improve: store the words in tries
//to use smarter method to push to adjWord
map<string,vector<string>> computeAdjacentWords( const vector<string> & words ){
	map<string, vector<string>> adjWords;
	for (int i = 0; i < words.size(); ++i)
	{
		for(int j = i + 1; j < words.size(); ++j)
		{
			if(oneCharOff(words[i], words[j]))
			{
				adjWords[words[i]].push_back(words[j]);
				adjWords[words[j]].push_back(words[i]);
			}
		}
	}
	return adjWords;
}
//still O(n^2), speed up by length
map<string,vector<string>> computeAdjacentWords2( const vector<string> & words ){
	map<string, vector<string>> adjWords;
	map<int, vector<string>> wordsByLength;
	for(auto & w : words)
		wordsByLength[w.length()].push_back(w);
	for(auto & entry : wordsByLength)
	{
		const vector<string> & groupByLength = entry.second;
		for (int i = 0; i < groupByLength.size(); ++i)
		{
			for(int j = i + 1; j < groupByLength.size(); ++j)
			{
				if(oneCharOff(groupByLength[i], groupByLength[j]))
				{
					adjWords[groupByLength[i]].push_back(groupByLength[j]);
					adjWords[groupByLength[j]].push_back(groupByLength[i]);
				}
			}
		}
	}
	
	return adjWords;
}
//use additional map
map<string,vector<string>> computeAdjacentWords3( const vector<string> & words ){
	map<string, vector<string>> adjWords;
	unordered_map<int, vector<string>> wordsByLength;
	for(auto & w : words) //group by length to avoid meaningless comparision between words with different length
		wordsByLength[w.length()].push_back(w);
	for(auto & entry : wordsByLength) //work on each length
	{
		const vector<string> & groupWords = entry.second;
		int groupWordLen = entry.first;
		
		for (int i = 0; i < groupWordLen; ++i) //work on each position; O(string_length)
		{
			unordered_map<string,vector<string>> repToWord;
			for(auto & word : groupWords) // each work, erase char at the same position i; to create repToWord map; O(#wordPerLen)
			{
				string rep = word;
				rep.erase(i,1);
				repToWord[rep].push_back(word);
			}
			for(auto & entry : repToWord)
			{
				const vector<string> & clique = entry.second;
				if(clique.size()>=2)
					// for(int p = 0; p < clique.size(); ++p) // here can't be saved seemingly
					// 	for(int q = p + 1; q < clique.size(); ++q)
					// 	{
					// 		adjWords[clique[p]].push_back(clique[q]);
					// 		adjWords[clique[q]].push_back(clique[p]);
					// 	}
					// version 2, time better 0.01
					for(auto itr = clique.begin(); itr != clique.end(); ++itr)
					{
						if(itr != clique.begin())
							adjWords[*itr].insert(adjWords[*itr].end(),clique.begin(), itr);
						if(itr != clique.end() - 1)
							adjWords[*itr].insert(adjWords[*itr].end(),itr+1, clique.end());
					}
			}
		}
	}
	
	return adjWords;
}

// stack
// prefix, postfix expression 
class Practice
{
public:
    // (a+b*c)+((d*e+f)*g) -> abc*+de*f+g*+
    // stack
    // without considering white space 
    string infix2postfix(string expr){
        stack<char> stkOperator;
        string res = "";
        unordered_map<char, int> map_precedence{{'*',1},{'/',1},{'+',0},{'-',0},{'(',-1},{')',-1}}; //() special case
        //assume all are binary operator
        int i = 0; 
        while(i < expr.size())
        {
            char c = expr[i];
            ++i;

            if(c == '(') { stkOperator.push(c); continue;}
            if(map_precedence.find(c) == map_precedence.end())
                res.push_back(c); //operand
            else{ //c is oeprator
                if(c == ')')
                {
                    while(!stkOperator.empty() && stkOperator.top()!='(')
                    {
                        res.push_back(stkOperator.top());
                        stkOperator.pop();
                    }
                    if(!stkOperator.empty()) stkOperator.pop();
                    //else error!, not pair parenthesis
                }                    
                else if(stkOperator.empty())
                {
                    stkOperator.push(c);
                }
                else{
                    while(!stkOperator.empty() && map_precedence[c] <= map_precedence[stkOperator.top()])
                    {
                        res.push_back(stkOperator.top());
                        stkOperator.pop();
                    }
                    stkOperator.push(c);
                }
            }
        }
        while(!stkOperator.empty())
        {
            res.push_back(stkOperator.top());
            stkOperator.pop();
        }
        return res;
    }
};



int main(){

	//test segment tree
	vector<int> v1{-1,0,1,2,3,-1};
	// segmentTree s{v1};
	indexTree it{v1};
	// cout << "segment tree: "<< s.query(0,3) << endl;
	// cout << "indexex tree: "<< it.sum(0,3) << endl;
	// s.update(1, 5);
	// it.update(1, 5);
	// cout << "segment tree: " << s.query(0,3) << endl;
	cout << "indexex tree: "<< it.sum(1,3) << endl;
	return 0;


	UnionFind un(5);
	//test tries
	vector<string> strs{"she", "sells", "sea", "shore", "the", "by", "sheer"};
	Trie trie;
	for(auto s : strs)
		trie.insert(s);
	// trie.deleteKey(strs[0]);
	trie.deleteKey(strs[5]);
	trie.deleteKey(strs[3]);
	trie.deleteKey(strs[2]);
	for(auto s : strs)
		cout << trie.search(s) << ' ';

	// cout << "main works\n";
	return 0;


	//test threaded BST
	ThreadedBST<int> tbst;
	vector<int> v10;
	for(int i  = 0; i < 10; ++i)
		v10.push_back(i);
	random_shuffle(v10.begin(), v10.end());
	cout << v10 << endl;

	for(int i : v10)
		tbst.insert(i);
	tbst.insert(2);
	tbst.insert(1);
	tbst.insert(3);
	tbst.insert(9);
	tbst.inorder_print();
	return 0;

	
	//load a dict and compute adjacent words(only one character different)
	CPUTimer c_t;
	vector<string>  dict;
	string filename = "./google-10000-english-master/google-10000-english.txt";
	readDict(filename, dict);
	c_t.reset();
	// map<string,vector<string>> res = computeAdjacentWords2(dict); //.6s
	map<string,vector<string>> res = computeAdjacentWords3(dict); //.15s, unordered map : 0.11s
	cout << "time comsumption: " << c_t.cur_CPUTime() << endl;

	// for(auto & entry : res)
	// 	cout << entry.first << ":\n " << entry.second << "\n\n";
	return 0;

	std::vector<char> v;
	if (FILE *fp = fopen("filename", "r"))
	{
		char buf[1024];
		while (size_t len = fread(buf, 1, sizeof(buf), fp))
			v.insert(v.end(), buf, buf + len);
		fclose(fp);
	}
	return 0;

	Practice pra;
    cout << pra.infix2postfix("(a+b*c)+((d*e+f)*g)") << endl;
    return 0;
    
}