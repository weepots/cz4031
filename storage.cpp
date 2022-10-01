#include <iostream>
#include <cstring>
#include <algorithm>
#include "storage.h"

using namespace std;

Storage::Storage(int storageSize, int blkNodeSize){
    //initialise pointers
    this->storagePtr = new char[storageSize]; // points to the first datablock in storage
    memset(storagePtr, '\0', storageSize);
    this->blkPtr = nullptr;
    //initialise attributes
    this->storageSize = storageSize;
    this->blkNodeSize = blkNodeSize;
    this->currentUsedBlkSize = 0;
    this->totalUsedBlkSize = 0;
    this->totalUsedRecordSize = 0;
    this->availBlk = storageSize/blkNodeSize;
    this->usedBlk = 0;

    this->blkAccessed.clear();
    this->deletedAddress.clear();
};

// destroy class and clear memory
Storage:: ~ Storage(){
    delete []storagePtr;
    storagePtr = nullptr;
};

//record functions
Address Storage::writeRecord(Record& record, int recordSize){
    if (recordSize > blkNodeSize){
        cout << "Records cannot be larger than block size\n";
        throw "Records cannot be larger than block size";
    }

    //Address where our record will be write into
    Address address;

    //Update the memory location where address has been deleted
    if(!deletedAddress.empty()){
        address = *(deletedAddress.back());
        deletedAddress.pop_back();

        //Insert the record in storage
        void *ptr = (char*) address.blockAddress+address.offset;
        memcpy(ptr, &record, sizeof(Record));

        //Update blocks accessed
        if(getBlkAccessed() < 5){
            insertBlkAccessed(&address);
        }
        //Update statistics
        totalUsedRecordSize += recordSize;
        if( (char*)address.blockAddress == blkPtr){
            currentUsedBlkSize += recordSize;
        }

        return address;
    }

    if(recordSize + currentUsedBlkSize > blkNodeSize || usedBlk == 0){
        if(availBlk > 0){
            blkPtr = storagePtr + usedBlk * blkNodeSize; //Allocate location of block
            Record temp;
            for(int i = 0; i < blkNodeSize; i += recordSize){
                void *ptr = (char*) blkPtr + i;
                memcpy(ptr, &temp, sizeof(Record));
            }
            
            //Update statistics
            currentUsedBlkSize = 0;
            totalUsedBlkSize += blkNodeSize;
            availBlk -= 1;
            usedBlk += 1;
        }
        else{
            cout << "No memory to allocate new block\n";
            throw "No memory to allocate new block\n";
        }
    }

    //Address where our record will be write into
    address.blockAddress = blkPtr;
    address.offset = currentUsedBlkSize;

    //Insert the record in storage
    void *ptr = (char*) address.blockAddress+address.offset;
    memcpy(ptr, &record, sizeof(Record));

    //Update blocks accessed
    if(getBlkAccessed() < 5){
        insertBlkAccessed(&address);
    }
    
    //Update statistics
    totalUsedRecordSize += recordSize;
    currentUsedBlkSize += recordSize;

    return address;
};

Record Storage::readRecord(Address *address, int recordSize){
    void *ptr = (char * ) address->blockAddress + address->offset;
    if(ptr >= storagePtr + storageSize || ptr < storagePtr){
        cout << "The address is trying to access memory location outside of storage\n";
        throw "Address is trying to access memory location outside of storage";
    }
    else if (address->offset % recordSize != 0){
        cout << "Invalid Address Format\n";
        throw "Invalid Address Format";
    }
    Record temp;
    memcpy(&temp, (char*) address->blockAddress+address->offset, sizeof(Record));
    if(temp.deleted){
        cout << "There is no record here!\n";
        throw "There is no record here!";

    }
    return temp;
}

void Storage::deleteRecord(Address *address, int recordSize){
    //get record address
    try{
        Record temp = readRecord(address, recordSize);
        if(!temp.deleted){
            temp.deleted = true; //Set the deleted status to true;
            void *ptr = (char*) address->blockAddress+address->offset;
            memcpy(ptr, &temp, sizeof(Record));
            deletedAddress.push_back(address);

            //Update blockAccessed
            insertBlkAccessed(address);
        
            //Update actual storage size
            totalUsedRecordSize -= recordSize;
            if( (char*)address->blockAddress == blkPtr){
                currentUsedBlkSize -= recordSize;
            }

            //If the block is empty, remove the block
            if(emptyCheck(address, recordSize)){
                totalUsedBlkSize -= blkNodeSize;
                usedBlk--;
                availBlk++;
            }
        }
    }
    catch(...){
        cout<<"Could not remove record/block";
    };
    return;
};

char* Storage::getTConst(Address *address){
    Record temp;
    memcpy(&temp, (char*) address->blockAddress+address->offset, sizeof(Record));
    char *tempChar = temp.tconst;

    insertBlkAccessed(address);

    return tempChar;
}

float Storage::getAvgRating(Address *address){
    Record temp;
    memcpy(&temp, (char*) address->blockAddress+address->offset, sizeof(Record));

    insertBlkAccessed(address);

    return temp.avgRating;
}

int Storage::getNumVotes(Address *address){
    Record temp;
    memcpy(&temp, (char*) address->blockAddress+address->offset, sizeof(Record));

    insertBlkAccessed(address);

    return temp.numVotes;
}

bool Storage::getDeleted(Address *address){
    Record temp;
    memcpy(&temp, (char*) address->blockAddress+address->offset, sizeof(Record));

    insertBlkAccessed(address);

    return temp.deleted;
}

bool Storage:: emptyCheck(Address * address, int recordSize){
    char * tempBlkPtr = storagePtr + usedBlk * blkNodeSize; //Allocate location of block
    Record temp;
    for(int i = 0; i < blkNodeSize; i += recordSize){
        void *ptr = (char*) address->blockAddress + i;
        memcpy(&temp, (char*)ptr, recordSize);
        //cout << "\t" << temp.deleted << "\n";
        if(!temp.deleted) return false;
    }
    return true;
}

void Storage::insertBlkAccessed(Address *address){
    int blkNo = (address->blockAddress - storagePtr) / blkNodeSize;
    auto it = find(blkAccessed.begin(), blkAccessed.end(), blkNo);
    //printf("Block number : %d\n", blkNo);
    if(it == blkAccessed.end()){
        blkAccessed.push_back(blkNo);
        //cout << "inserted" << "\n";
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

int Storage :: getTotalUsedRecordSize(){
    return totalUsedRecordSize;
}

int Storage :: getTotalUsedBlkSize(){
    return totalUsedBlkSize;
}

int Storage :: getUsedBlk(){
    return usedBlk;
}

int Storage :: getAvailBlk(){
    return availBlk;
}

int Storage :: getBlkNo(Address *address){
    return (address->blockAddress - storagePtr) / blkNodeSize;
}

int Storage :: getActualMemoryUsed(){
    int sum = 0;
    sum += sizeof(storagePtr);
    sum += sizeof(blkPtr);
    sum += sizeof(storageSize);
    sum += sizeof(blkNodeSize);
    sum += sizeof(currentUsedBlkSize);
    sum += sizeof(totalUsedBlkSize);
    sum += sizeof(totalUsedRecordSize);
    sum += sizeof(deletedAddress);
    sum += sizeof(blkAccessed);
    sum += sizeof(storagePtr);
    sum += sizeof(availBlk);
    sum += sizeof(usedBlk);

    sum += sizeof(Address*)*deletedAddress.size();
    sum += sizeof(int)*blkAccessed.size();

    sum += blkNodeSize * usedBlk; 

    return sum;
}

void Storage :: display(){
    printf("--------------------------------------------------------------\n");
    printf("Storage size\t\t\t: %d\n", storageSize);
    printf("Block size\t\t\t: %d\n", blkNodeSize);
    printf("Memory used by records (MB)\t: %.5lf\n", (1.0*totalUsedRecordSize)/1000000);
    printf("Memory used by blocks (MB)\t: %.5lf\n",  (1.0*totalUsedBlkSize)/1000000);
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
        if(!temp.deleted){
            printf("\t%p %s %d\n", (char*)address.blockAddress+i, temp.tconst, temp.numVotes);
        }
        else{
            printf("\t ********Empty/Deleted********\n");
        }
    }
    printf("\n");
}

void Storage::printEveryRecordInAccessedBlock(){ 
    int count = 0;
    for(auto it : blkAccessed){
        printf("Block Number : %d\n", it);
        for(int i = 0; i < blkNodeSize; i += sizeof(Record)){
            Record temp;
            void *recordAddress = (char *) storagePtr + it * blkNodeSize;
            memcpy(&temp, (char*)recordAddress + i, sizeof(Record));
            if(!temp.deleted){
                printf("\t%s\n", temp.tconst);
            }
            else{
                printf("\t ********Empty/Deleted********\n");
            }
        }
        if(++count == 5) return;
    }
    return;
}

char* accessTConst(Address *address){
    void* memoryAdr = (char*) address->blockAddress+address->offset;
    char* tempCh =  (*(Record*)memoryAdr).tconst;
    return tempCh;
};

float accessAvgRating(Address *address){
    void* memoryAdr = (char*) address->blockAddress+address->offset;
    return (*(Record*)memoryAdr).avgRating;
};

int accessNumVotes(Address *address){
    void* memoryAdr = (char*) address->blockAddress+address->offset;
    return (*(Record*)memoryAdr).numVotes;
};
