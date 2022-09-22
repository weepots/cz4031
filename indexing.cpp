#include <iostream>
#include <math.h>
using namespace std;

const int N = 3 ;


class Node{
    bool _leafNode;
    //list of keys
    int _key[N];
    //list of pointers to other nodes
    Node *_pointer[N+1];
    //pointer to next node, if any
    Node *_nextNode = NULL;
    //list of pointers to records(this will be empty if it is a leaf node)
    Record *_record[N];

    int _size = 0;

    friend class BPlusTree;

public:
    Node(){

    for(int i = 0 ;i<N;i++){
        _key[i]=NULL;
        _pointer[i]=NULL;
    }
    _pointer[N+1]=NULL;
    }
};

class BPlusTree{
    Node *_root = new Node;
    int _noOfNodes = 1 ;
    int _height = 1;
public:
    BPlusTree(){
    _root->_leafNode = true;
    }
    void insert(Record record){
        int nodeTrackerIndex = 0;
        Node *nodeTracker[_height];
        nodeTracker[nodeTrackerIndex] = _root;
        Node *newNode = new Node;
        while(!nodeTracker[nodeTrackerIndex]->_leafNode){
            for(int i=0;i<N;i++){
                if(nodeTracker[nodeTrackerIndex]->_key[i]!=NULL && nodeTracker[nodeTrackerIndex]->_key[i]>record.getValue()){
                    nodeTrackerIndex++;
                    nodeTracker[nodeTrackerIndex] = nodeTracker[nodeTrackerIndex-1]->_pointer[i];
                    break;
                }
                else if(nodeTracker[nodeTrackerIndex]->_key[i]==NULL){
                    nodeTrackerIndex+=1;
                    nodeTracker[nodeTrackerIndex] = nodeTracker[nodeTrackerIndex-1]->_pointer[i];

                break;
                }
                else if(nodeTracker[nodeTrackerIndex]->_key[i]!=NULL && nodeTracker[nodeTrackerIndex]->_key[i]<=record.getValue()&& i == N-1){
                    nodeTrackerIndex+=1;
                    nodeTracker[nodeTrackerIndex] = nodeTracker[nodeTrackerIndex-1]->_pointer[i+1];
                    break;
                }
            }
        }

        bool emptySpace = false;
        for(int i=0;i<N;i++){
            if(nodeTracker[nodeTrackerIndex]->_key[i] == NULL){
                emptySpace = true;
            }
        }
        //if(record.getValue() == 11)nodeTracker[nodeTrackerIndex]->printAll();
        if(emptySpace){
            int temp1,temp2;
            Record *r1,*r2,t;
            for(int i=0;i<N;i++){
                if(i==0 && nodeTracker[nodeTrackerIndex]->_key[i] == NULL){
                    nodeTracker[nodeTrackerIndex]->_key[i] = record.getValue();
                    nodeTracker[nodeTrackerIndex]->_record[i] = &record;
                    nodeTracker[nodeTrackerIndex]->_size++;
                    //printf(" i1 ");
                    break;
                }
                else if(nodeTracker[nodeTrackerIndex]->_key[i]>record.getValue()){
                    r1 = nodeTracker[nodeTrackerIndex]->_record[i];
                    temp1 = nodeTracker[nodeTrackerIndex]->_key[i];
                    nodeTracker[nodeTrackerIndex]->_record[i] = &record;
                    nodeTracker[nodeTrackerIndex]->_key[i] = record.getValue();
                    nodeTracker[nodeTrackerIndex]->_size++;
                    for(int j = i+1;j<N;j++){
                        r2 = nodeTracker[nodeTrackerIndex]->_record[j];
                        temp2 = nodeTracker[nodeTrackerIndex]->_key[j];
                        nodeTracker[nodeTrackerIndex]->_record[j] = r1;
                        nodeTracker[nodeTrackerIndex]->_key[j] = temp1;
                        r1 = r2;
                        temp1=temp2;
                        //cout<<"i2 "<<endl;

                    }

                    break;
                }
                else if(nodeTracker[nodeTrackerIndex]->_key[i] == NULL){
                    nodeTracker[nodeTrackerIndex]->_key[i] = record.getValue();
                    nodeTracker[nodeTrackerIndex]->_record[i] = &record;
                    nodeTracker[nodeTrackerIndex]->_size++;
                    //cout<<"i3 "<<endl;

                    break;
                }
            }

            while(nodeTrackerIndex!=0){
                nodeTrackerIndex--;
                Node *cursor;
                int minVal = 0;
                for(int i =0;i<N;i++){
                    if(nodeTracker[nodeTrackerIndex]->_key[i]==NULL)break;
                    cursor = nodeTracker[nodeTrackerIndex]->_pointer[i+1];
                    while(!cursor->_leafNode)cursor = cursor->_pointer[0];
                    nodeTracker[nodeTrackerIndex]->_key[i] = cursor->_key[0];
                }
            }
            _noOfNodes++;
            return;
        }
        else{
            int temp[N+1];
            Record *recordTemp[N+1];
            int floorVal = floor((N+1)/2);
            int ceilVal = ceil((N+1)/2);
            bool recordAdded = false;
            int keyIndex=0;
            for(int i=0;i<N+1;i++){
                if(!recordAdded){
                        if(keyIndex<N && nodeTracker[nodeTrackerIndex]->_key[keyIndex]<record.getValue()){
                            temp[i]=nodeTracker[nodeTrackerIndex]->_key[keyIndex];
                            recordTemp[i] = nodeTracker[nodeTrackerIndex]->_record[keyIndex];
                            keyIndex++;
                        }
                        else{
                            temp[i] = record.getValue();
                            recordTemp[i] = &record;
                            recordAdded = true;
                        }
                }
                else{
                    temp[i]=nodeTracker[nodeTrackerIndex]->_key[keyIndex];
                    recordTemp[i]=nodeTracker[nodeTrackerIndex]->_record[keyIndex];
                    keyIndex++;
                }

            }

            newNode->_leafNode = true;
            newNode->_nextNode = nodeTracker[nodeTrackerIndex]->_nextNode;
            nodeTracker[nodeTrackerIndex]->_nextNode = newNode;
            nodeTracker[nodeTrackerIndex]->_size = ceilVal;
            newNode->_size = floorVal;
            int tempIndex=0;
            for(int i=0;i<N;i++){
                if(i<ceilVal){
                nodeTracker[nodeTrackerIndex]->_key[i] = temp[tempIndex];
                nodeTracker[nodeTrackerIndex]->_record[i] = recordTemp[tempIndex];
                tempIndex++;
                }
                else{
                nodeTracker[nodeTrackerIndex]->_key[i] = NULL;
                nodeTracker[nodeTrackerIndex]->_record[i] = NULL;
                }
            }
            for(int i=0;i<N;i++){
                if(i<floorVal){
                newNode->_key[i] = temp[tempIndex];
                newNode->_record[i] = recordTemp[tempIndex];
                tempIndex++;
                }
                else{
                newNode->_key[i] = NULL;
                newNode->_record[i] = NULL;
                }
            }
        }

        if(nodeTrackerIndex==0 && !emptySpace){
            Node *internalNode = new Node;
            internalNode->_leafNode = false;
            internalNode->_pointer[0] = _root;
            internalNode->_pointer[1] = newNode;
            internalNode->_key[0] = internalNode->_pointer[1]->_key[0];
            internalNode->_size++;
            _root->_nextNode = newNode;
            _root = internalNode;
            _height++;
            _noOfNodes+=2;
            return;

        }

        Node *newInternalNode = newNode;
        bool newNodesToAdd = true;
        bool emptyPointer = false;

        while(nodeTrackerIndex !=0){
            if(newNodesToAdd){
            _noOfNodes++;
            for(int i=0;i<N+1;i++){
                if(nodeTracker[nodeTrackerIndex-1]->_pointer[i] == NULL){
                    emptyPointer = true;
                }
            }

            if(emptyPointer){
                for(int i=0;i<N+1;i++){
                    if(nodeTracker[nodeTrackerIndex-1]->_pointer[i] == nodeTracker[nodeTrackerIndex]){
                        for(int j=N;j>i;j--){
                        nodeTracker[nodeTrackerIndex-1]->_pointer[j] = nodeTracker[nodeTrackerIndex-1]->_pointer[j-1];
                        }
                        nodeTracker[nodeTrackerIndex-1]->_pointer[i+1] = newInternalNode;
                        nodeTracker[nodeTrackerIndex-1]->_size++;
                        break;

                    }
                }
                nodeTrackerIndex--;
                Node *cursor;
                for(int i =0;i<N;i++){
                    if(nodeTracker[nodeTrackerIndex]->_key[i]==NULL)break;
                    cursor = nodeTracker[nodeTrackerIndex]->_pointer[i+1];
                    while(!cursor->_leafNode)cursor = cursor->_pointer[0];
                    nodeTracker[nodeTrackerIndex]->_key[i] = cursor->_key[0];
                    nodeTracker[nodeTrackerIndex]->_size++;
                }
                _noOfNodes++;
                newNodesToAdd = false;
            }
            else{
            Node *temp[N+2];
            int floorVal = floor((N)/2);
            int ceilVal = ceil((N)/2)+1;
            bool pointerAdded = false;
            int pointerIndex=0;
            for(int i=0;i<N+3;i++){
                if(!pointerAdded){
                        if(pointerIndex<N+1 && nodeTracker[nodeTrackerIndex-1]->_pointer[pointerIndex] == nodeTracker[nodeTrackerIndex]){
                            temp[i]=nodeTracker[nodeTrackerIndex-1]->_pointer[pointerIndex];
                            temp[i+1] = newInternalNode;
                            i++;
                            pointerIndex+=1;
                            pointerAdded = true;
                        }
                        else{
                            temp[i] = nodeTracker[nodeTrackerIndex-1]->_pointer[pointerIndex];
                            pointerIndex++;
                        }
                }
                else{
                    temp[i]=nodeTracker[nodeTrackerIndex]->_pointer[pointerIndex];
                    pointerIndex++;
                }

            }

            Node *internalNode = new Node;
            internalNode->_leafNode = false;
            int tempIndex=0;
            nodeTracker[nodeTrackerIndex-1]->_size = ceilVal;
            internalNode->_size = floorVal;
            for(int i=0;i<N+1;i++){
                if(i<=ceilVal){
                nodeTracker[nodeTrackerIndex-1]->_pointer[i] = temp[tempIndex];
                tempIndex++;
                }
                else{
                nodeTracker[nodeTrackerIndex-1]->_pointer[i] = NULL;
                }
            }
            for(int i=0;i<N+1;i++){
                if(i<=floorVal){
                internalNode->_pointer[i] = temp[tempIndex];
                tempIndex++;
                }
                else{
                internalNode->_pointer[i] = NULL;
                }
            }
            nodeTrackerIndex--;
                Node *cursor;
                for(int i =0;i<N;i++){
                    if(nodeTracker[nodeTrackerIndex]->_pointer[i+1]==NULL){
                        nodeTracker[nodeTrackerIndex]->_key[i] = NULL;
                    }
                    else{
                    cursor = nodeTracker[nodeTrackerIndex]->_pointer[i+1];
                    while(!cursor->_leafNode)cursor = cursor->_pointer[0];
                    nodeTracker[nodeTrackerIndex]->_key[i] = cursor->_key[0];
                    }
                }
                for(int i =0;i<N;i++){
                    if(internalNode->_pointer[i+1]==NULL)break;
                    cursor = internalNode->_pointer[i+1];
                    while(!cursor->_leafNode)cursor = cursor->_pointer[0];
                    internalNode->_key[i] = cursor->_key[0];
                }
            newInternalNode = internalNode;
            }
        }
        else{
            nodeTrackerIndex--;
                Node *cursor;
                nodeTracker[nodeTrackerIndex]->_size = 0;
                for(int i =0;i<N;i++){
                    if(nodeTracker[nodeTrackerIndex]->_key[i]==NULL)break;
                    nodeTracker[nodeTrackerIndex]->_size++;
                    cursor = nodeTracker[nodeTrackerIndex]->_pointer[i+1];
                    while(!cursor->_leafNode)cursor = cursor->_pointer[0];
                    nodeTracker[nodeTrackerIndex]->_key[i] = cursor->_key[0];
                }
        }
        }

        if(newNodesToAdd){
                Node *newRootNode = new Node;
                newRootNode->_leafNode = false;
                newRootNode->_pointer[0] = _root;
                newRootNode->_pointer[1] = newInternalNode;
                newRootNode->_size++;
                _root = newRootNode;
                _height++;
                _noOfNodes++;

                Node *cursor;
                cursor = _root->_pointer[1];
                while(!cursor->_leafNode)cursor = cursor->_pointer[0];
                _root->_key[0] = cursor->_key[0];
        }
        else{
                Node *cursor;
                _root->_size = 0;
                for(int i =1;i<N+1;i++){
                if(_root->_pointer[i]==NULL){
                        _root->_key[i-1] = NULL;
                    }
                    else{
                            _root->_size++;
                    cursor = _root->_pointer[i];
                    while(!cursor->_leafNode)cursor = cursor->_pointer[0];
                    _root->_key[i-1] = cursor->_key[0];
                    }
                }
        }

    }
    Record* search(int value){
    int noOfNodes = 1,nodesPrinted = 1;
    Record *result;
    Node *cursor = _root;


    while(!cursor->_leafNode){
        if(nodesPrinted<=5){
        for(int i =0;i<N;i++){
            //if(cursor->_key[i] == NULL)break;
            printf("%d, ",cursor->_key[i]);
        }
        printf("\n");
        nodesPrinted++;
        }
        noOfNodes++;
        for(int i=0;i<N;i++){
            if(cursor->_key[i]!=NULL && cursor->_key[i]>value){
                cursor = cursor->_pointer[i];
                break;
            }
            else if(cursor->_key[i]==NULL){
                cursor = cursor->_pointer[i];

            break;
            }
            else if(cursor->_key[i]!=NULL && cursor->_key[i]<=value&& i == N-1){
                cursor = cursor->_pointer[i+1];
                break;
            }
        }
        }
                if(nodesPrinted<=5){
        for(int i =0;i<N;i++){
            //if(cursor->_key[i] == NULL)break;
            printf("%d, ",cursor->_key[i]);
            if(cursor->_key[i] == value){
                result = cursor->_record[i];
            }
        }
        }
        printf("\n");
        printf("No of nodes accessed:%d\n",noOfNodes);
        return result;

    }
    void remove(Record record){
    }

};
