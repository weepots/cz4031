#include <iostream>


using namespace std;

const int tconst_size = 12;
typedef unsigned un_int;

struct Record{
    char tconst[tconst_size]; //12 byte length record ID as key
    float avgRating; //4 byte length
    int numVotes; // 4 byte length

};

struct Address{
    char* blockAddress;
    int offset;
};

struct BlkNode{
    Address address;
    int blkSize;
    struct blkNode* next;

    
};


class storage{
private:
    //Pointers
    char* storagePtr;
    char* blkPtr;
    //Attributes in bytes
    int storageSize; //storage size in bytes
    int blkNodeSize; //blk node size in bytes
    int currentUsedBlkSize; //current used blk size in bytes
    int usedBlkSize; //total used block size
    int usedRecordSize; //total used record size

    //Attributes in numbers
    int availBlk; //number of unallocated blocks
    int usedBlk; //number of allocated blocks

public:
    //initialise class
    storage(int storageSize, int blkNodeSize){
        char* storagePtr = nullptr;
        this->storagePtr = new char[storageSize]; // points to the first datablock in storage
        this->blkPtr = nullptr;

        this->storageSize = storageSize;
        this->blkNodeSize = blkNodeSize;
        this->currentUsedBlkSize = 0;
        this->usedBlkSize = 0;
        this->usedRecordSize = 0;

        this->availBlk = storageSize/blkNodeSize;
        this->usedBlk = 0;
    };
    //destroy class and clear memory
    // ~ storage();

    //get functions
    int getStorageSize(){  return storageSize; };
    int getblkNodeSize(){  return blkNodeSize; }
    int getUsedBlkSize(){  return usedBlkSize; };
    int getUsedRecordSize(){  return usedRecordSize; };
    int getAvailBlk(){ return availBlk; };
    int getUsedBlk(){ return usedBlk; };

    // // record functions
    // char writeRecord(int recordSize);
    // void deleteRecord(Address address, int recordSize);
    // bool createBlock();
    // bool emptyCheck(Address address);
};

int main(){
    storage storage(200000,100);
    cout << storage.getStorageSize() << "\n";
}