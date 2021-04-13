
#include "../headers/local-utils.hpp"

/* TODO: normal ptr validity check */
bool isBadPtr( const void* const ptr )
{
    if (ptr == nullptr)
        return true;

    return ptr == nullptr;
}

Error_t readFile2Buff( const char*   const inFileNameP,
                             char**  const buffPP ,
                             size_t* const sizeP )
{
  /* Pointers check */
    if (isBadPtr (inFileNameP) || isBadPtr (buffPP) || isBadPtr (sizeP))
        return Error_t::PTR_ERR_;

  /* Opening input file */
    FILE* inFileP = fopen (inFileNameP, "rb");
    if (inFileP == nullptr)
        return Error_t::FILE_ERR_;

  /* Getting num of bytes in input file */
    size_t bytesNum = getFileLength (inFileP);
    if (bytesNum == 0)
        return Error_t::FILE_ERR_;

  /* Allocating memory for buff */
    char* buffP = (char* )calloc (sizeof (char), bytesNum + 1);
    if (buffP == nullptr)
    {
        fclose (inFileP);
        return Error_t::MEM_ERR_;
    }

  /* Reading info and closing input file */
    fread (buffP, sizeof (char), bytesNum, inFileP);
    fclose (inFileP);

  /* Values return stuff */
    *buffPP = buffP;
    *sizeP  = bytesNum;

    return Error_t::OK_;
}

size_t getFileLength( FILE* const fileP )
{
    assert (!isBadPtr (fileP));

  /* Moving to file end */
    fseek (fileP, 0, SEEK_END);
  /* Getting num of bytes */
    int bytesNum = ftell (fileP);
  /* Rewinding file back to begin */
    rewind (fileP);

    return bytesNum;
}

int printData( char* buffP, const HashTableUnit_t &hashTableUnit )
{
    int shift = 0;

    shift = sprintf (buffP,
                     "\t\t"   "UNIT:" "\n"
                     "\t\t\t"     "data: %.5s..." "\n",
                     hashTableUnit.hashableData_);
    return shift;
}

HashTableUnit_t::HashTableUnit_t( const HashTableUnit_t& rvalue )
{
    strcpy (hashableData_, rvalue.hashableData_);
}

HashTableUnit_t HashTableUnit_t::operator=( const HashTableUnit_t& rvalue )
{
    strcpy (hashableData_, rvalue.hashableData_);

    return *this;
}

HashTableUnit_t::HashTableUnit_t( const HashableData_t& hashableData )
{
    strcpy (hashableData_, hashableData);
}

HashTableUnit_t::HashTableUnit_t( const char* str )
{
    assert (!isBadPtr (str));

    strcpy (hashableData_, str);
}

int fastStrCmp( const HashableData_t& str1, const HashableData_t& str2 )
{
    static const int mask = _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH;

    __m128i str1Part = _mm_set_epi64x (*(long long*)str1, *((long long*)str1 + 1));
    __m128i str2Part = _mm_set_epi64x (*(long long*)str2, *((long long*)str2 + 1));

    str1Part = _mm_cmpeq_epi64 (str1Part, str2Part);
    register size_t result = _mm_movemask_epi8 (str1Part);

    if (result != 0xffff)
        return 1;

    str1Part = _mm_set_epi64x (*((long long*)str1 + 2), *((long long*)str1 + 3));
    str2Part = _mm_set_epi64x (*((long long*)str2 + 2), *((long long*)str2 + 3));

    str1Part = _mm_cmpeq_epi64 (str1Part, str2Part);
    return _mm_movemask_epi8 (str1Part) != 0xffff;
}