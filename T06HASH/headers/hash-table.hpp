
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
      typedef HashTableKey_t HashFunc_t( const HashableData_t& hashableData );

/* Fields */
private:

    /* Hash table size */
      static const size_t HASH_TABLE_SIZE_ = 1013;

    /* Used hash func */
      const  HashFunc_t* hashFuncP_ = nullptr;
    
    /* Num of units in table */
      size_t numOfUnits_ = 0;

    /* Hash table */
      List   hashTableP_ [HASH_TABLE_SIZE_];

/* Methods */
public:
    /* Ctor */
      HashTable ();

    /* Dtor */
     ~HashTable ();

    /* Deleted stuff */
      HashTable          ( const HashTable & ) = delete;
      HashTable operator=( const HashTable & ) = delete;

    /* To set hash func */
      Error_t setHashFunc( const HashFunc_t hashFunc );

    /* To transform text file to bin file */
      Error_t transText2Bin( const char* const inFileName, const char* const outFileName );
    /* To add elements form binary file */
      Error_t readFromBin( const char* const inFileName );
    /* All hash table units free */
      Error_t reset();

    /* To get unit by hashableData */
      HashTableUnit_t& get( const HashableData_t&  data2Seek );
    /* To add unit */
      Error_t add( const HashTableUnit_t& unit2Add );

    /* To out statistics of hash func */
      Error_t outStat( char const * const outFileNameP );
    /* For speed tests */
      void speedTest();

    /* Dump function */
      void dump( const char* const outFileName );

private:
    /* To put all words from text buff to bin buff */
      Error_t buff2Bin( const char* const inBuffP, char* outBuffP, size_t* outNumOfBytesP );

};

#endif