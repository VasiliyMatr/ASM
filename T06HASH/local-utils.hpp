
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#ifndef LOCAL_UTILS_HPP_INCL_
#define LOCAL_UTILS_HPP_INCL_

enum class Error_t {

    OK_                 =  0    ,
    PTR_ERR_            = -0xFF ,
    MEM_ERR_                    ,
    FILE_ERR_                   ,
    PARCE_ERR_                  ,

};

/* Max str size */
  const size_t STR_MAX_SIZE_ = 64; 

/* Hash table storable data */
  typedef const char*     hashTableData_t;
/* Hash table key value type */
  typedef unsigned int    hashTableKey_t;

/* Init value for hashTableKey_t */
  const hashTableKey_t  INIT_KEY_T_VAL_  = 0xAB0BA;

/* Unit to store in hash table lists as element */
  struct HashTableUnit_t {
  
      hashTableData_t data_ = "";
  
  };

/* To check ptrs validity */
  bool isBadPtr( const void* ptr );

/* To read file to buff */
  Error_t readFile2Buff( const char* const inFileNameP,
                         char**  const buffPP,
                         size_t* const sizeP );

/* To get file length */
  size_t getFileLength( FILE* const filePtr );

/* Func to print hash table units in dump buffer  */
  int printData( char* buff, const HashTableUnit_t &hashTableUnit );

#endif