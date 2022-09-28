#include <iostream>
#include <cstring>
#include <algorithm>
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

    //Update blocks accessed
    if(getBlkAccessed() < 5){
        insertBlkAccessed(address);
    }
    
    //Update statistics
    usedRecordSize += recordSize;
    currentUsedBlkSize += recordSize;

    return address;
};

Record Storage::readRecord(Address address){
    Record temp;
    memcpy(&temp, (char*) address.blockAddress+address.offset, sizeof(Record));
    return temp;
}

void Storage::deleteRecord(Address address, int recordSize){
    //get record address
    try{
        void* recordAddress = (char *)address.blockAddress + address.offset;
        //set entire record to null
        memset(recordAddress, '\0', recordSize);

        //Update actuall storage size
        usedRecordSize =- recordSize;
        currentUsedBlkSize -= recordSize;

        //If the block is empty, remove the block
        if(emptyCheck(address)){
            usedBlkSize -= blkNodeSize;
            usedBlk--;
            availBlk++;
        }
    }
    catch(...){
        cout<<"Could not remove record/block";
    };
    return;
};

char* Storage::getTConst(Address address){
    Record temp;
    memcpy(&temp, (char*) address.blockAddress+address.offset, sizeof(Record));
    char *tempChar = temp.tconst;
    return tempChar;
}

float Storage::getAvgRating(Address address){
    Record temp;
    memcpy(&temp, (char*) address.blockAddress+address.offset, sizeof(Record));
    return temp.avgRating;
}

int Storage::getNumVotes(Address address){
    Record temp;
    memcpy(&temp, (char*) address.blockAddress+address.offset, sizeof(Record));
    return temp.numVotes;
}

bool Storage:: emptyCheck(Address address){
        char *emptyBlock = new char[blkNodeSize];
        memset(emptyBlock, '\0', blkNodeSize);
        bool isEmptyBlock = memcmp(emptyBlock, address.blockAddress, blkNodeSize);
        return isEmptyBlock;
}

void Storage::insertBlkAccessed(Address address){
    int blkNo = (address.blockAddress - storagePtr) / blkNodeSize;
    auto it = find(blkAccessed.begin(), blkAccessed.end(), blkNo);
    if(it == blkAccessed.end()){
        blkAccessed.push_back(blkNo);
    }
}

int Storage::getBlkAccessed(){
    return blkAccessed.size();
}

int Storage::resetBlkAccessed(){
    int temp = blkAccessed.size();
    blkAccessed.clear();
    return temp;
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
    printf("--------------------------------------------------------------\n\n");
}

void Storage::printEveryRecordInSameBlock(Address address){
    int blkNo = (address.blockAddress - storagePtr) / blkNodeSize;
    printf("Block Number : %d\n", blkNo);
    for(int i = 0; i < blkNodeSize; i += sizeof(Record)){
        Record temp;
        memcpy(&temp, (char*)address.blockAddress+i, sizeof(Record));
        printf("\t%s %d\n", temp.tconst, temp.numVotes);
    }
    printf("\n");
}

void Storage::printEveryRecordInAccessedBlock(){ // TO BE CONTINUED
    for(auto it : blkAccessed){
        printf("Block Number : %d\n", it);
        for(int i = 0; i < blkNodeSize; i += sizeof(Record)){
            Record temp;
            void *recordAddress = (char *) storagePtr + it * blkNodeSize;
            memcpy(&temp, (char*)recordAddress + i, sizeof(Record));
            printf("\t%s %d\n", temp.tconst, temp.numVotes);
        }
    }
    return;
}

char* accessTConst(Address address){
    void* memoryAdr = (char*) address.blockAddress+address.offset;
    char* tempCh =  (*(Record*)memoryAdr).tconst;
    return tempCh;
};

float accessAvgRating(Address address){
    void* memoryAdr = (char*) address.blockAddress+address.offset;
    return (*(Record*)memoryAdr).avgRating;
};

int accessNumVotes(Address address){
    void* memoryAdr = (char*) address.blockAddress+address.offset;
    return (*(Record*)memoryAdr).numVotes;
};