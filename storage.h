#ifndef STORAGE_H
#define STORAGE_H

#include<vector>
using namespace std;


const int tconst_size = 10;
typedef unsigned un_int;

struct Record{
    char tconst[tconst_size]; //10 byte length record ID as key
    float avgRating; //4 byte length
    int numVotes; // 4 byte length
};

struct Address{
    char* blockAddress;
    int offset;
};

char* accessTConst(Address address);
float accessAvgRating(Address address);
int accessNumVotes(Address address);

class Storage{
private:
    //Pointers
    char *storagePtr;  // Pointer to the memory pool.
    char *blkPtr; 

    //Attributes in bytes
    int storageSize; //storage size in bytes
    int blkNodeSize; //blk node size in bytes
    int currentUsedBlkSize; //current used blk size in bytes
    int usedBlkSize; //total used block size
    int usedRecordSize; //total used record size

    //Attributes in numbers
    int availBlk; //number of unallocated blocks
    int usedBlk; //number of allocated blocks
    vector<int> blkAccessed;

public:
    //initialise class
    Storage(int storageSize, int blkNodeSize);
    //destroy class and clear memory
    ~Storage();


    //get functions
    int getStorageSize();
    int getblkNodeSize();
    int getUsedBlkSize();
    int getUsedRecordSize();
    int getAvailBlk();
    int getUsedBlk();

    // record functions
    Address writeRecord(int recordSize);
    Record readRecord(Address address);
    char* getTConst(Address address);
    float getAvgRating(Address address);
    int getNumVotes(Address address);
    void deleteRecord(Address address, int recordSize);
    bool emptyCheck(Address address);
    void insertBlkAccessed(Address address);
    int resetBlkAccessed();
    int getBlkAccessed();
    

    //display functionS
    void printEveryRecordInSameBlock(Address address);
    void printEveryRecordInAccessedBlock();
    void display();

    //
};


#endif