/*******************************
* Feb 2018
* https://www.geeksforgeeks.org/segment-tree-set-1-sum-of-given-range/
* no template

* leetcode: 745
*****************************/
#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <cmath>
#include <iostream>

using namespace std;

//for summation
class segmentTree{
public:
  segmentTree(vector<int> & a){
    cout << "construct segment tree\n";
    arr = a;
    arr_size = a.size();
    int height = (int)(ceil(log2(arr_size))); 
    tree_size = 2 * pow(2, height) - 1;
    // cout << tree_size << endl;
    // st.clear();
    st.resize(tree_size);
    build_tree();
    // cout << arr << endl;
    // cout << st << endl;
  }
  //destructor
  
  int query(int qs, int qe)
  {
    if(qs > qe || qs < 0 || qe > arr_size-1)
      return 0; // wrong querry
    return query(0, arr_size-1, qs, qe, 0);
  }
  
  void update(int arr_loc, int new_value){
    if(arr_loc < 0 || arr_loc > arr_size - 1)
      return ; //wrong update
    
    int diff = new_value - arr[arr_loc];
    arr[arr_loc] = new_value;
    update(0, arr_size-1, arr_loc, 0, diff);

    cout << arr << endl;
    cout << st << endl;
  }
  
  void build_tree(){
    int sum = build_helper(0, arr_size-1, 0);
  }
  
private:
  int arr_size;
  int tree_size;
  vector<int> arr;
  vector<int> st;
  
  int getMid(int a, int b){
    return a + (b-a)/2;
  }
  
  int build_helper(int as, int ae, int sindex){
    if(as == ae) st[sindex] = arr[as];
    else{
      int mid = getMid(as, ae);
      int left = build_helper(as, mid, sindex * 2 + 1);
      int right = build_helper(mid+1, ae, sindex * 2 + 2);
      st[sindex] = left + right;
    } 
    // cout << "[" << sindex << "]" << " = " << st[sindex] << endl;  
    return st[sindex];
  }
  
  int query(int ss, int se, int qs, int qe, int index){
    if(qe < ss || qs > se) return 0;
    
    if(qs <= ss && qe >= se) return st[index];

  	int mid = getMid(ss, se);
  	int left = query(ss, mid, qs, qe, index * 2 + 1);
  	int right = query(mid+1, se, qs, qe, index*2 + 2);
  	return left + right;
    
  }
  
  void update(int ss, int se, int index, int st_index, int diff){
    if(index < ss || index > se) return; //not in the right range
    
    st[st_index] += diff; //update this node
    if(ss != se){
      int mid = getMid(ss, se);
      update(ss, mid, index, st_index * 2 + 1, diff);
      update(mid + 1, se, index, st_index * 2 + 2, diff);
    }
  }
  
};







class indexTree {
public:
  indexTree(vector<int>& a):it{new int[a.size()+1]}, sz{a.size()+1}, arr(a.size(), 0){ 
    cout << "construct indexed tree\n";
    fill(it, it+sz+1, 0);
    for(int i = 0; i < a.size(); ++i)
      update(i, a[i]);

    cout << "array: " << arr << endl;
    cout << "indexed tree: " ;
    for(int i  = 0; i < sz; ++i) 
      cout << it[i] << ' ';
    cout << endl;
  }
  ~indexTree(){
    delete [] it;
  }
   
  void update(int idx, int val){

    int diff = val - arr[idx];
    arr[idx] = val;

    idx += 1;
    cout << "update : " << idx << endl;
    while(idx < sz){
      it[idx] += diff;
      idx += last_set_one(idx);
    }     

    // cout << arr << endl; 
  }
  
  int sum(int l, int r){
    return sum(r) - (l==0? 0 : sum(l-1));
  }
  int sum(int idx){
    int sum_val = 0; 
    idx += 1;
    while(idx > 0){
      sum_val += it[idx];
      idx -= last_set_one(idx);
    }
    return sum_val;
  }
    
private:
  int * it;
  size_t sz;
  vector<int> arr;
  
  int last_set_one(int x){
    return (x & (-x)); 
  }
  
  
  
  
};
#endif
