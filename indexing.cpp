#include <iostream>
#include <math.h>
#include <vector>
#include <queue>
using namespace std;

class Record
{
    int value;

public:
    Record(int val)
    {
        value = val;
    }
    Record()
    {
        value = 0;
    }
    int getValue()
    {
        return value;
    }
};

const int N = 3;
const int MIN_KEYS_LEAF = (N + 1) / 2;
const int MIN_KEYS_INTERNAL = N / 2;
const int NUM_NODES_TO_DISPLAY = 5;

// g++ -o main.exe indexing.cpp main.cpp

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
    std::vector<Record *> *_record; // pointer to list of vectors containing pointers to records
    // Record **_record;
    // actual number of keys
    int _size = 0;

    friend class BPlusTree;

public:
    Node()
    {
        _key = new int[N];
        _pointer = new Node *[N + 1];
        _record = new vector<Record *>[N];
        // _record = new Record *[N];

        for (int i = 0; i < N; i++)
        {
            _key[i] = NULL;
            _pointer[i] = NULL;
        }
        _pointer[N + 1] = NULL;
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
    void insert(Record record)
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
                if (nodeTracker[nodeTrackerIndex]->_key[i] != NULL && nodeTracker[nodeTrackerIndex]->_key[i] > record.getValue())
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
                else if (nodeTracker[nodeTrackerIndex]->_key[i] != NULL && nodeTracker[nodeTrackerIndex]->_key[i] <= record.getValue() && i == N - 1)
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
            if (nodeTracker[nodeTrackerIndex]->_key[i] == record.getValue())
            {
                (nodeTracker[nodeTrackerIndex]->_record[i]).push_back(&record);
                return;
            }
        }

        if (emptySpace)
        {
            int temp1, temp2;
            vector<Record *> *r1, *r2, t;
            for (int i = 0; i < N; i++)
            {
                if (i == 0 && nodeTracker[nodeTrackerIndex]->_key[i] == NULL)
                {
                    nodeTracker[nodeTrackerIndex]->_key[i] = record.getValue();
                    nodeTracker[nodeTrackerIndex]->_record[i].push_back(&record);
                    nodeTracker[nodeTrackerIndex]->_size++;
                    // printf(" i1 ");
                    break;
                }
                else if (nodeTracker[nodeTrackerIndex]->_key[i] > record.getValue())
                {
                    r1 = &nodeTracker[nodeTrackerIndex]->_record[i];
                    temp1 = nodeTracker[nodeTrackerIndex]->_key[i];
                    // nodeTracker[nodeTrackerIndex]->_record[i] = &record;
                    vector<Record *> emptyVector;
                    nodeTracker[nodeTrackerIndex]->_record[i] = emptyVector;
                    (nodeTracker[nodeTrackerIndex]->_record[i]).push_back(&record);
                    nodeTracker[nodeTrackerIndex]->_key[i] = record.getValue();
                    nodeTracker[nodeTrackerIndex]->_size++;
                    for (int j = i + 1; j < N; j++)
                    {
                        r2 = &nodeTracker[nodeTrackerIndex]->_record[j];
                        temp2 = nodeTracker[nodeTrackerIndex]->_key[j];
                        nodeTracker[nodeTrackerIndex]->_record[j] = *r1;
                        nodeTracker[nodeTrackerIndex]->_key[j] = temp1;
                        r1 = r2;
                        temp1 = temp2;
                        // cout<<"i2 "<<endl;
                    }

                    break;
                }
                else if (nodeTracker[nodeTrackerIndex]->_key[i] == NULL)
                {
                    nodeTracker[nodeTrackerIndex]->_key[i] = record.getValue();
                    // nodeTracker[nodeTrackerIndex]->_record[i] = &record;
                    (nodeTracker[nodeTrackerIndex]->_record[i]).push_back(&record);
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
            _noOfNodes++;
            return;
        }
        else
        {
            int temp[N + 1];
            // Record *recordTemp[N + 1];
            std::vector<Record *> *recordTemp = new vector<Record *>[N + 1];
            int floorVal = floor((N + 1) / 2);
            int ceilVal = ceil((N + 1) / 2);
            bool recordAdded = false;
            int keyIndex = 0;
            for (int i = 0; i < N + 1; i++)
            {
                if (!recordAdded)
                {
                    if (keyIndex < N && nodeTracker[nodeTrackerIndex]->_key[keyIndex] < record.getValue())
                    {
                        temp[i] = nodeTracker[nodeTrackerIndex]->_key[keyIndex];
                        recordTemp[i] = nodeTracker[nodeTrackerIndex]->_record[keyIndex];
                        keyIndex++;
                    }
                    else
                    {
                        temp[i] = record.getValue();
                        recordTemp[i].push_back(&record);
                        // recordTemp[i] = &record;
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
                    vector<Record *> emptyVector;
                    nodeTracker[nodeTrackerIndex]->_record[i] = emptyVector;
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
                    vector<Record *> emptyVector;
                    newNode->_record[i] = emptyVector;
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
                    _noOfNodes++;
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

    Record *search(int value)
    {
        int noOfNodes = 1, nodesPrinted = 1;
        Record *result;
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
    // record is not actually removed from memory yet
    // idk if can yet, but try return reference of record, not found return nothing
    Record *remove(int key, int &numNodesDeleted)
    {
        if (_root == NULL)
        {
            cout << "tree is empty" << endl;
            return NULL;
        }

        Record *removedRecord = NULL;
        // int numNodesDeleted = 0;

        struct relatedNodes nodes = getLeafNode(key);
        Node *leafNode = nodes.node;
        Node *parentNode = nodes.parentNode;
        Node *leftSiblingNode = nodes.leftSiblingNode;
        Node *rightSiblingNode = nodes.rightSiblingNode;
        int leafNodeIndex = nodes.nodeIndex;
        int leftSiblingIndex = nodes.leftSiblingIndex;
        int rightSiblingIndex = nodes.rightSiblingIndex;
        int keyIndex = nodes.keyIndex;
        // cout << "------LEAF NODE DETAILS------" << endl;
        // cout << "leafNode: " << leafNode << endl;
        // cout << "parentNode: " << parentNode << endl;
        // cout << "leftSiblingNode: " << leftSiblingNode << endl;
        // cout << "rightSiblingNode: " << rightSiblingIndex << endl;
        // cout << "leafNodeIndex: " << leafNodeIndex << endl;
        // cout << "leftSiblingIndex: " << leftSiblingIndex << endl;
        // cout << "rightSiblingIndex: " << rightSiblingIndex << endl;
        // cout << "keyIndex: " << keyIndex << endl;

        if (leafNode == NULL)
        {
            // key not found
            cout << "key not found" << endl;
            return NULL;
        }

        // delete the key at leaf node (check if underflow -> can borrow from sibling, cannot borrow from sibling), update parents recursively

        // check vector: if points to >1 record, just remove duplicate. if point to 1 record, delete key
        if (leafNode->_record[keyIndex].size() > 1)
        {
            // duplicate key, no need to remove key in B+ tree, just remove duplicate
            removedRecord = (leafNode->_record[keyIndex]).front();
            leafNode->_record[keyIndex].erase(leafNode->_record[keyIndex].begin()); // remove first record
            displayStats(numNodesDeleted);

            return removedRecord;
        }
        else
        {
            // delete key at leaf node
            removedRecord = leafNode->_record[keyIndex].front();
            for (int i = keyIndex; i < leafNode->_size - 1; i++)
            {
                leafNode->_key[i] = leafNode->_key[i + 1]; // not sure if will be issue if deleting from full leaf node
                leafNode->_record[i] = leafNode->_record[i + 1];
            }
            leafNode->_size--;
            leafNode->_key[leafNode->_size] = NULL;
            vector<Record *> emptyVector;
            leafNode->_record[leafNode->_size] = emptyVector;
        }

        // if only 1 level tree, and last key deleted, entire tree is deleted
        if (leafNode == _root && leafNode->_size == 0)
        {
            delete[] leafNode->_key;
            delete[] leafNode->_pointer;
            delete[] leafNode->_record;
            delete leafNode;
            _root = NULL;
            _height--;
            cout << "entire tree deleted" << endl;
            displayStats(++numNodesDeleted);

            return removedRecord;
        }

        // if leftmost key (in the node) is removed, update keys in parent(s)
        // if 1 level tree, no need to update (no parents)
        if (leafNode != _root && keyIndex == 0)
        {
            cout << "leftmost key in leaf node deleted" << endl;
            // if node is also leftmost in parent, update grandparent and so on (bc subtree min is affected)
            updateParent(parentNode, leafNodeIndex, leafNode->_key[0]);
        }

        // check for min keys (leaf node)
        // cout << "leafNode size: " << leafNode->_size << endl;
        // cout << "MIN_KEYS_LEAF: " << MIN_KEYS_LEAF << endl;
        if (leafNode == _root || leafNode->_size >= MIN_KEYS_LEAF)
        {
            cout << "minimum met, no need to consider cases" << endl;
            displayStats(numNodesDeleted);
            return removedRecord;
        }

        // case 1: can borrow from left sibling
        if (leftSiblingNode != NULL && leftSiblingNode->_size - 1 >= MIN_KEYS_LEAF)
        {
            cout << "remove() case 1" << endl;
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
            vector<Record *> emptyVector;
            leftSiblingNode->_record[leftSiblingNode->_size] = emptyVector;

            // only direct parent will be affected.
            // will not borrow minimum key (leftmost on left sibling), so parents above will not change (? need confirm)
            parentNode->_key[leafNodeIndex - 1] = leafNode->_key[0];
        }

        // case 2: can borrow from right sibling
        else if (rightSiblingNode != NULL && rightSiblingNode->_size - 1 >= MIN_KEYS_LEAF)
        {
            cout << "remove() case 2" << endl;
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
            vector<Record *> emptyVector;
            rightSiblingNode->_record[rightSiblingNode->_size] = emptyVector;

            parentNode->_key[rightSiblingIndex - 1] = rightSiblingNode->_key[0];
        }

        // case 3: cannot borrow from either, merge with left sibling
        else if (leftSiblingNode != NULL)
        {
            cout << "remove() case 3" << endl;
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

            delete[] leafNode->_key;
            delete[] leafNode->_pointer;
            delete[] leafNode->_record;
            delete leafNode;
            cout << "leaf node merged with left sibling" << endl;
            numNodesDeleted++;

            // decrease num keys/ptrs in parent node, need to update parent nodes recursively
            removeInternal(parentNode, leafNodeIndex, numNodesDeleted);
        }

        // ? case 4: cannot borrow, no left sibling, merge with right sibling
        else if (rightSiblingNode != NULL)
        {
            cout << "remove() case 4" << endl;
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

            delete[] rightSiblingNode->_key;
            delete[] rightSiblingNode->_pointer;
            delete[] rightSiblingNode->_record;
            delete rightSiblingNode;
            cout << "leaf node merged with right sibling" << endl;
            numNodesDeleted++;

            removeInternal(parentNode, rightSiblingIndex, numNodesDeleted);
        }

        displayStats(numNodesDeleted);

        return removedRecord;
    }

    // if child node is deleted, need to check if still have sufficient keys. if not, borrow from/merge w sibling
    // internalNode: parent node of deleted child node
    void removeInternal(Node *internalNode, int removedChildIndex, int &numNodesDeleted)
    {
        // cout << "running removeInternal( " << internalNode << ", " << removedChildIndex << " )" << endl;
        // special case: if internalNode is the root, and has no keys (aka < 2 ptrs) after deletion, remove root
        if (internalNode == _root && internalNode->_size == 1)
        {
            cout << "in special case" << endl;
            if (removedChildIndex == 0)
            {
                cout << "using left child as root" << endl;
                _root = internalNode->_pointer[1];
            }
            else
            {
                cout << "using right child as root" << endl;
                _root = internalNode->_pointer[0];
            }
            _height--;
            delete[] internalNode->_key;
            delete[] internalNode->_pointer;
            delete[] internalNode->_record;
            delete internalNode;
            cout << "root node deleted" << endl;
            numNodesDeleted++;
            return;
        }

        // remove the related key and pointer
        for (int i = removedChildIndex - 1; i < internalNode->_size; i++)
        {
            internalNode->_key[i] = internalNode->_key[i + 1];
            internalNode->_pointer[i + 1] = internalNode->_pointer[i + 2]; // TODO: check this
        }
        internalNode->_size--;
        internalNode->_key[internalNode->_size] = NULL;
        internalNode->_pointer[internalNode->_size + 1] = NULL;

        // check if have minimum keys
        // cout << "testing condition: " << (internalNode == _root) << " " << (internalNode->_size) << endl;
        if (internalNode == _root || internalNode->_size >= MIN_KEYS_INTERNAL)
        {
            cout << "internal node minimum keys fulfilled" << endl;
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
        // cout << "------INTERNAL NODE DETAILS------" << endl;
        // cout << "internal: " << internalNode << endl;
        // cout << "parentNode: " << parentNode << endl;
        // cout << "leftSiblingNode: " << leftSiblingNode << endl;
        // cout << "rightSiblingNode: " << rightSiblingNode << endl;
        // cout << "internalNodeIndex: " << internalNodeIndex << endl;
        // cout << "leftSiblingIndex: " << leftSiblingIndex << endl;
        // cout << "rightSiblingIndex: " << rightSiblingIndex << endl;

        // case 1: borrow from left sibling
        if (leftSiblingNode != NULL && leftSiblingNode->_size - 1 >= MIN_KEYS_INTERNAL)
        {
            cout << "removeInternal() case 1" << endl;
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
            cout << "removeInternal() case 2" << endl;
            internalNode->_pointer[internalNode->_size + 1] = rightSiblingNode->_pointer[0];
            internalNode->_key[internalNode->_size] = parentNode->_key[rightSiblingIndex - 1]; // getMinKey(rightSiblingNode->_pointer[0]);
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
            cout << "removeInternal() case 3" << endl;
            // consider only the pointers of internalNode when adding to left sibling
            leftSiblingNode->_key[leftSiblingNode->_size] = parentNode->_key[internalNodeIndex - 1]; // TODO: check this
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

            delete[] internalNode->_key;
            delete[] internalNode->_pointer;
            delete[] internalNode->_record;
            delete internalNode;
            cout << "internal node merged with left sibling";
            numNodesDeleted++;
            removeInternal(parentNode, internalNodeIndex, numNodesDeleted);

            return;
        }

        // case 4: cannot borrow from siblings, no left sibling, merge with right sibling
        if (rightSiblingNode != NULL)
        {
            cout << "removeInternal() case 4" << endl;
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

            delete[] rightSiblingNode->_key;
            delete[] rightSiblingNode->_pointer;
            delete[] rightSiblingNode->_record;
            delete rightSiblingNode;
            cout << "internal node merged with right sibling";
            numNodesDeleted++;
            removeInternal(parentNode, rightSiblingIndex, numNodesDeleted);

            return;
        }
    }

    struct relatedNodes
    {
        Node *node = NULL;
        Node *parentNode = NULL;
        Node *leftSiblingNode = NULL; // consider just returning the index and accessing via that?
        Node *rightSiblingNode = NULL;
        int nodeIndex, leftSiblingIndex, rightSiblingIndex;
        int keyIndex; // index of key in leaf node
    };

    // recursively updates keys in parent if leftmost key in child has changed
    // only stops when 1. child is not leftmost in parent, or 2. parent is root
    void updateParent(Node *parent, int childIndex, int newMinKey)
    {
        // cout << "in updateParent(" << childIndex << " " << newMinKey << endl;
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
                        cout << "getrelatednodes left sibling found, index " << i - 1 << endl;
                        nodes.leftSiblingNode = cur->_pointer[i - 1];
                        nodes.leftSiblingIndex = i - 1;
                    }
                    if (i < cur->_size)
                    { // have right sibling
                        cout << "getrelatednodes right sibling found, index " << i + 1 << endl;
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
                cout << "getLeafNode: following " << i << "th pointer" << endl;
            }
            else
            {
                cur = cur->_pointer[i];
                cout << "getLeafNode: following " << i << "th pointer" << endl;
            }
        }

        // cout << "getLeafNode narrowed down to this leaf: " << endl;
        // displayLeafNode(cur);

        // check if leaf node actually has the key
        for (int j = 0; j < cur->_size; j++)
        {
            if (cur->_key[j] == key)
            {
                cout << "exact key found in leaf node, index in parent node:" << i << endl;
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
                    cout << "left sibling found" << endl;
                    nodes.leftSiblingNode = prev->_pointer[i - 1];
                    nodes.leftSiblingIndex = i - 1;
                }
                if (i < prev->_size)
                { // have right sibling
                    cout << "right sibling found" << endl;
                    nodes.rightSiblingNode = prev->_pointer[i + 1];
                    nodes.rightSiblingIndex = i + 1;
                }
                break;
            }
            else if (cur->_key[j] > key)
            { // key not found
                cout << "exact key not found in leaf node" << endl;
                return nodes; // TODO: not sure if can return this? need to check
            }
        }

        return nodes;
    }

    // inclusive on both lower bound and upper bound
    void searchRange(int lower, int upper)
    {
        // std::vector<Record *> records = {};

        if (_root == NULL)
        {
            cout << "B+ tree is empty";
            return;
            // return records;
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
            cur = cur->_pointer[i]; // check
            // cur = (found) ? cur->_pointer[i] : cur->_pointer[i + 1];
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
                    foundTotal = foundTotal + cur->_key[i] * cur->_record[i].size(); // check these
                    cout << "checking record getvalue: " << cur->_record[i].front()->getValue() << " should be " << cur->_key[i] << endl;
                    // numFound++;
                    // foundTotal = foundTotal + cur->_key[i];
                    // records.push_back(cur->_record[i]);
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
        cout << "\nContent of first 5 data blocks: TODO" << endl;

        cout << "Number of records found: " << numFound << endl;
        cout << "Average rating: " << (float)foundTotal / numFound << endl;
        return;
    }

    void displayStats(int numNodesDeleted)
    {
        cout << "Number of nodes deleted: " << numNodesDeleted << endl;
        cout << "Number of nodes in updated B+ tree: " << countTreeNodes() << endl;
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
            // cout << "Current Queue has " << queue.size() << " items" << endl;
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

        // cout << "Internal Node (until NULL)";
        // for (int i = 0; i < N; i++)
        // {
        //     if (node->_pointer[i] != NULL)
        //     {

        //         cout << " | " << node->_pointer[i];
        //     }
        //     if (node->_key[i] != NULL)
        //     {
        //         cout << " | " << node->_key[i];
        //     }
        // }
        // cout << endl;
    }
    void displayLeafNode(Node *node)
    {
        cout << "Leaf Node [" << node->_size << " records]: ";
        for (int i = 0; i < node->_size; i++)
        {
            cout << " | " << &(node->_record[i]) << " (" << node->_record[i].size() << ")" //<< " val: " << node->_record[i].front()->getValue()
                 << " | " << node->_key[i];
        }
        cout << " | nextNode:" << node->_nextNode << endl;

        // cout << "Leaf Node (until NULL)";
        // for (int i = 0; i < N; i++)
        // {
        //     // if (node->_record[i] != NULL)
        //     if (!node->_record[i].empty())
        //     {

        //         cout << " | " << &(node->_record[i]);
        //     }
        //     if (node->_key[i] != NULL)
        //     {
        //         cout << " | " << node->_key[i];
        //     }
        // }
        // cout << endl;
    }
};
