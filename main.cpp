#include <iostream>
#include <fstream>
#include "storage.h"

using namespace std;


int main(){
    int input, block_size;

    cout<< "Block size: \n 1. 200 Bytes \n 2. 500 Bytes \n 3. Quit" << endl;
    cin >> input;
    switch (input){
        case 1:
            block_size = 200;
            break;
        case 2:
            block_size = 500;
            break;
        default:
            return 0;
    }

    storage storage(200000000, block_size);
    // storage storage(20,5);

    cout << storage.getAvailBlk() <<endl;


    cout << block_size <<endl;
}
