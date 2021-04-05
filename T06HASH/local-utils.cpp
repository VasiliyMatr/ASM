
#include "local-utils.hpp"

bool isBadPtr( const void* ptr )
{
    if (ptr == nullptr)
        return true;

    return false;
}

Error_t readFile2Buff( const char* const inFileNameP,
                       const char** buffPP ,
                       size_t* sizeP )
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
    char* buffP = (char* )calloc (sizeof (char), bytesNum);
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

size_t getFileLength( FILE* const filePtr )
{
    assert (!isBadPtr (filePtr));

  /* moving to file end */
    fseek (filePtr, 0, SEEK_END);
  /* getting num of bytes */
    int bytesNum = ftell (filePtr);
  /* rewinding file back to begin */
    rewind (filePtr);

    return bytesNum;
}

int printData( char* buff, const HashTableUnit_t &hashTableUnit )
{
    int shift = 0;

    shift = sprintf (buff,
                     "\t\t"   "UNIT:" "\n"
                     "\t\t\t"     "data: %.10s..." "\n"
                     "\t\t\t"     "key : %d"       "\n" ,
                     hashTableUnit.data_, hashTableUnit.key_);
    return shift;
}