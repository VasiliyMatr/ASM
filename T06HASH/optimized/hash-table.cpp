
#include "../headers/hash-table.hpp"

HashTable::HashTable()
{}

HashTable::~HashTable()
{
  /* Need to free strs buff */
    if (buffP_ != nullptr)
      free (buffP_); 
}

Error_t HashTable::setHashFunc( HashFunc_t hashFuncP )
{
    if (isBadPtr ((void* )hashFuncP))
        return Error_t::PTR_ERR_;

    Error_t error = reset ();

    if (error != Error_t::OK_)
        return error;

    hashFuncP_ = hashFuncP;

    return Error_t::OK_;
}

HashTableUnit_t HashTable::get( HashableData_t data2Seek )
{
    if (hashFuncP_ == nullptr)
        return { INIT_HASHABLE_DATA_VAL_ };

    size_t listId = hashFuncP_ (data2Seek) % HASH_TABLE_SIZE_;

    List* listP = hashTableP_ + listId;
    List::ListElem_t* elemP = listP->getHeadP ();

    while (elemP != nullptr)
    {
        if (!strcmp (elemP->listElemData_.hashableData_, data2Seek))
            return elemP->listElemData_;

        elemP = listP->getPrevOrNext (elemP, List::ListElemSide_t::NEXT_);
    }

    return { INIT_HASHABLE_DATA_VAL_ };
}

Error_t HashTable::add( HashTableUnit_t unit2Add )
{
    if (hashFuncP_ == nullptr)
        return Error_t::PTR_ERR_;

    size_t listId = hashFuncP_ (unit2Add.hashableData_) % HASH_TABLE_SIZE_;

    List* listP = hashTableP_ + listId;
    List::ListElem_t* elemP = listP->getHeadP ();

    while (elemP != nullptr)
    {
        if (!strcmp (elemP->listElemData_.hashableData_, unit2Add.hashableData_))
            return Error_t::VAL_EXIST_ERR_;

        elemP = listP->getPrevOrNext (elemP, List::ListElemSide_t::NEXT_);
    }

    listP->addPrevOrNext (listP->getTailP (), List::ListElemSide_t::NEXT_, unit2Add);

    ++numOfUnits_;

    return Error_t::OK_;
}

Error_t HashTable::setup (HashFunc_t hashFuncP, const char* const inFileNameP )
{
  /* Resetting at first */
    Error_t error = reset ();
    if (error != Error_t::OK_)
        return error;

  /* Initing hash function */
    if (isBadPtr ((void* )hashFuncP))
        return Error_t::PTR_ERR_;

    hashFuncP_ = hashFuncP;

  /* Reading data from file */
    size_t numOfBytes = 0;
    error = readFile2Buff (inFileNameP, &buffP_, &numOfBytes);
    if (error != Error_t::OK_)
        return error;

  /* Adding buff to hash table */
    error = buff2HashTable (numOfBytes);
    if (error != Error_t::OK_)
      return error;

  /* All is ok */
    return Error_t::OK_;
}

Error_t HashTable::buff2HashTable( const size_t numOfBytes )
{
  /* Format str for words scanning */
    static const char formatStr[] = "%*[a-zA-Z0-9]%n";

  /* buffP shift & buffP additional shift */
    int buffShift = 0;
    int buffAdditionalShift = 0;

  /* Last parced str */
    char* lastStrP = buffP_;

  /* Skipping begin spaces */
    sscanf (buffP_, " %n", &buffShift);
    lastStrP = buffP_ + buffShift;

  /* Reading until str end */
    while (sscanf (buffP_ + buffShift, formatStr, &buffAdditionalShift) != EOF)
    {

    /* Str stuff */
      /* Should read word & it's size should be less than STR_MAX_SIZE_ */
        if (buffAdditionalShift <= 0 || buffAdditionalShift > STR_MAX_SIZE_)
            return Error_t::PARCE_ERR_;
      /* Plusing additional shift */
        buffShift += buffAdditionalShift;
      /* Putting str end for comfortable work with buff */
        buffP_[buffShift++] = '\0';

    /* Hash stuff */
      /* Unit to store in hash table */
        HashTableUnit_t hashTableUnit = { lastStrP };

        Error_t error = add (hashTableUnit);
        if (error != Error_t::OK_)
            return error;

    /* Str stuff */
      /* Skippint spaces */
        sscanf (buffP_ + buffShift, " %n", &buffAdditionalShift);
        buffShift += buffAdditionalShift;

      /* Updating str ptr */
        lastStrP = buffP_ + buffShift;
      /* Nulling additional shift */
        buffAdditionalShift = 0;
    }

  /* All is ok */
    return Error_t::OK_;
}

Error_t HashTable::reset()
{
    hashFuncP_ = nullptr;

    if (buffP_ != nullptr)
    {
        free (buffP_);
        buffP_ = nullptr;
    }

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