
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#ifndef LOCAL_UTILS_HPP_INCL_
#define LOCAL_UTILS_HPP_INCL_

/* For errors info output */
enum class Error_t {

/* NO ERRORS */
  /* No errors */
    OK_                 =  0    ,

/* COMMON ERRORS */
  /* Not valid ptr */
    PTR_ERR_            = -0xFF ,
  /* Error with memory allocation */
    MEM_ERR_                    ,
  /* Error with file (can't open, etc) */
    FILE_ERR_                   ,

/* HASH TABLE ERRORS */
  /* Error with hash table loading from file - wrong characters found */
    PARCE_ERR_                  ,
  /* Tried to add data with hashable value that is already exist in table */
    VAL_EXIST_ERR_              ,

};

/* Max str size - it is limited for really fast optimizations */
  const size_t STR_MAX_SIZE_ = 64; 

/* Hash table hashable data - it is used to seek for needed data in table */
  typedef char            HashableData_t [STR_MAX_SIZE_];
/* Hash table key value type */
  typedef unsigned int    HashTableKey_t;

/* Init value for HashTableKey_t */
  const HashTableKey_t  INIT_KEY_T_VAL_  = 0xAB0BA;
/* Iuit value for HashableData_t - can't be met in table */
  const HashableData_t  INIT_HASHABLE_DATA_VAL_ = "";

/* Unit to store in hash table lists as element */
  struct HashTableUnit_t {

      /* Data, that is used for search in table */
      HashableData_t hashableData_ = "";

      /* Other data can be placed here */
  
      /* Needed methods */
      HashTableUnit_t operator=( const HashTableUnit_t& rval);
      HashTableUnit_t operator=( char* rval );

  };

/* To check ptrs validity */
  bool isBadPtr( const void* const ptr );

/* To read file to buff */
  Error_t readFile2Buff( const char*   const inFileNameP,
                               char**  const buffPP,
                               size_t* const sizeP );

/* To get file length */
  size_t getFileLength( FILE* const fileP );

/* Func to print hash table units in dump buffer  */
  int printData( char* buffP, const HashTableUnit_t &hashTableUnit );

#endif