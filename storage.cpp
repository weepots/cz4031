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
    if(recordSize + )

};

void storage::deleteRecord(char address){

};

bool storage::createBlock(){

};



