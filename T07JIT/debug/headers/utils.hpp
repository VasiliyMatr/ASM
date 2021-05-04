
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#ifndef UTILS_HPP_INCL_
#define UTILS_HPP_INCL_

/* For errors info output */
enum class Error_t {

/* NO ERRORS */
  /* No errors */
    OK_                 =  0        ,

/* COMMON ERRORS */
  /* Not valid ptr */
    PTR_ERR_            = -0xFFFF   ,
  /* Error with memory allocation */
    MEM_ERR_                        ,
  /* Error with file (can't open, etc) */
    FILE_ERR_                       ,
  /* Error with cmd parse */
    CMD_ERR_                        ,

};

/* sized types */
typedef unsigned char   _BYTE; /* one byte */
typedef unsigned short  _WORD; /* two bytes */
typedef unsigned int    _DWRD; /* four bytes */

/* my binary format align type  */
typedef int _AL_TYPE;

/* To check ptrs validity */
  bool isBadPtr( const void* const ptr );

/* To read file to buff */
  Error_t readFile2Buff( const char*   const inFileNameP,
                               _BYTE** const buffPP,
                               size_t* const sizeP );

/* To write buff to file */
  Error_t writeBuff2File( const char*  const outFileNameP,
                          const _BYTE* const buffP,
                          const size_t      buffSize );

/* To get file length */
  /* Rewinds file back */
  size_t getFileLength( FILE* const fileP );

#endif