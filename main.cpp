#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "storage.h"
#include <cstring>
using namespace std;

int main(){
    // Experiment 1: 
    // Store the data (which is about IMDb movives and
    // described in Part 4) on the disk (as specified in Part 1) and report the
    // following statistics:
    // - the number of blocks;
    // - the size of database (in terms of MB);

    int input, block_size;
    cout << "Please select a block size:\n";
    cout << "1. 200 bytes\n"; 
    cout << "2. 500 Bytes\n" ;
    cout << "3. Quit\n";
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
    vector <Address> data;

    ifstream fin("data.tsv");
    string line;
    getline(fin,line); //Get rid of the first row which is column label;
    while(getline(fin,line)){
        vector<string> parts;
        Record record;
        Address dataRecordAddress;
        string tempData;
        split(parts,line, boost::is_any_of("\t"));

        //passed in record values
        strcpy(record.tconst, parts[0].c_str());
        record.avgRating = stof(parts[1]);
        record.numVotes = stof(parts[2]);

        //insert the record into the storage
        storage.writeRecord(&record, sizeof(Record));
    }

    storage.display();
    fin.close();

// Experiment 2: build a B+ tree on the attribute "numVotes" by
// inserting the records sequentially and report the following statistics:
// - the parameter n of the B+ tree;
// - the number of nodes of the B+ tree;
// - the height of the B+ tree, i.e., the number of levels of the B+ tree;


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





    cout << block_size <<endl;
}
