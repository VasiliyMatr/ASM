
#include "hash-table.hpp"
#include "hash-funcs.hpp"

int main()
{
    HashTable hashTable;
    hashTable.setup (oneValHash         , "test.txt");
    hashTable.dump  ("outOneVal.txt"        );
    hashTable.setup (firstLetterHash    , "test.txt");
    hashTable.dump  ("outFirstLetter.txt"   );
    hashTable.setup (lettersSummHash    , "test.txt");
    hashTable.dump  ("outLettersSum.txt"    );
    hashTable.setup (lettersAvgHash     , "test.txt");
    hashTable.dump  ("outLettersAvg.txt"    );
}