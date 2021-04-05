
#include "local-utils.hpp"

#ifndef HASH_FUNCS_HPP_INCL
#define HASH_FUNCS_HPP_INCL

/* always returns one value */
hashTableKey_t oneValHash( hashTableData_t hashTableData );

/* returns first letter value */
hashTableKey_t firstLetterHash( hashTableData_t hashTableData );

/* returns letters summ */
hashTableKey_t lettersSummHash( hashTableData_t hashTableData );

/* returns letters summ / strlen (hashTableData) */
hashTableKey_t lettersAvgHash( hashTableData_t hashTableData );

#endif