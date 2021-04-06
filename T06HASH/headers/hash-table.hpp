
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
      typedef HashTableKey_t HashFunc_t (HashableData_t);

/* Fields */
private:

    /* Hash table size */
      static const size_t HASH_TABLE_SIZE_ = 1003;

    /* Used hash func */
      const  HashFunc_t* hashFuncP_ = nullptr;
    
    /* Buff with strs */
      char*  buffP_ = nullptr;

    /* Hash table */
      List   hashTableP_ [HASH_TABLE_SIZE_];

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
      Error_t setup( const HashFunc_t hashFunc , const char* const inFileName );
      Error_t reset();

    /* To get unit by hashableData */
      HashTableUnit_t  get  ( HashableData_t  data2Seek );
    /* To add unit */
      Error_t          add  ( HashTableUnit_t unit2Add );

    /* Dump function */
      void dump( const char* const outFileName );

private:
    /* To put all words from text file to hash table */
      Error_t buff2HashTable( const size_t numOfBytes );

};

#endif