
#include "hash-table.hpp"

HashTable::HashTable()
{}

Error_t HashTable::setup (HashFunc_t hashFuncP, const char* const inFileNameP )
{
    if (isBadPtr ((void* )hashFuncP))
        return Error_t::PTR_ERR_;

    size_t numOfBytes = 0;
    char*  buffP       = nullptr;

    Error_t error = readFile2Buff (inFileNameP, &buffP, &numOfBytes);
    if (error != Error_t::OK_)
        return error;

    buff2HashTable (buffP, numOfBytes);

    hashFuncP_ = hashFuncP;

    return Error_t::OK_;
}


Error_t HashTable::buff2HashTable( char* const buffP, const size_t numOfBytes )
{
  /* Format str for words scanning */
    static const char formatStr[] = "%*[a-zA-Z]%n";
  /* buffP shift */
    int buffShift = 0;
  /* buffP additional shift value for 1 sscanf */
    int buffAdditionalShift = 0;

  /* Last parced str */
    char* lastStr = buffP;

  /* Sipping begin spaces */
    sscanf (buffP, " %n", &buffShift);

    lastStr = buffP + buffShift;

  /* Reading until file end */
    while (sscanf (buffP + buffShift, formatStr, &buffAdditionalShift) != EOF)
    {
      /* Should read word & it's size should be less than STR_MAX_SIZE_ */
        if (buffAdditionalShift <= 0 || buffAdditionalShift > STR_MAX_SIZE_)
            return Error_t::PARCE_ERR_;
        
      /* Plusing additional shift & putting str end for comfortable work with buff */
        buffShift += buffAdditionalShift;
        buffP[buffShift++] = '\0';
        
      /* Skippint spaces */
        sscanf (buffP + buffShift, " %n", &buffAdditionalShift);
        buffShift += buffAdditionalShift;

      /* Test stuff */
        printf ("%s" "\n", lastStr);
        lastStr = buffP + buffShift;

      /* Nulling additional shift */
        buffAdditionalShift = 0;
    }

    return Error_t::OK_;
}