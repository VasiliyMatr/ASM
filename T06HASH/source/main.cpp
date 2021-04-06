
#include "../headers/hash-table.hpp"
#include "../headers/hash-funcs.hpp"

#define DATA_PATH_ "data//"
#define DATA_BASE_PATH_ DATA_PATH_ "word_set.txt"

int main()
{
    HashTable hashTable;
    // hashTable.setup (oneValHash         , DATA_BASE_PATH_);
    // hashTable.dump  (DATA_PATH_ "outOneVal.txt");
    // hashTable.setup (firstLetterHash    , DATA_BASE_PATH_);
    // hashTable.dump  (DATA_PATH_ "outFirstLetter.txt");
    // hashTable.setup (lettersSummHash    , DATA_BASE_PATH_);
    // hashTable.dump  (DATA_PATH_ "outLettersSum.txt");
    hashTable.setup   (lettersAvgHash     , DATA_BASE_PATH_);
    hashTable.outStat (DATA_PATH_ "lettersAvgStat.txt");
}

#undef DATA_PATH_
#undef DATA_BASE_PATH_