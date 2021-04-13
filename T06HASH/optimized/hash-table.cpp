
#include "../headers/hash-table.hpp"

HashTable::HashTable()
{}

HashTable::~HashTable()
{}

Error_t HashTable::setHashFunc( const HashFunc_t hashFuncP )
{
    if (isBadPtr ((void* )hashFuncP))
        return Error_t::PTR_ERR_;

    Error_t error = reset ();

    if (error != Error_t::OK_)
        return error;

    hashFuncP_ = hashFuncP;

    return Error_t::OK_;
}

HashTableUnit_t& HashTable::get( const HashableData_t& data2Seek )
{
    static HashTableUnit_t BAD_VAL_("");

    if (hashFuncP_ == nullptr)
        return BAD_VAL_;

    size_t listId = hashFuncP_ (data2Seek) % HASH_TABLE_SIZE_;

    List* listP = hashTableP_ + listId;
    register List::ListElem_t* elemP = listP->getHeadP ();

    while (elemP != nullptr)
    {
        if (!fastStrCmp (elemP->listElemData_.hashableData_, data2Seek))
            return elemP->listElemData_;

        elemP = elemP->nextP_;
    }

    return BAD_VAL_;
}

Error_t HashTable::add( const HashTableUnit_t& unit2Add )
{
    if (hashFuncP_ == nullptr)
        return Error_t::PTR_ERR_;

    size_t listId = hashFuncP_ (unit2Add.hashableData_) % HASH_TABLE_SIZE_;

    List* listP = hashTableP_ + listId;
    List::ListElem_t* elemP = listP->getHeadP ();

    while (elemP != nullptr)
    {
        if (!fastStrCmp (elemP->listElemData_.hashableData_, unit2Add.hashableData_))
            return Error_t::VAL_EXIST_ERR_;

        elemP = listP->getPrevOrNext (elemP, List::ListElemSide_t::NEXT_);
    }

    listP->addPrevOrNext (listP->getTailP (), List::ListElemSide_t::NEXT_, unit2Add);

    ++numOfUnits_;

    return Error_t::OK_;
}

Error_t HashTable::transText2Bin( const char* const inFileNameP, const char* const outFileNameP )
{
    if (isBadPtr (inFileNameP) || isBadPtr (outFileNameP))
        return Error_t::PTR_ERR_;

  /* Number of bytes in input file */
    size_t inNumOfBytes = 0;
  /* Number of bytes in output file */
    size_t outNumOfBytes = 0
    ;
  /* In buff ptr */
    char* inBuffP = nullptr;
  /* Out buff ptr */
    char* outBuffP = nullptr;

  /* Reading data from file */
    Error_t error = readFile2Buff (inFileNameP, &inBuffP, &inNumOfBytes);
    if (error != Error_t::OK_)
        return error;

    outBuffP = (char*)calloc (sizeof (char), inNumOfBytes * STR_MAX_SIZE_ / 2);
    if (outBuffP == nullptr)
    {
        free (inBuffP);
        return Error_t::MEM_ERR_;
    }

  /* Adding buff to hash table */
    error = buff2Bin (inBuffP, outBuffP, &outNumOfBytes);
    free (inBuffP);
    if (error != Error_t::OK_)
    {
        free (outBuffP);
        return error;
    }

  /* Writing to out file */
    FILE* outFileP = fopen (outFileNameP, "wb");
    if (outFileP == nullptr)
    {
        free (outBuffP);
        return Error_t::FILE_ERR_;
    }

    fwrite (outBuffP, sizeof (char), outNumOfBytes, outFileP);
    fclose (outFileP);
    free (outBuffP);

  /* All is ok */
    return Error_t::OK_;
}

Error_t HashTable::buff2Bin( const char* const inBuffP, char* outBuffP, size_t* outNumOfBytesP )
{
  /* Needed assertions */
    assert (!isBadPtr (inBuffP));
    assert (!isBadPtr (outBuffP));
    assert (!isBadPtr (outNumOfBytesP));

  /* in & outt buffs shifts */
    int inBuffShift  = 0;
    int outBuffShift = 0;

  /* additional shift for sscanf and other stuff */
    int additionalShift = 0;

  /* Skipping begin spaces */
    sscanf (inBuffP, " %n", &inBuffShift);

  /* Reading until str end */
    while (sscanf (inBuffP + inBuffShift,
                   "%32s%n",
                   outBuffP + outBuffShift, &additionalShift) != EOF)
    {

      /* Should read word & it's size should be less than STR_MAX_SIZE_ */
        if (additionalShift <= 0)
            return Error_t::PARCE_ERR_;

      /* Plusing additional shift */
        inBuffShift  += additionalShift;
        outBuffShift += additionalShift;

      /* Filling with zeroes */
        for (; outBuffShift % STR_MAX_SIZE_ != 0; ++outBuffShift)
            outBuffP[outBuffShift] = '\0';
          
      /* Skippint spaces */
        sscanf (inBuffP + inBuffShift, " %n", &additionalShift);
        inBuffShift += additionalShift;

      /* Nulling additional shift */
        additionalShift = 0;

    }

  /* All is ok */
    *outNumOfBytesP = outBuffShift;
    return Error_t::OK_;
}

Error_t HashTable::readFromBin( const char* const inFileNameP )
{
    char* inBuffP = nullptr;
    size_t numOFBytes = 0;

    Error_t error = readFile2Buff (inFileNameP, &inBuffP, &numOFBytes);
    if (error != Error_t::OK_)
        return Error_t::PARCE_ERR_;

    for (int wordId = 0; wordId < numOFBytes / STR_MAX_SIZE_; wordId++)
    {
        HashableData_t data = "";
        memcpy (data, inBuffP + wordId * STR_MAX_SIZE_, STR_MAX_SIZE_);

        error = add (HashTableUnit_t (data));
        if (error != Error_t::OK_)
        {
            reset ();
            free (inBuffP);
            return error;
        }
    }

    free (inBuffP);

    return Error_t::OK_;
}

Error_t HashTable::reset()
{
    for (size_t listId = 0; listId < HASH_TABLE_SIZE_; ++listId)
    {
        Error_t error = hashTableP_[listId].reset();
        if (error != Error_t::OK_)
          return error;
    }

    numOfUnits_ = 0;

    return Error_t::OK_;
}

Error_t HashTable::outStat ( char const * const outFileNameP )
{
    if (isBadPtr (outFileNameP))
        return Error_t::PTR_ERR_;

    char* outBuffP = (char*) calloc (sizeof (char), HASH_TABLE_SIZE_ * 20);
    if (outBuffP == nullptr)
        return Error_t::MEM_ERR_;

    size_t buffShift = 0;

    for (size_t listId = 0; listId < HASH_TABLE_SIZE_; ++listId)
        buffShift += sprintf (outBuffP + buffShift,
                              "%ld" "\n",
                              hashTableP_[listId].getSize());

    FILE* outFileP = fopen (outFileNameP, "wb");
    if (outFileP == nullptr)
    {
        free (outBuffP);
        return Error_t::MEM_ERR_;
    }

    fwrite (outBuffP, sizeof (char), buffShift, outFileP);
    fclose (outFileP);
    free (outBuffP);

    return Error_t::OK_;
}

void HashTable::speedTest()
{
    for (int listId = 0; listId < HASH_TABLE_SIZE_; ++listId)
    {
        List* listP = hashTableP_ + listId;
        List::ListElem_t* elemP = listP->getHeadP ();

        while (elemP != nullptr)
        {
            get (elemP->listElemData_.hashableData_);
            elemP = elemP->nextP_;
        }
    }
}

void HashTable::dump( const char* const outFileNameP )
{
    if (isBadPtr (outFileNameP))
        return;

  /* Sizes for allocation */
    static const size_t SYMBOLS_FOR_ONE_LIST_DUMP_ = 20;
    static const size_t SYMBOLS_FOR_ONE_UNIT_DUMP_ = 30;

    size_t dumpBuffOffset = 0;
    char* dumpBuffP = (char* )calloc (sizeof (char),
        HASH_TABLE_SIZE_ * SYMBOLS_FOR_ONE_LIST_DUMP_ +
        numOfUnits_      * SYMBOLS_FOR_ONE_UNIT_DUMP_);

    if (dumpBuffP == nullptr)
        return;

    dumpBuffOffset += sprintf (dumpBuffP, "HASH TABLE:" "\n");

    for (size_t listId = 0; listId < HASH_TABLE_SIZE_; ++listId)
        dumpBuffOffset += hashTableP_[listId].dump (dumpBuffP + dumpBuffOffset);

    FILE* outFileP = fopen (outFileNameP, "wb");
    if (outFileP == nullptr)
    {
        free (dumpBuffP);
        return;
    }

    fwrite (dumpBuffP, sizeof (char), dumpBuffOffset, outFileP);
    fclose (outFileP);
    free   (dumpBuffP);
}