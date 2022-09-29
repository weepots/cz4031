#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

#include "storage.h"
//#include "indexing.cpp"
using namespace std;

int main(){
    int input, block_size;
    cout << "Please select a block size:\n";
    cout << "1. 200 bytes\n"; 
    cout << "2. 500 Bytes\n" ;
    cout << "3. Quit\n";
    cout << "Option: ";
    cin >> input;
    switch (input){
        case 1:
            block_size = 200;
            break;
        case 2:
            block_size = 500;
            break;
        default:
            return 0;
    }

    Storage storage(300000000, block_size);

    ifstream fin("data.tsv");
    string line;
    getline(fin,line); //Get rid of the first row which is column label;
    bool first = true;
    vector<Address> addressVector;
    while(getline(fin,line)){
        vector<string> parts;
        Record record;
        Address dataAddress;
        string tempData;
        
        //passed in record values
        int pos = line.find('\t');
        int pos2 = line.find('\t', pos+1);
        strcpy(record.tconst, line.substr(0, pos).c_str());
        record.avgRating = stof(line.substr(pos+1, pos2));
        record.numVotes = stoi(line.substr(pos2+1, line.size()));
        record.deleted = false;

        //cout << sizeof(Record) << " " << sizeof(record.tconst) << " " << sizeof(record.avgRating) << " " << sizeof(record.numVotes) << " " << sizeof(record.deleted) << "\n";
        //cout << record.tconst << " "  << record.avgRating << " " << record.numVotes << "\n";;

        //insert the record into the storage
        //cout << "Failed \n";
        dataAddress = storage.writeRecord(record, sizeof(Record));
        //storage.deleteRecord(dataAddress, sizeof(Record));
        addressVector.push_back(dataAddress);
    }
    storage.deleteRecord(&addressVector[1], sizeof(Record));
    storage.printEveryRecordInAccessedBlock();

    // Record temp;
    // temp.avgRating = 7.2;
    // temp.numVotes = 1111;
    // temp.deleted = false;

    // storage.writeRecord(temp, sizeof(Record));
    // storage.printEveryRecordInAccessedBlock();
    //Ways to access record (Need Address)
    Address adr = addressVector[0];
    printf("%s %f %d\n", storage.getTConst(&adr), storage.getAvgRating(&adr), storage.getNumVotes(&adr));

    Record rec = storage.readRecord(&adr);
    printf("%s %f %d\n", rec.tconst, rec.avgRating, rec.numVotes);

    void* memoryAdr = (char*) adr.blockAddress+adr.offset;
    printf("%s \n", (*(Record*)memoryAdr).tconst);

    printf("%s %f %d\n", accessTConst(&adr), accessAvgRating(&adr), accessNumVotes(&adr));
    // //----------------------------------------------------------------------------------------------------

    //storage.printEveryRecordInAccessedBlock();
    //storage.printEveryRecordInSameBlock(addressVector[0]);
    //storage.display();
    fin.close();

    // Experiment 1: 
    // Store the data (which is about IMDb movives and
    // described in Part 4) on the disk (as specified in Part 1) and report the
    // following statistics:
    // - the number of blocks;
    // - the size of database (in terms of MB);
    cout << "------------------------------Experiment 1--------------------------------\n";
    printf("No of available blocks\t\t: %d\n", storage.getAvailBlk());
    printf("No of used blocks\t\t: %d\n", storage.getUsedBlk());
    printf("Memory used by records (MB)\t: %.5lf\n", (1.0*storage.getTotalUsedRecordSize())/1000000);
    printf("Memory used by blocks (MB)\t: %.5lf\n",  (1.0*storage.getTotalUsedBlkSize()/1000000));
    cout << "--------------------------------------------------------------------------\n\n";
    
    //storage.printEveryRecordInAccessedBlock();
    printf("Number of blocks : %d\n", storage.resetBlkAccessed());

    // Experiment 2: build a B+ tree on the attribute "numVotes" by
    // inserting the records sequentially and report the following statistics:
    // - the parameter n of the B+ tree;
    // - the number of nodes of the B+ tree;
    // - the height of the B+ tree, i.e., the number of levels of the B+ tree;
    // BPlusTree tree;
    // int i  = 0;
    // for(auto it : addressVector){
    //     tree.insert(it);
    //     //cout << storage.getNumVotes(it) << "\n";
    //     //cout << "inserting....." << "\n";
    //     //tree.displayTree();
    //     cout << i++ << "\n";
    // }
    // cout << "Done inserting" << "\n";
    // //tree.displayTree();

    // int numNode = 0;
    // for(auto it : addressVector){
    //     cout << "\n" << "\nsearch" << storage.getNumVotes(it) << "\n";
    //     tree.remove(storage.getNumVotes(it), numNode);
    //     //tree.displayTree();
    // }

    // cout << "Done removing" << "\n";
    // tree.displayTree();

    // int numNode = 5;
    // Address* tempAddress = tree.remove(1807, numNode);
    // printf("Actual address : %p %d with %d \n", (char*) addressVector[7].blockAddress+addressVector[7].offset, addressVector[7].offset, storage.getNumVotes(addressVector[7]));
    // printf("Output address : %p %d\n", (char*) (*tempAddress).blockAddress + (*tempAddress).offset, tempAddress->offset);
    // Record temp = storage.readRecord(*tempAddress);
    // cout << temp.tconst << " " << temp.numVotes << "\n";


    //tree.displayTree();
    // tree.displayStats(5);


    


// Experiment 3: retrieve those movies with the “numVotes” equal
// to 500 and report the following statistics:
// - the number and the content of index nodes the process accesses; (for
// the content, it would be sufficient to report for the first 5 index nodes or data
// blocks only if there are more than 5, and this applies throughout Experiment 3
// and Experiment 4).
// - the number and the content of data blocks the process accesses;
// - the average of “averageRating’s” of the records that are returned;
// - the content of the root node and its 1st child node;



// Experiment 4: retrieve those movies with the attribute
// “numVotes” from 30,000 to 40,000, both inclusively and report the following
// statistics:
// - the number and the content of index nodes the process accesses;
// - the number and the content of data blocks the process accesses;
// - the average of “averageRating’s” of the records that are returned;


// Experiment 5: delete those movies with the attribute
// “numVotes” equal to 1,000, update the B+ tree accordingly, and report the
// following statistics:
// - the number of times that a node is deleted (or two nodes are merged)
// during the process of the updating the B+ tree;
// - the number nodes of the updated B+ tree;
// - the height of the updated B+ tree;
// - the content of the root node and its 1st child node of the updated B+
// tree;


// Re-set the block size to be 500 B and re-do Experiment 1, 2, 3, 4,
// and 5.
}
