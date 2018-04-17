#ifndef _HASH_H
#define _HASH_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>
#include "../yuanFun.h"
using namespace std;

// [] cuckoohashtable from weiss

/**
 * A hash routine for string objects.
 */
template<>
size_t hash(const string &key){
    size_t hashVal = 0;
    for(char ch : key)
        hashVal = 37 * hashVal + ch; // Horner's Rule [??]
    return hashVal;
}
template<>
size_t hash(int key){
    return key;
}
// SeparateChaining Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items

template <typename HashObj>
class HashTable_seprateChaining
{
private:
    vector<list<HashObj>> theLists;
    int currentSize;
    void rehash(){
        vector<list<HashObj>> oldLists = theLists; // copy?

        // Create new double-sized, empty table
        theLists.resize(nextPrime( 2 * theLists.size()));
        for( auto & thislist : theLists)
            thislist.clear();

        currentSize = 0;
        for(auto & thislist : oldLists)
            for( auto & x : thislist)
                insert(std::move(x)); // no need to delete old
                // currentSize++; //  insert calculate the currentSize
    }
    size_t myhash(const HashObj & x) const{
        static hash<HashObj> hf;
        return hf( x ) % theLists.size();
    }
public:
    explicit HashTable_seprateChaining(int size = 101) : currentSize(0)
    {theLists.resize(101) ; } // smallest
    // ~HashTable_seprateChaining();

    bool contains( const HashObj & x) const{
        auto & whichList = theLists[myhash(x)];
        return find(begin(whichList), end(whichList), x) != end(whichList);
    }
    bool insert( const HashObj & x){
        auto & whichList = theLists[myhash(x)];
        if(find(begin(whichList), end(whichList), x) != end(whichList))
            return false;
        whichList.push_back(x);

        if(++currentSize > theLists.size())
            rehash();
        return true;
    }
    bool remove( const HashObj & x){
        auto & whichList = theLists[myhash(x)];
        auto itr = find(begin(whichList), end(whichList), x));
        
        if(itr == end(whichList))
            return false;

        whichList.erase(itr);
        --currentSize;
        return true;
    }
};

// QuadraticProbing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
// int hashCode( string str ) --> Global method to hash strings

template <typename HashObj>
class HashTable_quadraticProbing
{
public:
    enum EntryType { ACTIVE, EMPTY, DELETED };
private:
    struct HashEntry
    {
        HashObj element;
        EntryType info;
        HashEntry( const HashObj & e = HashObj{}, EntryType i = EMPTY)
            : element{e}, info{i} { }
        HashEntry( const HashObj && e, EntryType i = EMPTY)
            : element { std::move(e)}, info{i} { }
    };

    vector<HashEntry> array;
    int currentSize;

    bool isActive(int currentPos) const
    { return array[currentPos].info == ACTIVE; }

    int findPose(const HashObj & x ) const{
        int offset = 1;
        int currentPos = myhash(x);

        while(array[currentPos].info != EMPTY && array[currentPos].element != x){
            currentPos += offset;
            offset += 2; // f(i) = f(i-1) + (2i - 1);
            if(currentPos >= array.size())
                currentPos -= array.size();
        }
        return currentPos;
    }

    void rehash()
    {
        vector<HashEntry> oldArray = array;

            // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( auto & entry : array )
            entry.info = EMPTY;

            // Copy table over
        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.info == ACTIVE )
                insert( std::move( entry.element ) );
    }

    size_t myhash( const HashObj & x) const{
        static hash<HashObj> hf;
        return hf(x) % array.size(); // cuz, in findPose, calculate bases on the VALID position
    }
public:
    // enum EntryType { ACTIVE, EMPTY, DELETED };

    explicit HashTable_quadraticProbing(int size = 101)
        : array( nextPrime(size) )
        { makeEmpty();}
    // ~HashTable_quadraticProbing();

    bool contains( const HashObj & x) const 
    { return isActive(findPose(x)); }

    void makeEmpty(){
        currentSize = 0;
        for(auto & entry : array)
            entry.info = EMPTY;
    }

    bool insert( const HashObj & x){
        int currentPos = findPose(x);
        if(isActive(currentPos)) // x exists already
            return false;
        if(array[ currentPos ].info != DELETED)
            ++currentSize;
        array[ currentPos ].element = x;
        array[ currentPos ].info = ACTIVE;

        if(currentSize > array.size() / 2)
            rehash();

        return true;
    }
    bool insert( const HashObj && x){
        int currentPos = findPose(x);
        if(isActive(currentPos)) // x exists already
            return false;
        if(array[ currentPos ].info != DELETED)
            ++currentSize;
        array[ currentPos ].element = std::move( x );
        array[ currentPos ].info = ACTIVE;

        if(currentSize > array.size() / 2)
            rehash();

        return true;
    }

    bool remove( const HashObj & x){
        int currentPos = findPose(x);
        if(!isActive(currentPos))
            return false; // x not exists or already delete

        array[currentPos].info = DELETED;
        return true;
    }
};


#endif
