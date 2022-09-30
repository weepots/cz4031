#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

#include "indexing.cpp"

using namespace std;

void runTest(BPlusTree tree, vector<int> v);
BPlusTree multipleInserts(BPlusTree tree);
BPlusTree insertKey(BPlusTree tree, int key);
BPlusTree multipleDeletes(BPlusTree tree);
BPlusTree deleteKey(BPlusTree tree, int key);

int main()
{
    int input, block_size;

    // cout<< "Block size: \n 1. 200 Bytes \n 2. 500 Bytes \n 3. Quit" << endl;
    // cin >> input;
    // switch (input){
    //     case 1:
    //         block_size = 200;
    //         break;
    //     case 2:
    //         block_size = 500;
    //         break;
    //     default:
    //         return 0;
    // }
    // cout << block_size <<endl;

    BPlusTree tree;

    // std::vector<int> v = {2, 4, 9, 11, 6, 7, 10, 8, 5, 12, 3, 13, 16, 1};
    std::vector<int> v = {};
    std::ifstream fin("smaller.txt");
    int element;
    while (fin >> element)
    {
        v.push_back(element);
    }
    cout << "total: " << v.size() << endl;
    // std::vector<int> v = {};
    // for (int i = 0; i < 1000000; i++)
    // {
    //     v.push_back(i);
    // }

    tree.displayTree();
    for (int i : v)
    {
        Record *newRecord = new Record(i);
        cout << "inserting " << newRecord->getValue() << endl;
        // cout << "inserting " << i << endl;
        tree.insert(newRecord);
        // cout << "finished inserting " << i << endl;
        // tree.displayTree();
    }

    tree.displayTree();
    cout << "FINISHED INSERTION" << endl;
    cout << "inserted " << v.size() << " records" << endl;

    int choice = 0;
    int manualOption = 0;
    cout << "Select a mode: \n 1. Manual\n 2. Run tests\n 3. Exit" << endl;
    cin >> choice;
    while (choice != 3)
    {
        if (choice == 1)
        {
            cout << "Manual mode, select action: \n 1. Insert\n 2. Delete\n 3. Search\n 4. Search Range\n 5. Exit manual mode" << endl;
            cin >> manualOption;
            while (manualOption != 5)
            {
                if (manualOption == 1)
                {
                    tree = multipleInserts(tree);
                }
                else if (manualOption == 2)
                {
                    tree = multipleDeletes(tree);
                }
                else if (manualOption == 3)
                {
                    tree.displayTree();
                    int key;
                    cout << "Enter key:" << endl;
                    cin >> key;
                    Record *searched = tree.search(key);
                    cout << "record found: " << searched->getValue() << endl;
                }
                else if (manualOption == 4)
                {
                    tree.displayTree();
                    int lb, up;
                    cout << "Enter lower bound (inclusive):" << endl;
                    cin >> lb;
                    cout << "Enter upper bound (inclusive):" << endl;
                    cin >> up;
                    tree.searchRange(lb, up);
                    // vector<Record *> records = tree.searchRange(lb, up);

                    // cout << "Records found: ";
                    // for (Record *i : records)
                    // {
                    //     cout << i->getValue() << " ";
                    // }
                }
                cout << "Manual mode, select action: \n 1. Insert\n 2. Delete\n 3. Search\n 4. Search Range\n 5. Exit manual mode" << endl;
                cin >> manualOption;
            }
        }
        if (choice == 2)
        {
            // runTest(tree, v);
            std::vector<int> todelete = {
                1645, 198, 1342, 120, 2127, 115

            };

            runTest(tree, v);
        }
        cout << "Select a mode: \n 1. Manual\n 2. Run tests\n 3. Exit" << endl;
        cin >> choice;
    }
}

void runTest(BPlusTree tree, vector<int> v)
{
    Record *removed;
    int numNodesDeleted = 0;
    std::queue<int> queue;
    for (int i : v)
    {
        queue.push(i);
    }

    while (!queue.empty())
    {
        int val = queue.front();
        queue.pop();
        cout << "\n------------ REMOVING " << val << "-----------------" << endl;
        removed = tree.remove(val, numNodesDeleted);
        if (removed != NULL)
        {

            cout << "removing " << removed->getValue() << ", final tree:" << endl;
        }
        else
        {
            cout << "key not found in b+ tree, nothing returned. final tree:" << endl;
        }
        tree.displayTree();
        cout << "Total number of nodes deleted: " << numNodesDeleted << endl;
    }
}

BPlusTree multipleInserts(BPlusTree tree)
{
    int key = 0;
    cout << "Input key to insert (-1 to exit):" << endl;
    cin >> key;
    while (key != -1)
    {
        tree = insertKey(tree, key);
        cout << "Input key to insert (-1 to exit):" << endl;
        cin >> key;
    }
    return tree;
}

BPlusTree insertKey(BPlusTree tree, int key)
{
    cout << "\n------------ INSERTING " << key << "-----------------" << endl;
    Record *newRecord = new Record(key);
    tree.insert(newRecord);
    cout << "key inserted. final tree:" << endl;
    tree.displayTree();

    return tree;
}

BPlusTree multipleDeletes(BPlusTree tree)
{
    int key = 0;
    cout << "Input key to delete (-1 to exit):" << endl;
    cin >> key;
    while (key != -1)
    {
        tree = deleteKey(tree, key);
        cout << "Input key to delete (-1 to exit):" << endl;
        cin >> key;
    }

    return tree;
}

BPlusTree deleteKey(BPlusTree tree, int key)
{
    Record *removed;
    int numNodesDeleted = 0;
    cout << "\n------------ REMOVING " << key << "-----------------" << endl;
    removed = tree.remove(key, numNodesDeleted);
    if (removed != NULL)
    {

        cout << "removing " << removed->getValue() << ", final tree:" << endl;
    }
    else
    {
        cout << "key not found in b+ tree, nothing returned. final tree:" << endl;
    }
    delete removed;
    tree.displayTree();

    return tree;
}