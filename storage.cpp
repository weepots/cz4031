#include <iostream>
#include <cstring>
#include "storage.h"

using namespace std;



storage::storage(int storageSize, int blkNodeSize){
    //initialise pointers
    char* storagePtr = nullptr;
    this->storagePtr = new char[storageSize]; // points to the first datablock in storage
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
// storage:: ~ storage(){
//     delete storagePtr;
//     storagePtr = nullptr;
// };
// record functions
char storage::writeRecord(int recordSize){
    if((recordSize + currentUsedBlkSize > blkNodeSize) or (usedBlk == 0)){

        bool allocated = createBlock();
        if(!allocated){
            cout << "unable to allocate a new block";
        }
    }
    else if (recordSize > blkNodeSize){
        cout << "Records cannot be larger than block size";
    }
    else{
        usedRecordSize += recordSize;
        currentUsedBlkSize += recordSize;
    }
    

};

void storage::deleteRecord(Address address, int recordSize){
    //get record address

    try{
        void* recordAddress = (char *)address.blockAddress + address.offset;
        //set entire record to null
        memset(recordAddress, '/0', recordSize );

        //Update actuall storage size
        usedRecordSize =- recordSize;


        //
        if(emptyCheck(address)){
            usedBlk--;
            availBlk++;
            currentUsedBlkSize -= blkNodeSize;
        
        }
    }
    catch(...){
        cout<<"Could not remove record/block";
    };

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

bool storage:: emptyCheck(Address address){
        unsigned char emptyBlock[blkNodeSize];
        memset(emptyBlock, '/0', blkNodeSize);
        bool isEmptyBlock = memcmp(emptyBlock, address.blockAddress, blkNodeSize);
        return isEmptyBlock;
}

//getters
int storage :: getStorageSize(){
    return storageSize;
}

int storage :: getblkNodeSize(){
    return blkNodeSize;
}

int storage :: getUsedRecordSize(){
    return usedRecordSize;
}

int storage :: getUsedBlkSize(){
    return usedBlkSize;
}

int storage :: getUsedBlk(){
    return usedBlkSize;
}

int storage :: getAvailBlk(){
    return availBlk;
}