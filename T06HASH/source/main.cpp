
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
    hashTable.setup (lettersAvgHash     , DATA_BASE_PATH_);
    hashTable.dump  (DATA_PATH_ "outLettersAvg.txt");

    char const test_strs[][100] = {
        "anonymously",
        "dose",
        "salvers",
        "Year",
        "forandthe",
        "yardarm",
    };

    for (int i = 0; i < 1000000; ++i)
    {
        for (int j = 0; j < sizeof (test_strs) / 100; ++j)
            hashTable.get (test_strs[j]);
    }
}

#undef DATA_PATH_
#undef DATA_BASE_PATH_