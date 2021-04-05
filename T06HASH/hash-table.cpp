
#include "hash-table.hpp"

HashTable::HashTable()
{}

Error_t HashTable::setup (HashFunc_t hashFuncP, const char* const inFileNameP )
{
    if (isBadPtr (hashFuncP))
        return Error_t::PTR_ERR_;

    size_t numOfBytes = 0;
    char*  buffP       = nullptr;

    Error_t error = readFile2Buff (inFileNameP, &buffP, &numOfBytes);
    if (error != Error_t::OK_)
        return error;

    buff2HashTable (buffP, numOfBytes);

    hashFuncP_ = hashFuncP;
}


Error_t HashTable::buff2HashTable( const char* const buffP, const size_t numOfBytes )
{
  /* Format str for words scanning */
    static const char formatStr[] = "%*s[a-zA-Z] %n";
  /* buffP shift */
    size_t buffShift = 0;
  /* buffP additional shift value for 1 sscanf */
    size_t buffAdditionalShift = 0;

  /* Sipping begin spaces */
    sscanf (buffP, " %n", &buffShift);

  /* Reading until file end */
    while (sscanf (buffP + buffShift, formatStr, &buffAdditionalShift) != EOF)
    { 
        if (buffAdditionalShift == 0)
            return Error_t::PARCE_ERR_;

        printf ("%.10s" "\n", buffP + buffShift);

        buffShift += buffAdditionalShift;
        buffAdditionalShift = 0;
    }

    buffShift += buffAdditionalShift;

    return Error_t::OK_;
}