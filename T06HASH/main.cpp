
#include "headers/hash-table.hpp"
#include "headers/hash-funcs.hpp"

#define DATA_PATH_ "data//"
#define DATA_BASE_PATH_ DATA_PATH_ "word_set.hs"

int main()
{
    HashTable hashTable;
    hashTable.setHashFunc (crc32Hash);
    hashTable.readFromBin (DATA_BASE_PATH_);

    for (int i = 0; i < 1000; ++i)
        hashTable.speedTest ();
}

#undef DATA_PATH_
#undef DATA_BASE_PATH_