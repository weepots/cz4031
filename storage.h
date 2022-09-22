#define STORAGE_H

const int tconst_size = 10;
typedef unsigned un_int;

struct Record{
    char tconst[tconst_size]; //10 byte length
    float avgRating;
    int numVotes;

};

struct blkNode{
    char address;
    int blkSize;
    struct blkNode* next;
};



class storage{
private:
    //Pointers
    char* storagePtr;
    char* blkPtr;
    //Attributes in bytes
    int storageSize;
    int blkNodeSize;
    int currentblkNodeSize;
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
    void deleteRecord(char address);
    bool createBlock();
    
    


};