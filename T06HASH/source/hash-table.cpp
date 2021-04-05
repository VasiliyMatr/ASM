
#include "../headers/hash-table.hpp"

HashTable::HashTable()
{}

HashTable::~HashTable()
{
  /* Need to free strs buff */
    if (buffP_ != nullptr)
      free (buffP_); 
}

HashTableUnit_t HashTable::get( hashTableData_t data2Seek )
{
    size_t listId = hashFuncP_ (data2Seek) % HASH_TABLE_SIZE_;

    List* listP = hashTableP_ + listId;
    List::ListElem_t* elemP = listP->getHead ();

    while (elemP != nullptr)
    {
        if (!strcmp (elemP->data_.data_, data2Seek))
            return elemP->data_;

        elemP = listP->getPrevOrNext (elemP, List::listElemSide_t::NEXT_);
    }

    return { "" };
}

Error_t HashTable::add( HashTableUnit_t unit2Add )
{
    size_t listId = hashFuncP_ (unit2Add.data_) % HASH_TABLE_SIZE_;

    List* listP = hashTableP_ + listId;
    List::ListElem_t* elemP = listP->getHead ();

    while (elemP != nullptr)
    {
        if (!strcmp (elemP->data_.data_, unit2Add.data_))
            return Error_t::VAL_EXIST_ERR_;

        elemP = listP->getPrevOrNext (elemP, List::listElemSide_t::NEXT_);
    }

    listP->addPrevOrNext (listP->getTail (), List::listElemSide_t::NEXT_, unit2Add);

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
    static const char formatStr[] = "%*[a-zA-Z]%n";
  /* buffP shift */
    int buffShift = 0;
  /* buffP additional shift value for 1 sscanf */
    int buffAdditionalShift = 0;

  /* Last parced str */
    char* lastStr = buffP_;

  /* Skipping begin spaces */
    sscanf (buffP_, " %n", &buffShift);
    lastStr = buffP_ + buffShift;

  /* Reading until file end */
    while (sscanf (buffP_ + buffShift, formatStr, &buffAdditionalShift) != EOF)
    {
      /* Should read word & it's size should be less than STR_MAX_SIZE_ */
        if (buffAdditionalShift <= 0 || buffAdditionalShift > STR_MAX_SIZE_)
            return Error_t::PARCE_ERR_;

      /* Unit to store in hash table */
        HashTableUnit_t hashTableUnit = { lastStr };

      /* Counting list id */
        size_t listId = hashFuncP_ (lastStr) % HASH_TABLE_SIZE_;

      /* Getting needed list tail */
        List::ListElem_t* needListTail = hashTableP_[listId].getTail ();

      /* Adding unit */
        hashTableP_[listId].
        addPrevOrNext (needListTail, List::listElemSide_t::NEXT_, hashTableUnit);

      /* Plusing additional shift & putting str end for comfortable work with buff */
        buffShift += buffAdditionalShift;
        buffP_[buffShift++] = '\0';
        
      /* Skippint spaces */
        sscanf (buffP_ + buffShift, " %n", &buffAdditionalShift);
        buffShift += buffAdditionalShift;

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

    return Error_t::OK_;
}

void HashTable::dump( const char* const outFileName )
{
    if (isBadPtr (outFileName))
        return;

    size_t dumpBuffOffset = 0;
    char* dumpBuffP = (char* )calloc (sizeof (char), HASH_TABLE_SIZE_ * 100);
    if (dumpBuffP == nullptr)
        return;

    dumpBuffOffset += sprintf (dumpBuffP, "HASH TABLE:" "\n");

    for (size_t listId = 0; listId < HASH_TABLE_SIZE_; ++listId)
        dumpBuffOffset += hashTableP_[listId].dump (dumpBuffP + dumpBuffOffset);

    FILE* outFileP = fopen (outFileName, "wb");
    if (outFileP == nullptr)
    {
        free (dumpBuffP);
        return;
    }

    fwrite (dumpBuffP, sizeof (char), dumpBuffOffset, outFileP);
    fclose (outFileP);
    free   (dumpBuffP);
}