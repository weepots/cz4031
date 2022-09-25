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
    int input, block_size;

    cout<< "Block size: \n 1. 200 Bytes \n 2. 500 Bytes \n 3. Quit" << endl;
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

    storage storage(200000000, block_size);
    // storage storage(20,5);
    vector <Address> data;
    cout << storage.getAvailBlk() <<endl;
    ifstream fin("data.tsv");
    string line;
    while (getline(fin,line)){
        vector<string> parts;
        Record record;
        Address dataRecordAddress;
        string tempData;
        split(parts,line, boost::is_any_of("\t"));
        //passed in record values
        strcpy(record.tconst, parts[0].c_str());
        record.avgRating = stof(parts[1]);
        record.numVotes = stof(parts[2]);
        //still need to pass in address(TODO)

       

         cout << "First of " << parts.size() << " elements: " << record.avgRating << endl;

    }

    fin.close();




    cout << block_size <<endl;
}
