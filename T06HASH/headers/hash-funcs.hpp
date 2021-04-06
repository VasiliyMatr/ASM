
#include "local-utils.hpp"

#ifndef HASH_FUNCS_HPP_INCL
#define HASH_FUNCS_HPP_INCL

/* always returns one value */
HashTableKey_t oneValHash( HashableData_t hashableData_ );

/* returns first letter value */
HashTableKey_t firstLetterHash( HashableData_t hashableData_ );

/* returns letters summ */
HashTableKey_t lettersSummHash( HashableData_t hashableData_ );

/* returns letters summ / strlen (hashableData_) */
HashTableKey_t lettersAvgHash( HashableData_t hashableData_ );

/* my hash func from 1 sem))) */
HashTableKey_t myHash( HashableData_t hashableData );

/* crc32 */
HashTableKey_t crc32Hash( HashableData_t hashableData );

#endif