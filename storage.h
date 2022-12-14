#ifndef STORAGE_H
#define STORAGE_H

#include<vector>
using namespace std;

const int tconst_size = 11;

struct Record{
    float avgRating ;           //4 bytes length
    int numVotes ;              // 4 bytes length
    char tconst[tconst_size];   //11 bytes length record ID as key
    bool deleted = true;        //1 bytes length
};

//Size of record should be 19 but because of memory positioning. Record will be a 20bytes struct

struct Address{
    char*   blockAddress; //
    int     offset;
};

char* accessTConst(Address *address);
float accessAvgRating(Address *address);
int accessNumVotes(Address *address);

class Storage{
private:
    //Pointers
    char *storagePtr;   // Pointer to the memory pool.
    char *blkPtr;       // Pointer to current block.

    //Attributes
    int storageSize;        //storage size in bytes
    int blkNodeSize;        //blk node size in bytes
    int currentUsedBlkSize; //current used blk size in bytes
    int totalUsedBlkSize;   //total used block size
    int totalUsedRecordSize;//total used record size
    vector<Address *> deletedAddress; 
    vector<int> blkAccessed;
    // vector that stores address where a record has been deleted

    int availBlk; //number of unallocated blocks
    int usedBlk; //number of allocated blocks
public:
    //initialise class
    Storage(int storageSize, int blkNodeSize);
    //destroy class and clear memory
    ~Storage();


    //get functions
    int getStorageSize();
    int getblkNodeSize();
    int getTotalUsedBlkSize();
    int getTotalUsedRecordSize();
    int getAvailBlk();
    int getUsedBlk();
    int getBlkNo(Address *address);

    // record functions
    Address writeRecord(Record& record, int recordSize);
    Record readRecord(Address *address, int recordSize);
    char* getTConst(Address *address);
    float getAvgRating(Address *address);
    int getNumVotes(Address *address);
    bool getDeleted(Address *address);
    void deleteRecord(Address *address, int recordSize);
    bool emptyCheck(Address *address, int recordSize);
    void insertBlkAccessed(Address *address);
    int resetBlkAccessed();
    int getBlkAccessed();
    int getActualMemoryUsed();
    

    //display functionS
    void printEveryRecordInSameBlock(Address address);
    void printEveryRecordInAccessedBlock();
    void display();

    //
};


#endif