#include <iostream>
#include <cstring>
#include "storage.h"

using namespace std;

Storage::Storage(int storageSize, int blkNodeSize){
    //initialise pointers
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
Address Storage::writeRecord(int recordSize){
    if (recordSize > blkNodeSize){
        cout << "Records cannot be larger than block size\n";
        throw "Records cannot be larger than block size";
    }

    if(recordSize + currentUsedBlkSize > blkNodeSize || usedBlk == 0){
        if(availBlk > 0){
            blkPtr = storagePtr + usedBlk * blkNodeSize; //Allocate location of block
            
            //Update statistics
            currentUsedBlkSize = 0;
            usedBlkSize += blkNodeSize;
            availBlk -= 1;
            usedBlk += 1;
        }
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

//Record* Storage::readRecord(Address address, int recordSize){
//    void* temp = operator new(recordSize);
//    memcpy(temp, (char *)address.blockAddress+address.offset, recordSize);
//
//    return temp;
//};

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
    printf("Memory used by records (MB)\t: %.5lf\n", (1.0*usedRecordSize)/1000000);
    printf("Memory used by blocks (MB)\t: %.5lf\n",  (1.0*usedBlkSize)/1000000);
    printf("Memory used by current block\t: %d\n", currentUsedBlkSize);
    printf("No of available blocks\t\t: %d\n", availBlk);
    printf("No of used blocks\t\t: %d\n", usedBlk);
    printf("--------------------------------------------------------------\n");
}