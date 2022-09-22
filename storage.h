#define STORAGE_H

const int tconst_size = 12;
typedef unsigned un_int;

struct Record{
    char tconst[tconst_size]; //12 byte length
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
    int storageSize;
    int blkNodeSize;
    int currentUsedBlkSize;
    int usedBlkSize;
    int usedRecordSize;

    //Attributes in numbers
    int availBlk;
    int usedBlk;

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
    void deleteRecord(Address address);
    bool createBlock();
    
    


};