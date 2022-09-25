#define STORAGE_H

const int tconst_size = 12;
typedef unsigned un_int;

struct Record{
    char tconst[tconst_size]; //12 byte length record ID as key
    float avgRating; //4 byte length
    int numVotes; // 4 byte length

};

struct BlkNode{
    Address address;
    int blkSize;
    struct blkNode* next;

    
};


struct Address{
    char* blockAddress;
    int offset;
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
    storage(int storageSize, int blkNodeSize);
    //destroy class and clear memory
    ~ storage();


    //get functions
    int getStorageSize();
    int getblkNodeSize();
    int getUsedBlkSize();
    int getUsedRecordSize();
    int getAvailBlk();
    int getUsedBlk();

    // record functions
    char writeRecord(int recordSize);
    void deleteRecord(Address address, int recordSize);
    bool createBlock();
    bool emptyCheck(Address address);
    
    


};
