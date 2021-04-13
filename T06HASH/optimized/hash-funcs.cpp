
#include "../headers/hash-funcs.hpp"
#include <nmmintrin.h>

HashTableKey_t oneValHash( const HashableData_t& hashableData )
{
    return 0;
}

HashTableKey_t firstLetterHash( const HashableData_t& hashableData )
{
    return *hashableData;
}

HashTableKey_t lettersSummHash( const HashableData_t& hashableData )
{
    int hash = 0;
    int hashTableDataShift = 0;

    for (; hashableData [hashTableDataShift] != '\0'; ++hashTableDataShift)
        hash += hashableData [hashTableDataShift];

    return hash;
}

HashTableKey_t lettersAvgHash( const HashableData_t& hashableData )
{
    return lettersSummHash (hashableData) / strlen (hashableData);
}

HashTableKey_t myHash( const HashableData_t& hashableData )
{
    const size_t byte_move = sizeof (HashTableKey_t) - 1;
    int hash = 0xAB0BA;

    for (size_t i = 0; hashableData[i] != '\0'; i++)
    {
        hash = (-hash * *(hashableData + i) + hash >> byte_move ^ hash << 1) + hash - i;
    }

    return hash;
}

HashTableKey_t crc32Hash( const HashableData_t& hashableData )
{
    int            symbolId = 0;
    char           symbol   = hashableData[0];
    HashTableKey_t hash     = 0xFFFFFFFF;

    hash = _mm_crc32_u64 (hash, *(unsigned long long*) hashableData);

    return _mm_crc32_u64 (hash, *(((unsigned long long*) hashableData) + 1));
}