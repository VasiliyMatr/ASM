
#include <stdlib.h>
#include <assert.h>

#ifndef HASH_TABLE_HPP_INCL_
#define HASH_TABLE_HPP_INCL_

#include "list.hpp"

/* Hash table class */
class HashTable {
  
/* Data types */
public:

    /* Hash func type */
      typedef hashTableKey_t HashFunc_t (hashTableData_t);

/* Fields */
private:

    /* Hash table size */
      static const size_t HASH_TABLE_SIZE_ = 1003;

    /* Used hash func */
      HashFunc_t* hashFuncP_ = nullptr;
    
    /* Buff with strs */
      char* buffP_ = nullptr;

    /* Hash table */
      List hashTableP_ [HASH_TABLE_SIZE_];

/* Methods */
public:
    /* Ctor */
      HashTable ();

    /* Dtor */
     ~HashTable ();

    /* Deleted stuff */
      HashTable           (const HashTable &) = delete;
      HashTable operator= (const HashTable &) = delete;

    /* Hash table setup & reset */
      Error_t setup( HashFunc_t const hashFunc , const char* const inFileName );
      Error_t reset();

    /* To get element by key */
      HashTableUnit_t  get  ( hashTableData_t hashTableUnitData );
    /* To add element */
      Error_t          add  ( HashTableUnit_t hashTableUnit );

    /* Dump function */
      void dump( const char* const outFileName );

private:
    /* To put all words from text file to hash table */
      Error_t buff2HashTable( const size_t numOfBytes );

};

#endif