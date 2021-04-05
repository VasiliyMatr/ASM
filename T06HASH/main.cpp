
#include "hash-table.hpp"

hashTableKey_t testHash( hashTableData_t )
{
    return 0;
}

int main()
{
    HashTable hashTable;
    hashTable.setup (testHash, "test.txt");
    hashTable.dump  ("out.txt");
}