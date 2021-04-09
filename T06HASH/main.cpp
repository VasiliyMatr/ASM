
#include "headers/hash-table.hpp"
#include "headers/hash-funcs.hpp"

#define DATA_PATH_ "data//"
#define DATA_BASE_PATH_ DATA_PATH_ "word_set.txt"

int main()
{
    HashTable hashTable;
    hashTable.setHashFunc (crc32Hash);
    hashTable.transText2Bin ("text.txt", "out.hs");
    Error_t error = hashTable.readFromBin (DATA_PATH_ "word_set.hs");
    error = hashTable.readFromBin ("out.hs");

    HashableData_t aboba = "aboba";

    printf ("%s", hashTable.get (aboba));
}

#undef DATA_PATH_
#undef DATA_BASE_PATH_