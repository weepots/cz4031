#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

#include "storage.h"
#include "indexing.cpp"
using namespace std;

void experiment1(Storage& storage){
    cout << "------------------------------Experiment 1--------------------------------\n";
    printf("No of available blocks\t\t: %d\n", storage.getAvailBlk());
    printf("No of used blocks\t\t: %d\n", storage.getUsedBlk());
    printf("Memory used by records (MB)\t: %.5lf\n", (1.0 * storage.getTotalUsedRecordSize()) / 1000000);
    printf("Memory used by blocks (MB)\t: %.5lf\n", (1.0 * storage.getTotalUsedBlkSize() / 1000000));
    cout << "--------------------------------------------------------------------------\n\n";

    storage.resetBlkAccessed();
}

void experiment2(BPlusTree& tree){
    cout << "\n\n------------------------------Experiment 2--------------------------------\n";
    int numNode = 0;
    tree.displayStats();
    cout << "--------------------------------------------------------------------------\n\n";
}

void experiment3(Storage& storage, BPlusTree& tree){
    cout << "\n\n------------------------------Experiment 3--------------------------------\n";
    vector<Address*> tempAddressVector = tree.searchRange(500, 500);
    for(auto it : tempAddressVector){
        storage.getNumVotes(it);
    }
    printf("Number of data blocks process accessed : %d\n", storage.getBlkAccessed());
    storage.printEveryRecordInAccessedBlock();
    cout << "--------------------------------------------------------------------------\n\n";

    storage.resetBlkAccessed();
}

void experiment4(Storage& storage, BPlusTree& tree){
    cout << "\n\n------------------------------Experiment 4--------------------------------\n";
    vector<Address*> tempAddressVector = tree.searchRange(30000, 40000);
    for(auto it : tempAddressVector){
        storage.getNumVotes(it);
    }
    printf("Number of data blocks process accessed : %d\n", storage.getBlkAccessed());
    storage.printEveryRecordInAccessedBlock();
    cout << "--------------------------------------------------------------------------\n\n";

    storage.resetBlkAccessed();
}

void experiment5(Storage& storage, BPlusTree& tree){
    cout << "\n\n------------------------------Experiment 5--------------------------------\n";
    int numNodesDeleted = 0;
    int keyToRemove = 1000;
    Address *removed;
    while( (removed = tree.remove(keyToRemove, numNodesDeleted)) != nullptr){
        numNodesDeleted++;
        storage.deleteRecord(removed, sizeof(Record));
    }
    tree.displayRemoveStats(numNodesDeleted);
    cout << "--------------------------------------------------------------------------\n\n";
}

void experiment(Storage& storage, BPlusTree& tree){
    experiment1(storage);
    experiment2(tree);
    experiment3(storage, tree);
    experiment4(storage, tree);
    experiment5(storage, tree);  
}

int main()
{   
    printf("Welcome to CZ4031 Project 1!\n");
    printf("Made by Group 62\n");
    int input = 1, block_size;
    while(input != 3){
        cout << "Please select a block size:\n";
        cout << "1. 200 bytes\n";
        cout << "2. 500 Bytes\n";
        cout << "0. Quit\n";
        cout << "Option: ";
        cin >> input;
        switch (input)
        {
        case 1:
            block_size = 200;
            break;
        case 2:
            block_size = 500;
            break;
        default:
            printf("Thanks for running this program!\n");
            return 0;
        }

        Storage storage(300000000, block_size);
        BPlusTree tree = BPlusTree();

        //Read from the file
        ifstream fin("data.tsv");
        string line;
        getline(fin, line); // Get rid of the first row which is column label;
        bool first = true;
        vector<Address> addressVector;
        while (getline(fin, line))
        {
            vector<string> parts;
            Record record;
            Address dataAddress;
            string tempData;

            // passed in record values
            int pos = line.find('\t');
            int pos2 = line.find('\t', pos + 1);
            strcpy(record.tconst, line.substr(0, pos).c_str());
            record.avgRating = stof(line.substr(pos + 1, pos2));
            record.numVotes = stoi(line.substr(pos2 + 1, line.size()));
            record.deleted = false;

            dataAddress = storage.writeRecord(record, sizeof(Record));
            addressVector.push_back(dataAddress);
        }

        storage.display();
        fin.close();

        for (int i = 0; i < addressVector.size(); i++){ // This here is an address object
            Address *temp = &addressVector[i];
            tree.insert(temp);
        }
        
        experiment(storage, tree);
    }

    // Experiment 1:
    // Store the data (which is about IMDb movives and
    // described in Part 4) on the disk (as specified in Part 1) and report the
    // following statistics:
    // - the number of blocks;
    // - the size of database (in terms of MB);
    
    //experiment1(storage);

    // Experiment 2: build a B+ tree on the attribute "numVotes" by
    // inserting the records sequentially and report the following statistics:
    // - the parameter n of the B+ tree;
    // - the number of nodes of the B+ tree;
    // - the height of the B+ tree, i.e., the number of levels of the B+ tree;
    
    //experiment2(tree);

    // Experiment 3: retrieve those movies with the “numVotes” equal
    // to 500 and report the following statistics:
    // - the number and the content of index nodes the process accesses; (for
    // the content, it would be sufficient to report for the first 5 index nodes or data
    // blocks only if there are more than 5, and this applies throughout Experiment 3
    // and Experiment 4).
    // - the number and the content of data blocks the process accesses;
    // - the average of “averageRating’s” of the records that are returned;

    //experiment3(storage, tree);

    // Experiment 4: retrieve those movies with the attribute
    // “numVotes” from 30,000 to 40,000, both inclusively and report the following
    // statistics:
    // - the number and the content of index nodes the process accesses;
    // - the number and the content of data blocks the process accesses;
    // - the average of “averageRating’s” of the records that are returned;

    //experiment4(storage, tree);

    // Experiment 5: delete those movies with the attribute
    // “numVotes” equal to 1,000, update the B+ tree accordingly, and report the
    // following statistics:
    // - the number of times that a node is deleted (or two nodes are merged)
    // during the process of the updating the B+ tree;
    // - the number nodes of the updated B+ tree;
    // - the height of the updated B+ tree;
    // - the content of the root node and its 1st child node of the updated B+
    // tree;

    //experiment5(storage, tree);

    // Re-set the block size to be 500 B and re-do Experiment 1, 2, 3, 4,
    // and 5.
}
