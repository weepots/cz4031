#include <iostream>
#include <cstring>
#include "storage.h"

using namespace std;

Storage::Storage(int storageSize, int blkNodeSize){
    //initialise pointers
    char* storagePtr = nullptr;
    this->storagePtr = new char[storageSize]; // points to the first datablock in storage
    this->blkPtr = nullptr;
    //initialise attributes
    this->storageSize = storageSize;
    this->blkNodeSize = blkNodeSize;
    this->currentUsedBlkSize = 0;
    this->usedBlkSize = 0;
    this->usedRecordSize = 0;
    this->availBlk = storageSize/blkNodeSize;
    this->usedBlk = 0;
};

// destroy class and clear memory
Storage:: ~ Storage(){
    delete storagePtr;
    storagePtr = nullptr;
};

//record functions
Address Storage::createBlock(int recordSize){
    //If recordSize is larger than blockSize, return error
    if (recordSize > blkNodeSize){
        cout << "Records cannot be larger than block size\n";
        throw "Records cannot be larger than block size";
    }

    //If record cannot fit into current block
    if((recordSize + currentUsedBlkSize > blkNodeSize) or (usedBlk == 0)){
        //If there is enough space for a block, allocate it
        if(recordSize + currentUsedBlkSize <= storageSize){
            blkPtr = storagePtr + usedBlk * blkNodeSize; //Allocate location of block
            
            //Update statistics
            currentUsedBlkSize = 0;
            usedBlkSize += blkNodeSize;
            availBlk -= 1;
            usedBlk += 1;
        }
        //Otherwise, there is not enough space in storage, throw an error
        else{
            cout << "No memory to allocate new block\n";
            throw "No memory to allocate new block\n";
        }
    }

    //Address where our record will be write into
    Address address = {blkPtr, currentUsedBlkSize};

    //Update statistics
    usedRecordSize += recordSize;
    currentUsedBlkSize += recordSize;

    return address;
};

Address Storage::writeRecord(Record* record, int recordSize){
    Address memoryAddress = createBlock(recordSize);
    memcpy((char*) memoryAddress.blockAddress+memoryAddress.offset, record, recordSize);

    return memoryAddress;
};

void Storage::deleteRecord(Address address, int recordSize){
    //get record address
    try{
        void* recordAddress = (char *)address.blockAddress + address.offset;
        //set entire record to null
        memset(recordAddress, '\0', recordSize );

        //Update actuall storage size
        usedRecordSize =- recordSize;
        currentUsedBlkSize -= recordSize;
        usedBlkSize -= blkNodeSize;

        //
        if(emptyCheck(address)){
            usedBlk--;
            availBlk++;

        
        }
    }
    catch(...){
        cout<<"Could not remove record/block";
    };
    return;
};

bool Storage:: emptyCheck(Address address){
        unsigned char emptyBlock[blkNodeSize];
        memset(emptyBlock, '\0', blkNodeSize);
        bool isEmptyBlock = memcmp(emptyBlock, address.blockAddress, blkNodeSize);
        return isEmptyBlock;
}

//getters
int Storage :: getStorageSize(){
    return storageSize;
}

int Storage :: getblkNodeSize(){
    return blkNodeSize;
}

int Storage :: getUsedRecordSize(){
    return usedRecordSize;
}

int Storage :: getUsedBlkSize(){
    return usedBlkSize;
}

int Storage :: getUsedBlk(){
    return usedBlkSize;
}

int Storage :: getAvailBlk(){
    return availBlk;
}

void Storage :: display(){
    printf("--------------------------------------------------------------\n");
    printf("Storage size\t\t\t: %d\n", storageSize);
    printf("Block size\t\t\t: %d\n", blkNodeSize);
    printf("Memory used by records (MB)\t: %lf\n", (1.0*usedRecordSize)/1000000);
    printf("Memory used by blocks (MB)\t: %lf\n",  (1.0*usedBlkSize)/1000000);
    printf("Memory used by current block\t: %d\n", currentUsedBlkSize);
    printf("No of available blocks\t\t: %d\n", availBlk);
    printf("No of used blocks\t\t: %d\n", usedBlk);
    printf("--------------------------------------------------------------\n");
}