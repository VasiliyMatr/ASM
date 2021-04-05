
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

    /* Used hash func */
      HashFunc_t* hashFuncP_ = nullptr;

    /* Hash table size */
      static const size_t HASH_TABLE_SIZE_ = 1003;

    /* Hash table */
      List hashTableP_ [HASH_TABLE_SIZE_];

/* Methods */
public:
    /* Ctor */
      HashTable ();

    /* Deleted stuff */
      HashTable           (const HashTable &) = delete;
      HashTable operator= (const HashTable &) = delete;

    /* Hash table setup & reset */
      Error_t setup( HashFunc_t const hashFunc , const char* const inFileName );
      Error_t reset();

    /* To get element by key */
      HashTableUnit_t  get  ( key_t key );

    /* Dump function */
      void dump( const char* const outFileName );

private:
    /* To put all words from text file to hash table */
      Error_t buff2HashTable( char* const buffP, const size_t numOfBytes );

};

#endif