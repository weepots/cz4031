#include <iostream>
using namespace std;

const int N = 3 ;

class Node{
    bool _leafNode;
    //list of keys
    int _key[N];
    //list of pointers to other nodes
    Node *_pointer[N];
    //pointer to next node, if any
    Node *_nextNode = NULL;
    //list of pointers to records(this will be empty if it is a leaf node)
    Record *_record[N];

    friend class BPlusTree;

public:
    Node(){}
};

class BPlusTree{
    Node _root;
    int _noOfNodes = 0 ;
    int _height = 1;
public:
    BPTree(){
    _root._leafNode = true;
    }
    void insert(Record record){
    }
    void remove(Record record){
    }

};
