
#include "../headers/hash-funcs.hpp"

HashTableKey_t oneValHash( HashableData_t hashableData )
{
    return 0;
}

HashTableKey_t firstLetterHash( HashableData_t hashableData )
{
    return *hashableData;
}

HashTableKey_t lettersSummHash( HashableData_t hashableData )
{
    int hash = 0;
    int hashTableDataShift = 0;

    for (; hashableData [hashTableDataShift] != '\0'; ++hashTableDataShift)
        hash += hashableData [hashTableDataShift];

    return hash;
}

HashTableKey_t lettersAvgHash( HashableData_t hashableData )
{
    return lettersSummHash (hashableData) / strlen (hashableData);
}