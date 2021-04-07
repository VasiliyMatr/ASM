
#include "headers/hash-table.hpp"
#include "headers/hash-funcs.hpp"

#define DATA_PATH_ "data//"
#define DATA_BASE_PATH_ DATA_PATH_ "word_set.txt"

int main()
{
    HashTable hashTable;
    hashTable.setup   (crc32Hash          , DATA_BASE_PATH_);
    for (int i = 0; i < 10; ++i)
        hashTable.speedTest ();
}

#undef DATA_PATH_
#undef DATA_BASE_PATH_