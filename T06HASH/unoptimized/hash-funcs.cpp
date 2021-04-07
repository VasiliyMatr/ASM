
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

HashTableKey_t myHash( HashableData_t hashableData )
{
    const size_t byte_move = sizeof (HashTableKey_t) - 1;
    int hash = 0xAB0BA;

    for (size_t i = 0; hashableData[i] != '\0'; i++)
    {
        hash = (-hash * *(hashableData + i) + hash >> byte_move ^ hash << 1) + hash - i;
    }

    return hash;
}

HashTableKey_t crc32Hash( HashableData_t hashableData )
{
  /* Not initing data for speed */
    unsigned int crc32Table[256];
    unsigned int crc32Hash;
    int i, j;

    int len = strlen (hashableData);

    for (i = 0; i < 256; i++)
    {
        crc32Hash = i;
        for (j = 0; j < 8; j++)
            crc32Hash = crc32Hash & 1 ? (crc32Hash >> 1) ^ 0xEDB88320UL : crc32Hash >> 1;

        crc32Table[i] = crc32Hash;
    };

    crc32Hash = 0xFFFFFFFFUL;

    while (len--)
        crc32Hash = crc32Table[(crc32Hash ^ *hashableData++) & 0xFF] ^ (crc32Hash >> 8);

    return crc32Hash ^ 0xFFFFFFFFUL;
}