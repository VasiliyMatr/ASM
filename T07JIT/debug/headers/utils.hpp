
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
typedef unsigned char       BYTE__; /* one byte */
typedef unsigned short      WORD__; /* two bytes */
typedef unsigned int        DWRD__; /* four bytes */
typedef unsigned long long  QWRD__; /* eight bytes */

/* my binary format align type  */
typedef unsigned int AL_TYPE__;

/* To check ptrs validity */
  bool isBadPtr( const void* const ptr );

/* To read file to buff */
  Error_t readFile2Buff( const char*   const inFileNameP,
                               BYTE__** const buffPP,
                               size_t* const sizeP );

/* To write buff to file */
  Error_t writeBuff2File( const char*  const outFileNameP,
                          const BYTE__* const buffP,
                          const size_t      buffSize );

/* To get file length */
  /* Rewinds file back */
  size_t getFileLength( FILE* const fileP );

#endif