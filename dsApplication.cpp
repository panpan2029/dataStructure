#include "../yuanHeadAll.h"
#include "../yuanFun.h"
#include "../CPUTimer.h"

using namespace std;

// 4.8.4 An Example That Uses Several Maps

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
	map<int, vector<string>> wordsByLength;
	for(auto & w : words) //group by length to avoid meaningless comparision between words with different length
		wordsByLength[w.length()].push_back(w);
	for(auto & entry : wordsByLength) //work on each length
	{
		const vector<string> & groupWords = entry.second;
		int groupWordLen = entry.first;
		
		for (int i = 0; i < groupWordLen; ++i) //work on each position; O(string_length)
		{
			map<string,vector<string>> repToWord;
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
					for(int p = 0; p < clique.size(); ++p)
						for(int q = p + 1; q < clique.size(); ++q)
						{
							adjWords[clique[p]].push_back(clique[q]);
							adjWords[clique[q]].push_back(clique[p]);
						}
			}
		}
	}
	
	return adjWords;
}

int main(){
	CPUTimer c_t;
	vector<string>  dict;
	string filename = "./google-10000-english-master/google-10000-english.txt";
	readDict(filename, dict);
	c_t.reset();
	map<string,vector<string>> res = computeAdjacentWords2(dict); //.6s
	// map<string,vector<string>> res = computeAdjacentWords3(dict); //.15s
	cout << "time comsumption: " << c_t.cur_CPUTime() << endl;

	for(auto & entry : res)
		cout << entry.first << ":\n " << entry.second << "\n\n";
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
}