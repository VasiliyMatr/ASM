
#include "hash-funcs.hpp"

hashTableKey_t oneValHash( hashTableData_t hashTableData )
{
    return 0;
}

hashTableKey_t firstLetterHash( hashTableData_t hashTableData )
{
    return *hashTableData;
}

hashTableKey_t lettersSummHash( hashTableData_t hashTableData )
{
    int hash = 0;
    int hashTableDataShift = 0;

    for (; hashTableData [hashTableDataShift] != '\0'; ++hashTableDataShift)
        hash += hashTableData [hashTableDataShift];

    return hash;
}

hashTableKey_t lettersAvgHash( hashTableData_t hashTableData )
{
    return lettersSummHash (hashTableData) / strlen (hashTableData);
}