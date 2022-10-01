#include <iostream>
#include <math.h>
#include "storage.h"
#include <vector>
#include <queue>
using namespace std;

const int N = 3;
const int MIN_KEYS_LEAF = (N + 1) / 2;
const int MIN_KEYS_INTERNAL = N / 2;
const int NUM_NODES_TO_DISPLAY = 5;

// g++ -o main.exe indexing.cpp main.cpp
// g++ -o main.exe storage.h storage.cpp indexing.cpp main.cpp

class Node
{
    bool _leafNode;
    // list of keys
    int *_key;
    // list of pointers to other nodes
    Node **_pointer;
    // pointer to next node, if any
    Node *_nextNode = NULL;
    // list of pointers to records(this will be empty if it is a leaf node)
    std::vector<Address *> *_record; // pointer to list of vectors containing pointers to records
    // Record **_record;
    // actual number of keys
    int _size = 0;

    friend class BPlusTree;

public:
    Node()
    {
        this->_key = new int[N];
        _pointer = new Node *[N + 1];
        _record = new vector<Address *>[N];
        // printf("%d %d %d %d %d %d\n", sizeof(_leafNode), sizeof(_key), sizeof(_pointer), sizeof(_nextNode), sizeof(_record), sizeof(_size));
        //  _record = new Record *[N];

        for (int i = 0; i < N; i++)
        {
            _key[i] = NULL;
            _pointer[i] = NULL;
        }
        _pointer[N] = NULL;
    }

    ~Node()
    {
        delete[] _key;
        delete[] _pointer;
        delete[] _record;
    }
};

class BPlusTree
{
    Node *_root;
    int _noOfNodes = 0;
    int _height = 0;

public:
    BPlusTree()
    {
        _root = NULL;
    }
    Node getRoot() { return *_root; }

    int getNumNodes()
    {
        return _noOfNodes;
    }

    void insert(Address *record)
    {
        if (_noOfNodes == 0)
        {
            _root = new Node;
            _root->_leafNode = true;
            _noOfNodes++;
            _height++;
        }

        int nodeTrackerIndex = 0;
        Node *nodeTracker[_height];
        nodeTracker[nodeTrackerIndex] = _root;
        Node *newNode = new Node;
        while (!nodeTracker[nodeTrackerIndex]->_leafNode)
        {
            for (int i = 0; i < N; i++)
            {
                if (nodeTracker[nodeTrackerIndex]->_key[i] != NULL && nodeTracker[nodeTrackerIndex]->_key[i] > accessNumVotes(record))
                {
                    nodeTrackerIndex++;
                    nodeTracker[nodeTrackerIndex] = nodeTracker[nodeTrackerIndex - 1]->_pointer[i];
                    break;
                }
                else if (nodeTracker[nodeTrackerIndex]->_key[i] == NULL)
                {
                    nodeTrackerIndex += 1;
                    nodeTracker[nodeTrackerIndex] = nodeTracker[nodeTrackerIndex - 1]->_pointer[i];

                    break;
                }
                else if (nodeTracker[nodeTrackerIndex]->_key[i] != NULL && nodeTracker[nodeTrackerIndex]->_key[i] <= accessNumVotes(record) && i == N - 1)
                {
                    nodeTrackerIndex += 1;
                    nodeTracker[nodeTrackerIndex] = nodeTracker[nodeTrackerIndex - 1]->_pointer[i + 1];
                    break;
                }
            }
        }

        bool emptySpace = false;
        for (int i = 0; i < N; i++)
        {
            if (nodeTracker[nodeTrackerIndex]->_key[i] == NULL)
            {
                emptySpace = true;
            }
            if (nodeTracker[nodeTrackerIndex]->_key[i] == accessNumVotes(record))
            {
                (nodeTracker[nodeTrackerIndex]->_record[i]).push_back(record);
                return;
            }
        }

        if (emptySpace)
        {
            int temp1, temp2;
            std::vector<Address *> r1, r2, t;
            for (int i = 0; i < N; i++)
            {
                if (i == 0 && nodeTracker[nodeTrackerIndex]->_key[i] == NULL)
                {
                    nodeTracker[nodeTrackerIndex]->_key[i] = accessNumVotes(record);
                    (nodeTracker[nodeTrackerIndex]->_record[i]).push_back(record);
                    // printf("%d ",nodeTracker[nodeTrackerIndex]->_record[i]->getValue());
                    nodeTracker[nodeTrackerIndex]->_size++;
                    // printf(" i1 ");
                    break;
                }
                else if (nodeTracker[nodeTrackerIndex]->_key[i] > accessNumVotes(record))
                {
                    r1 = nodeTracker[nodeTrackerIndex]->_record[i];
                    temp1 = nodeTracker[nodeTrackerIndex]->_key[i];
                    vector<Address *> newVector;
                    nodeTracker[nodeTrackerIndex]->_record[i] = newVector;
                    (nodeTracker[nodeTrackerIndex]->_record[i]).push_back(record);
                    nodeTracker[nodeTrackerIndex]->_key[i] = accessNumVotes(record);
                    nodeTracker[nodeTrackerIndex]->_size++;
                    for (int j = i + 1; j < N; j++)
                    {
                        r2 = nodeTracker[nodeTrackerIndex]->_record[j];
                        temp2 = nodeTracker[nodeTrackerIndex]->_key[j];
                        nodeTracker[nodeTrackerIndex]->_record[j] = r1;
                        nodeTracker[nodeTrackerIndex]->_key[j] = temp1;
                        r1 = r2;
                        temp1 = temp2;
                        // cout<<"i2 "<<endl;
                    }

                    break;
                }
                else if (nodeTracker[nodeTrackerIndex]->_key[i] == NULL)
                {
                    nodeTracker[nodeTrackerIndex]->_key[i] = accessNumVotes(record);
                    (nodeTracker[nodeTrackerIndex]->_record[i]).clear();
                    (nodeTracker[nodeTrackerIndex]->_record[i]).push_back(record);
                    nodeTracker[nodeTrackerIndex]->_size++;
                    // cout<<"i3 "<<endl;

                    break;
                }
            }

            while (nodeTrackerIndex != 0)
            {
                nodeTrackerIndex--;
                Node *cursor;
                int minVal = 0;
                for (int i = 0; i < N; i++)
                {
                    if (nodeTracker[nodeTrackerIndex]->_key[i] == NULL)
                        break;
                    cursor = nodeTracker[nodeTrackerIndex]->_pointer[i + 1];
                    while (!cursor->_leafNode)
                        cursor = cursor->_pointer[0];
                    nodeTracker[nodeTrackerIndex]->_key[i] = cursor->_key[0];
                }
            }
            //_noOfNodes++;
            // printf("record value before return:%d ",nodeTracker[nodeTrackerIndex]->_record[0]->getValue());
            return;
        }
        else
        {
            int temp[N + 1];
            std::vector<Address *> recordTemp[N + 1];
            int floorVal = floor((N + 1) / 2);
            int ceilVal = ceil((N + 1) / 2);
            bool recordAdded = false;
            int keyIndex = 0;
            for (int i = 0; i < N + 1; i++)
            {
                if (!recordAdded)
                {
                    if (keyIndex < N && nodeTracker[nodeTrackerIndex]->_key[keyIndex] < accessNumVotes(record))
                    {
                        temp[i] = nodeTracker[nodeTrackerIndex]->_key[keyIndex];
                        recordTemp[i] = nodeTracker[nodeTrackerIndex]->_record[keyIndex];
                        keyIndex++;
                    }
                    else
                    {
                        temp[i] = accessNumVotes(record);
                        (recordTemp[i]).push_back(record);
                        recordAdded = true;
                    }
                }
                else
                {
                    temp[i] = nodeTracker[nodeTrackerIndex]->_key[keyIndex];
                    recordTemp[i] = nodeTracker[nodeTrackerIndex]->_record[keyIndex];
                    keyIndex++;
                }
            }

            newNode->_leafNode = true;
            newNode->_nextNode = nodeTracker[nodeTrackerIndex]->_nextNode;
            nodeTracker[nodeTrackerIndex]->_nextNode = newNode;
            nodeTracker[nodeTrackerIndex]->_size = ceilVal;
            newNode->_size = floorVal;
            int tempIndex = 0;
            for (int i = 0; i < N; i++)
            {
                if (i < ceilVal)
                {
                    nodeTracker[nodeTrackerIndex]->_key[i] = temp[tempIndex];
                    nodeTracker[nodeTrackerIndex]->_record[i] = recordTemp[tempIndex];
                    tempIndex++;
                }
                else
                {
                    nodeTracker[nodeTrackerIndex]->_key[i] = NULL;
                    // nodeTracker[nodeTrackerIndex]->_record[i] = NULL;
                }
            }
            for (int i = 0; i < N; i++)
            {
                if (i < floorVal)
                {
                    newNode->_key[i] = temp[tempIndex];
                    newNode->_record[i] = recordTemp[tempIndex];
                    tempIndex++;
                }
                else
                {
                    newNode->_key[i] = NULL;
                    // newNode->_record[i] = NULL;
                }
            }
        }

        if (nodeTrackerIndex == 0 && !emptySpace)
        {
            Node *internalNode = new Node;
            internalNode->_leafNode = false;
            internalNode->_pointer[0] = _root;
            internalNode->_pointer[1] = newNode;
            internalNode->_key[0] = internalNode->_pointer[1]->_key[0];
            internalNode->_size++;
            _root->_nextNode = newNode;
            _root = internalNode;
            _height++;
            _noOfNodes += 2;
            return;
        }

        Node *newInternalNode = newNode;
        bool newNodesToAdd = true;
        bool emptyPointer = false;

        while (nodeTrackerIndex != 0)
        {
            if (newNodesToAdd)
            {
                _noOfNodes++;
                for (int i = 0; i < N + 1; i++)
                {
                    if (nodeTracker[nodeTrackerIndex - 1]->_pointer[i] == NULL)
                    {
                        emptyPointer = true;
                    }
                }

                if (emptyPointer)
                {
                    for (int i = 0; i < N + 1; i++)
                    {
                        if (nodeTracker[nodeTrackerIndex - 1]->_pointer[i] == nodeTracker[nodeTrackerIndex])
                        {
                            for (int j = N; j > i; j--)
                            {
                                nodeTracker[nodeTrackerIndex - 1]->_pointer[j] = nodeTracker[nodeTrackerIndex - 1]->_pointer[j - 1];
                            }
                            nodeTracker[nodeTrackerIndex - 1]->_pointer[i + 1] = newInternalNode;
                            nodeTracker[nodeTrackerIndex - 1]->_size++;
                            break;
                        }
                    }
                    nodeTrackerIndex--;
                    Node *cursor;
                    for (int i = 0; i < N; i++)
                    {
                        if (nodeTracker[nodeTrackerIndex]->_key[i] == NULL)
                            break;
                        cursor = nodeTracker[nodeTrackerIndex]->_pointer[i + 1];
                        while (!cursor->_leafNode)
                            cursor = cursor->_pointer[0];
                        nodeTracker[nodeTrackerIndex]->_key[i] = cursor->_key[0];
                        nodeTracker[nodeTrackerIndex]->_size++;
                    }
                    //_noOfNodes++;
                    newNodesToAdd = false;
                }
                else
                {
                    Node *temp[N + 2];
                    int floorVal = floor((N) / 2);
                    int ceilVal = ceil((N) / 2) + 1;
                    bool pointerAdded = false;
                    int pointerIndex = 0;

                    for (int i = 0; i < N + 3; i++)
                    {
                        if (!pointerAdded)
                        {
                            if (pointerIndex < N + 1 && nodeTracker[nodeTrackerIndex - 1]->_pointer[pointerIndex] == nodeTracker[nodeTrackerIndex])
                            {
                                temp[i] = nodeTracker[nodeTrackerIndex - 1]->_pointer[pointerIndex];
                                temp[i + 1] = newInternalNode;
                                i++;
                                pointerIndex += 1;
                                pointerAdded = true;
                            }
                            else
                            {
                                temp[i] = nodeTracker[nodeTrackerIndex - 1]->_pointer[pointerIndex];
                                pointerIndex++;
                            }
                        }
                        else
                        {
                            temp[i] = nodeTracker[nodeTrackerIndex - 1]->_pointer[pointerIndex];
                            pointerIndex++;
                        }
                    }

                    Node *internalNode = new Node;
                    internalNode->_leafNode = false;
                    int tempIndex = 0;
                    nodeTracker[nodeTrackerIndex - 1]->_size = ceilVal;
                    internalNode->_size = floorVal;

                    for (int i = 0; i < N + 1; i++)
                    {
                        if (i <= ceilVal)
                        {
                            nodeTracker[nodeTrackerIndex - 1]->_pointer[i] = temp[tempIndex];
                            tempIndex++;
                        }
                        else
                        {
                            nodeTracker[nodeTrackerIndex - 1]->_pointer[i] = NULL;
                        }
                    }

                    for (int i = 0; i < N + 1; i++)
                    {
                        if (i <= floorVal)
                        {
                            internalNode->_pointer[i] = temp[tempIndex];
                            tempIndex++;
                        }
                        else
                        {
                            internalNode->_pointer[i] = NULL;
                        }
                    }

                    nodeTrackerIndex--;
                    Node *cursor;

                    for (int i = 0; i < N; i++)
                    {
                        if (nodeTracker[nodeTrackerIndex]->_pointer[i + 1] == NULL)
                        {
                            nodeTracker[nodeTrackerIndex]->_key[i] = NULL;
                        }
                        else
                        {
                            cursor = nodeTracker[nodeTrackerIndex]->_pointer[i + 1];
                            while (!cursor->_leafNode)
                                cursor = cursor->_pointer[0];
                            nodeTracker[nodeTrackerIndex]->_key[i] = cursor->_key[0];
                        }
                    }

                    for (int i = 0; i < N; i++)
                    {
                        if (internalNode->_pointer[i + 1] == NULL)
                        {
                            internalNode->_key[i] = NULL;
                        }
                        else
                        {
                            cursor = internalNode->_pointer[i + 1];
                            while (!cursor->_leafNode)
                            {
                                cursor = cursor->_pointer[0];
                            }
                            internalNode->_key[i] = cursor->_key[0];
                        }
                    }

                    newInternalNode = internalNode;
                }
            }
            else
            {

                Node *cursor;
                nodeTracker[nodeTrackerIndex]->_size = 0;
                for (int i = 0; i < N; i++)
                {

                    if (nodeTracker[nodeTrackerIndex]->_pointer[i + 1] == NULL)
                    {
                        nodeTracker[nodeTrackerIndex]->_key[i] = NULL;
                    }
                    else
                    {
                        nodeTracker[nodeTrackerIndex]->_size++;
                        cursor = nodeTracker[nodeTrackerIndex]->_pointer[i + 1];
                        while (!cursor->_leafNode)
                            cursor = cursor->_pointer[0];
                        nodeTracker[nodeTrackerIndex]->_key[i] = cursor->_key[0];
                    }
                }
                nodeTrackerIndex--;
            }
        }

        if (newNodesToAdd)
        {
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
            while (!cursor->_leafNode)
                cursor = cursor->_pointer[0];
            _root->_key[0] = cursor->_key[0];
        }
        else
        {
            Node *cursor;
            _root->_size = 0;
            for (int i = 1; i < N + 1; i++)
            {
                if (_root->_pointer[i] == NULL)
                {
                    _root->_key[i - 1] = NULL;
                }
                else
                {
                    _root->_size++;
                    cursor = _root->_pointer[i];
                    while (!cursor->_leafNode)
                        cursor = cursor->_pointer[0];
                    _root->_key[i - 1] = cursor->_key[0];
                }
            }
        }
    }

    Address *search(int value)
    {
        int noOfNodes = 1, nodesPrinted = 1;
        Address *result;
        Node *cursor = _root;

        while (!cursor->_leafNode)
        {
            if (nodesPrinted <= 5)
            {
                for (int i = 0; i < N; i++)
                {
                    // if(cursor->_key[i] == NULL)break;
                    printf("%d, ", cursor->_key[i]);
                }
                printf("\n");
                nodesPrinted++;
            }
            noOfNodes++;
            for (int i = 0; i < N; i++)
            {
                if (cursor->_key[i] != NULL && cursor->_key[i] > value)
                {
                    cursor = cursor->_pointer[i];
                    break;
                }
                else if (cursor->_key[i] == NULL)
                {
                    cursor = cursor->_pointer[i];

                    break;
                }
                else if (cursor->_key[i] != NULL && cursor->_key[i] <= value && i == N - 1)
                {
                    cursor = cursor->_pointer[i + 1];
                    break;
                }
            }
        }
        if (nodesPrinted <= 5)
        {
            for (int i = 0; i < N; i++)
            {
                // if(cursor->_key[i] == NULL)break;
                printf("%d, ", cursor->_key[i]);
                if (cursor->_key[i] == value)
                {
                    result = cursor->_record[i].front();
                }
            }
        }
        printf("\n");
        printf("No of nodes accessed:%d\n", noOfNodes);
        return result;
    }

    // removes first record with matched key from b+ tree, updates b+ tree, returns address of record removed
    // record is not actually removed from memory yet. returns NULL if no record with given key
    // numNodesDeleted will iterate if node is deleted when removing key
    Address *remove(int key, int &numNodesDeleted)
    {
        Address *removedRecord = NULL;

        if (_root == NULL)
        {
            // cout << "tree is empty" << endl;
            return removedRecord;
        }

        struct relatedNodes nodes = getLeafNode(key);
        Node *leafNode = nodes.node;
        Node *parentNode = nodes.parentNode;
        Node *leftSiblingNode = nodes.leftSiblingNode;
        Node *rightSiblingNode = nodes.rightSiblingNode;
        int leafNodeIndex = nodes.nodeIndex;
        int leftSiblingIndex = nodes.leftSiblingIndex;
        int rightSiblingIndex = nodes.rightSiblingIndex;
        int keyIndex = nodes.keyIndex;

        if (leafNode == NULL)
        {
            // cout << "key not found" << endl;
            return removedRecord;
        }

        // delete the key at leaf node (check if underflow -> can borrow from sibling, cannot borrow from sibling), update parents recursively

        // check vector: if points to >1 record, just remove duplicate. if point to 1 record, delete key
        if (leafNode->_record[keyIndex].size() > 1)
        {
            // duplicate key, no need to remove key in B+ tree, just remove duplicate
            removedRecord = (leafNode->_record[keyIndex]).front();
            leafNode->_record[keyIndex].erase(leafNode->_record[keyIndex].begin()); // remove first record

            return removedRecord;
        }
        else
        {
            // delete key at leaf node
            removedRecord = leafNode->_record[keyIndex].front();
            for (int i = keyIndex; i < leafNode->_size - 1; i++)
            {
                leafNode->_key[i] = leafNode->_key[i + 1];
                leafNode->_record[i] = leafNode->_record[i + 1];
            }
            leafNode->_size--;
            leafNode->_key[leafNode->_size] = NULL;
            leafNode->_record[leafNode->_size].clear();
        }

        // if only 1 level tree, and last key deleted, entire tree is deleted
        if (leafNode == _root && leafNode->_size == 0)
        {
            delete leafNode;
            _noOfNodes--;
            _root = NULL;
            _height--;
            // cout << "entire tree deleted" << endl;

            return removedRecord;
        }

        // if leftmost key (in the node) is removed, update keys in parent(s)
        // if 1 level tree, no need to update (no parents)
        if (leafNode != _root && keyIndex == 0)
        {
            // cout << "leftmost key in leaf node deleted" << endl;
            // if node is also leftmost in parent, update grandparent and so on (bc subtree min is affected)
            updateParent(parentNode, leafNodeIndex, leafNode->_key[0]);
        }

        if (leafNode == _root || leafNode->_size >= MIN_KEYS_LEAF)
        {
            // cout << "minimum met, no need to consider cases" << endl;
            return removedRecord;
        }

        // case 1: can borrow from left sibling
        if (leftSiblingNode != NULL && leftSiblingNode->_size - 1 >= MIN_KEYS_LEAF)
        {
            // cout << "remove() case 1" << endl;
            // make space at leftmost on leafNode
            for (int i = leafNode->_size; i > 0; i--)
            {
                leafNode->_key[i] = leafNode->_key[i - 1];
                leafNode->_record[i] = leafNode->_record[i - 1];
            }
            // take rightmost key from left sibling
            leafNode->_key[0] = leftSiblingNode->_key[leftSiblingNode->_size - 1];
            leafNode->_record[0] = leftSiblingNode->_record[leftSiblingNode->_size - 1];
            leafNode->_size++;

            leftSiblingNode->_size--;
            leftSiblingNode->_key[leftSiblingNode->_size] = NULL;
            leftSiblingNode->_record[leftSiblingNode->_size].clear();

            // only direct parent will be affected.
            // will not borrow minimum key (leftmost on left sibling), so parents above will not change
            parentNode->_key[leafNodeIndex - 1] = leafNode->_key[0];
        }

        // case 2: can borrow from right sibling
        else if (rightSiblingNode != NULL && rightSiblingNode->_size - 1 >= MIN_KEYS_LEAF)
        {
            // cout << "remove() case 2" << endl;
            // take leftmost key from right sibling
            leafNode->_key[leafNode->_size] = rightSiblingNode->_key[0];
            leafNode->_record[leafNode->_size] = rightSiblingNode->_record[0];
            leafNode->_size++;

            // shift all keys and ptrs in right sibling to left (fill space left by moved key/ptr)
            for (int i = 0; i < rightSiblingNode->_size - 1; i++)
            {
                rightSiblingNode->_key[i] = rightSiblingNode->_key[i + 1];
                rightSiblingNode->_record[i] = rightSiblingNode->_record[i + 1];
            }
            rightSiblingNode->_size--;
            rightSiblingNode->_key[rightSiblingNode->_size] = NULL;
            rightSiblingNode->_record[rightSiblingNode->_size].clear();

            parentNode->_key[rightSiblingIndex - 1] = rightSiblingNode->_key[0];
        }

        // case 3: cannot borrow from either, merge with left sibling
        else if (leftSiblingNode != NULL)
        {
            // cout << "remove() case 3" << endl;
            int j = leftSiblingNode->_size; // current index for left sibling
            for (int i = 0; i < leafNode->_size; i++)
            {
                leftSiblingNode->_key[j] = leafNode->_key[i];
                leftSiblingNode->_record[j] = leafNode->_record[i];
                j++;
            }
            leftSiblingNode->_nextNode = leafNode->_nextNode;
            leftSiblingNode->_size = j;
            leafNode->_size = 0;

            delete leafNode;
            _noOfNodes--;
            // cout << "leaf node merged with left sibling" << endl;
            numNodesDeleted++;

            // decrease num keys/ptrs in parent node, need to update parent nodes recursively
            removeInternal(parentNode, leafNodeIndex, numNodesDeleted);
        }

        // case 4: cannot borrow, no left sibling, merge with right sibling
        else if (rightSiblingNode != NULL)
        {
            // cout << "remove() case 4" << endl;
            int j = leafNode->_size;
            for (int i = 0; i < rightSiblingNode->_size; i++)
            {
                leafNode->_key[j] = rightSiblingNode->_key[i];
                leafNode->_record[j] = rightSiblingNode->_record[i];
                j++;
            }
            leafNode->_nextNode = rightSiblingNode->_nextNode;
            leafNode->_size = j;
            rightSiblingNode->_size = 0;

            delete rightSiblingNode;
            _noOfNodes--;
            // cout << "leaf node merged with right sibling" << endl;
            numNodesDeleted++;

            removeInternal(parentNode, rightSiblingIndex, numNodesDeleted);
        }

        return removedRecord;
    }

    // if child node is deleted, need to check if still have sufficient keys. if not, borrow from/merge w sibling
    // internalNode: parent node of deleted child node
    void removeInternal(Node *internalNode, int removedChildIndex, int &numNodesDeleted)
    {
        // special case: if internalNode is the root, and has no keys (aka < 2 ptrs) after deletion, remove root
        if (internalNode == _root && internalNode->_size == 1)
        {
            if (removedChildIndex == 0)
            {
                // cout << "using left child as root" << endl;
                _root = internalNode->_pointer[1];
            }
            else
            {
                // cout << "using right child as root" << endl;
                _root = internalNode->_pointer[0];
            }
            _height--;

            delete internalNode;
            _noOfNodes--;
            // cout << "root node deleted" << endl;
            numNodesDeleted++;
            return;
        }

        // remove the related key and pointer
        for (int i = removedChildIndex - 1; i < internalNode->_size; i++)
        {
            internalNode->_key[i] = internalNode->_key[i + 1];
            internalNode->_pointer[i + 1] = internalNode->_pointer[i + 2];
        }
        internalNode->_size--;
        internalNode->_key[internalNode->_size] = NULL;
        internalNode->_pointer[internalNode->_size + 1] = NULL;

        // check if have minimum keys
        if (internalNode == _root || internalNode->_size >= MIN_KEYS_INTERNAL)
        {
            // cout << "internal node minimum keys fulfilled" << endl;
            return;
        }

        // get parents and siblings
        struct relatedNodes nodes = getRelatedNodes(internalNode);
        Node *parentNode = nodes.parentNode;
        Node *leftSiblingNode = nodes.leftSiblingNode;
        Node *rightSiblingNode = nodes.rightSiblingNode;
        int internalNodeIndex = nodes.nodeIndex;
        int leftSiblingIndex = nodes.leftSiblingIndex;
        int rightSiblingIndex = nodes.rightSiblingIndex;

        // case 1: borrow from left sibling
        if (leftSiblingNode != NULL && leftSiblingNode->_size - 1 >= MIN_KEYS_INTERNAL)
        {
            // cout << "removeInternal() case 1" << endl;
            // take last pointer of left sibling to be first pointer of internal node
            // new key of internal node = min key of subtree pointed to w original pointer
            internalNode->_pointer[internalNode->_size + 1] = internalNode->_pointer[internalNode->_size]; // move last pointer first
            for (int i = internalNode->_size; i > 0; i--)
            {
                internalNode->_key[i] = internalNode->_key[i - 1];
                internalNode->_pointer[i] = internalNode->_pointer[i - 1];
            }
            internalNode->_pointer[0] = leftSiblingNode->_pointer[leftSiblingNode->_size];
            internalNode->_key[0] = parentNode->_key[internalNodeIndex - 1];
            internalNode->_size++;

            parentNode->_key[internalNodeIndex - 1] = leftSiblingNode->_key[leftSiblingNode->_size - 1];
            // higher level nodes are not affected, since the minimum key in left sibling is not affected -> minimum of subtree will not change

            leftSiblingNode->_size--;
            leftSiblingNode->_key[leftSiblingNode->_size] = NULL;
            leftSiblingNode->_pointer[leftSiblingNode->_size + 1] = NULL;
            return;
        }

        // case 2: borrow from right sibling
        if (rightSiblingNode != NULL && rightSiblingNode->_size - 1 >= MIN_KEYS_INTERNAL)
        {
            // cout << "removeInternal() case 2" << endl;
            internalNode->_pointer[internalNode->_size + 1] = rightSiblingNode->_pointer[0];
            internalNode->_key[internalNode->_size] = parentNode->_key[rightSiblingIndex - 1];
            internalNode->_size++;

            parentNode->_key[rightSiblingIndex - 1] = rightSiblingNode->_key[0];

            for (int i = 0; i < rightSiblingNode->_size; i++)
            {
                rightSiblingNode->_key[i] = rightSiblingNode->_key[i + 1];
                rightSiblingNode->_pointer[i] = rightSiblingNode->_pointer[i + 1];
            }
            rightSiblingNode->_size--;
            rightSiblingNode->_key[rightSiblingNode->_size] = NULL;
            rightSiblingNode->_pointer[rightSiblingNode->_size + 1] = NULL;

            return;
        }

        // case 3: cannot borrow from siblings, merge with left sibling
        if (leftSiblingNode != NULL)
        {
            // cout << "removeInternal() case 3" << endl;
            // consider only the pointers of internalNode when adding to left sibling
            leftSiblingNode->_key[leftSiblingNode->_size] = parentNode->_key[internalNodeIndex - 1];
            leftSiblingNode->_pointer[leftSiblingNode->_size + 1] = internalNode->_pointer[0];
            int j = leftSiblingNode->_size + 1;
            for (int i = 0; i < internalNode->_size; i++)
            {
                leftSiblingNode->_key[j] = internalNode->_key[i];
                leftSiblingNode->_pointer[j + 1] = internalNode->_pointer[i + 1];
                j++;
            }
            leftSiblingNode->_size = j;
            internalNode->_size = 0;

            delete internalNode;
            _noOfNodes--;
            // cout << "internal node merged with left sibling";
            numNodesDeleted++;
            removeInternal(parentNode, internalNodeIndex, numNodesDeleted);

            return;
        }

        // case 4: cannot borrow from siblings, no left sibling, merge with right sibling
        if (rightSiblingNode != NULL)
        {
            // cout << "removeInternal() case 4" << endl;
            // new key = min of right subtree = key in parent node
            internalNode->_key[internalNode->_size] = parentNode->_key[rightSiblingIndex - 1];
            internalNode->_pointer[internalNode->_size + 1] = rightSiblingNode->_pointer[0];
            int j = internalNode->_size + 1;
            for (int i = 0; i < rightSiblingNode->_size; i++)
            {
                internalNode->_key[j] = rightSiblingNode->_key[i];
                internalNode->_pointer[j + 1] = rightSiblingNode->_pointer[i + 1];
                j++;
            }
            internalNode->_size = j;
            rightSiblingNode->_size = 0;

            delete rightSiblingNode;
            _noOfNodes--;
            // cout << "internal node merged with right sibling";
            numNodesDeleted++;
            removeInternal(parentNode, rightSiblingIndex, numNodesDeleted);

            return;
        }
    }

    struct relatedNodes
    {
        Node *node = NULL;
        Node *parentNode = NULL;
        Node *leftSiblingNode = NULL;
        Node *rightSiblingNode = NULL;
        int nodeIndex, leftSiblingIndex, rightSiblingIndex;
        int keyIndex; // index of key in leaf node
    };

    // recursively updates keys in parent if leftmost key in child has changed
    // only stops when 1. child is not leftmost in parent, or 2. parent is root
    void updateParent(Node *parent, int childIndex, int newMinKey)
    {
        if (childIndex == 0 && parent != _root)
        {
            struct relatedNodes nodes = getRelatedNodes(parent);
            updateParent(nodes.parentNode, nodes.nodeIndex, newMinKey);
        }
        else
        {
            if (childIndex > 0)
            {
                parent->_key[childIndex - 1] = newMinKey;
            }
        }
        return;
    }

    struct relatedNodes getRelatedNodes(Node *childNode)
    {
        struct relatedNodes nodes;

        if (childNode == _root)
        {
            return nodes;
        }

        Node *cur = _root;
        std::queue<Node *> queue;

        queue.push(_root);
        while (cur != NULL && !queue.empty())
        {
            cur = queue.front();
            queue.pop();

            for (int i = 0; i < cur->_size + 1; i++)
            {
                if (cur->_pointer[i] == childNode)
                {
                    nodes.node = childNode;
                    nodes.parentNode = cur;
                    nodes.nodeIndex = i;
                    if (i > 0)
                    { // have left sibling
                        // cout << "getrelatednodes left sibling found, index " << i - 1 << endl;
                        nodes.leftSiblingNode = cur->_pointer[i - 1];
                        nodes.leftSiblingIndex = i - 1;
                    }
                    if (i < cur->_size)
                    { // have right sibling
                        // cout << "getrelatednodes right sibling found, index " << i + 1 << endl;
                        nodes.rightSiblingNode = cur->_pointer[i + 1];
                        nodes.rightSiblingIndex = i + 1;
                    }
                    return nodes;
                }
                else if (!cur->_pointer[i]->_leafNode)
                {
                    queue.push(cur->_pointer[i]);
                }
            }
        }

        return nodes;
    }

    struct relatedNodes getLeafNode(int key)
    {
        struct relatedNodes nodes;

        Node *cur = _root;
        Node *prev = _root;

        int i;
        while (!cur->_leafNode)
        {
            bool found = false; // if ptr < any key, found is true. else, use last ptr
            // target < _key[0], follow _pointer[0]
            // _key[0] <= target < key[1], follow _pointer[1]
            for (i = 0; i < cur->_size; i++)
            {
                if (key < cur->_key[i])
                {
                    found = true;
                    break;
                }
            }

            prev = cur;
            if (found)
            {
                cur = cur->_pointer[i];
                // cout << "getLeafNode: following " << i << "th pointer" << endl;
            }
            else
            {
                cur = cur->_pointer[i];
                // cout << "getLeafNode: following " << i << "th pointer" << endl;
            }
        }

        // check if leaf node actually has the key
        for (int j = 0; j < cur->_size; j++)
        {
            if (cur->_key[j] == key)
            {
                // cout << "exact key found in leaf node, index in parent node:" << i << endl;
                nodes.node = cur;
                nodes.keyIndex = j;
                if (cur == _root)
                {
                    break; // single level tree, no parents/siblings
                }
                nodes.nodeIndex = i;
                nodes.parentNode = prev;
                if (i > 0)
                { // have left sibling
                    // cout << "left sibling found" << endl;
                    nodes.leftSiblingNode = prev->_pointer[i - 1];
                    nodes.leftSiblingIndex = i - 1;
                }
                if (i < prev->_size)
                { // have right sibling
                    // cout << "right sibling found" << endl;
                    nodes.rightSiblingNode = prev->_pointer[i + 1];
                    nodes.rightSiblingIndex = i + 1;
                }
                break;
            }
            else if (cur->_key[j] > key)
            { // key not found
                // cout << "exact key not found in leaf node" << endl;
                return nodes;
            }
        }

        return nodes;
    }

    // inclusive on both lower bound and upper bound
    vector<Address *> searchRange(int lower, int upper)
    {
        std::vector<Address *> records;

        if (_root == NULL)
        {
            cout << "B+ tree is empty";
            return records;
        }

        Node *cur = _root;
        int numIndexNodesAccessed = 0;
        int numFound = 0;
        int foundTotal = 0;

        // ptr1 key1 ptr2 key2 ptr3 key3 ptr4

        cout << "\nContent of first 5 index nodes:" << endl;
        // try to find first leaf node with key >= LB. try == LB first
        while (!cur->_leafNode)
        {
            numIndexNodesAccessed++;
            if (numIndexNodesAccessed <= NUM_NODES_TO_DISPLAY)
            {
                // print content of current index block
                cout << "#" << numIndexNodesAccessed << ": ";
                displayInternalNode(cur);
            }

            int i;
            bool found = false; // if ptr < any key, found is true. else, use last ptr
            for (i = 0; i < cur->_size; i++)
            {
                if (lower < cur->_key[i])
                {
                    found = true;
                    break;
                }
            }
            cur = cur->_pointer[i];
        }

        // check if leaf node actually has key >= LB. if not, start result from next node
        bool upperFound = false;
        while (!upperFound && cur != NULL)
        {
            numIndexNodesAccessed++;
            if (numIndexNodesAccessed <= NUM_NODES_TO_DISPLAY)
            {
                // print content of current index block
                cout << "#" << numIndexNodesAccessed << ": ";
                displayLeafNode(cur);
            }

            for (int i = 0; i < cur->_size; i++)
            {
                if (cur->_key[i] >= lower && cur->_key[i] <= upper)
                {
                    numFound = numFound + cur->_record[i].size();
                    for (int j = 0; j < cur->_record[i].size(); j++)
                    {
                        foundTotal = foundTotal + accessAvgRating(cur->_record[i].at(j));
                    }
                    records.insert(end(records), begin(cur->_record[i]), end(cur->_record[i]));
                }
                else if (cur->_key[i] > upper)
                {
                    upperFound = true;
                    break;
                }
            }
            cur = cur->_nextNode;
        }

        cout << "Total number of Index Blocks accessed: " << numIndexNodesAccessed << endl;
        cout << "Number of records found: " << numFound << endl;
        cout << "Average rating: " << (float)foundTotal / numFound << endl;
        return records;
    }

    void displayRemoveStats(int numNodesDeleted)
    {
        cout << "Number of nodes deleted: " << numNodesDeleted << endl;
        displayStats();
    }

    void displayStats()
    {

        cout << "Number of nodes in updated B+ tree: " << countTreeNodes() << " checking " << getNumNodes() << endl;
        cout << "Height of B+ tree: " << _height << endl;
        if (_height == 0)
        {
            cout << "No root node. No child node." << endl;
        }
        else
        {
            cout << "Content of root node: " << endl;
            displayInternalNode(_root);
            if (_height == 1)
            {
                cout << "No child node." << endl;
            }
            else if (_height == 2)
            {
                cout << "Content of 1st child node: " << endl;
                displayLeafNode(_root->_pointer[0]);
            }
            else
            {
                cout << "Content of 1st child node: " << endl;
                displayInternalNode(_root->_pointer[0]);
            }
        }
    }

    int countTreeNodes()
    {
        Node *cur = _root;
        std::queue<Node *> queue;
        int numNodes = 0;

        queue.push(_root);
        while (cur != NULL && !queue.empty())
        {
            cur = queue.front();
            queue.pop();
            numNodes++;
            if (cur->_leafNode)
            {
                continue;
            }
            for (int i = 0; i < cur->_size + 1; i++)
            {
                queue.push(cur->_pointer[i]);
            }
        }

        return numNodes;
    }

    void displayTree()
    {
        Node *cur = _root;
        bool isComplete = false; // true when node == leafnode and pointer to next node is null
        std::queue<Node *> queue;

        if (_root == NULL)
        {
            cout << "B+ Tree is empty\n";
            return;
        }

        cout << "\nStart of Tree. height=" << _height << endl;
        // breadth first traversal
        cout << "---ROOT---" << endl;
        queue.push(_root);
        int prevNumNodes = 1; //
        int nextNumNodes = 0;
        int level = 0;
        while (!queue.empty())
        {
            cur = queue.front();
            queue.pop();
            if (prevNumNodes == 0)
            {
                cout << "\n\n---Level " << ++level << "---" << endl;
                prevNumNodes = nextNumNodes;
                nextNumNodes = 0;
            }
            if (cur->_leafNode)
            {
                displayLeafNode(cur);
                if (cur->_nextNode == NULL)
                {
                    cout << "End of Tree\n"
                         << endl;
                    return;
                }
            }
            else
            {
                displayInternalNode(cur);
                for (int i = 0; i < cur->_size + 1; i++)
                {
                    queue.push(cur->_pointer[i]);
                    nextNumNodes++;
                }
                prevNumNodes--;
            }
        }
    }

    void displayInternalNode(Node *node)
    {
        // indicate how many children
        int i;
        cout << "Internal Node [" << node->_size + 1 << " children]: ";
        for (i = 0; i < node->_size; i++)
        {
            cout << " | " << node->_pointer[i] << " | " << node->_key[i];
        }
        cout << " | " << node->_pointer[i] << endl;
    }
    void displayLeafNode(Node *node)
    {
        cout << "Leaf Node [" << node->_size << " records]: ";
        for (int i = 0; i < node->_size; i++)
        {
            cout << " | " << (node->_record[i]).front() << " (" << node->_record[i].size() << ")"
                 << " | " << node->_key[i];
        }
        cout << " | nextNode:" << node->_nextNode << endl;
    }
};
