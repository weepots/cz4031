#include <iostream>
#include "storage.h"

using namespace std;



storage::storage(int storageSize, int blkNodeSize){
    //initialise pointers
    char* storagePtr = nullptr;
    this->storagePtr = new char[storageSize];
    this->blkPtr = nullptr;
    //initialise attributes
    this->storageSize = storageSize;
    this->blkNodeSize = blkNodeSize;
    this->usedBlkSize = 0;
    this->usedRecordSize = 0;
    this->availBlk = storageSize/blkNodeSize;
    this->usedBlk = 0;

};
// destroy class and clear memory
storage:: ~ storage(){
    delete storagePtr;
    storagePtr = nullptr;
};
// record functions
char storage::writeRecord(int recordSize){
    if((recordSize + currentUsedBlkSize) && (!createBlock())){
        cout << "Not enough space in storage to add new record";
    }
    else if (recordSize > blkNodeSize){
        cout << "Records cannot be larger than block size";
    }
    else{
        usedRecordSize += recordSize;
        currentUsedBlkSize += recordSize;
    }
    

};

void storage::deleteRecord(Address address){
    usedRecordSize -= 20; // reduce total record by size of one record , 20b
    void* recordAddress = 

};

bool storage::createBlock(){
    if(availBlk >0){
        blkPtr = storagePtr + usedBlk * blkNodeSize;
        usedBlkSize += blkNodeSize;
        availBlk -= 1;
        usedBlk += 1;
        currentUsedBlkSize = 0;
        return true;
    }
    return false;

};





