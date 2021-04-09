
#include "local-utils.hpp"

#ifndef HASH_FUNCS_HPP_INCL
#define HASH_FUNCS_HPP_INCL

/* always returns one value */
HashTableKey_t oneValHash( const HashableData_t& hashableData_ );

/* returns first letter value */
HashTableKey_t firstLetterHash( const HashableData_t& hashableData_ );

/* returns letters summ */
HashTableKey_t lettersSummHash( const HashableData_t& hashableData_ );

/* returns letters summ / strlen (hashableData_) */
HashTableKey_t lettersAvgHash( const HashableData_t& hashableData_ );

/* my hash func from 1 sem))) */
HashTableKey_t myHash( const HashableData_t& hashableData );

/* crc32 */
HashTableKey_t crc32Hash( const HashableData_t& hashableData );

#endif