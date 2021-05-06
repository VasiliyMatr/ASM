
#include "utils.hpp"

bool isBadPtr( const void* const ptr )
{
    if (ptr == nullptr)
        return true;

    return ptr == nullptr;
}

Error_t readFile2Buff( const char*   const inFileNameP,
                             BYTE__** const buffPP ,
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
    BYTE__* buffP = (BYTE__* )calloc (sizeof (BYTE__), bytesNum + 1);
    if (buffP == nullptr)
    {
        fclose (inFileP);
        return Error_t::MEM_ERR_;
    }

  /* Reading info and closing input file */
    fread ((void* )buffP, sizeof (BYTE__), bytesNum, inFileP);
    fclose (inFileP);

  /* Values return stuff */
    *buffPP = buffP;
    *sizeP  = bytesNum;

    return Error_t::OK_;
}

/* Rewinds file back!!! */
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

/* To write buff to file */
Error_t writeBuff2File( const char*  const outFileNameP,
                        const BYTE__* const buffP,
                        const size_t       buffSize )
{
  /* Pointers check */
    if (isBadPtr (outFileNameP) || isBadPtr (buffP))
        return Error_t::PTR_ERR_;

  /* Opening out file */
    FILE* outFileP = fopen (outFileNameP, "wb");
    if (outFileP == nullptr)
        return Error_t::FILE_ERR_;

  /* Writing 2 file */
    fwrite (buffP, buffSize, sizeof (BYTE__), outFileP);
    fclose (outFileP);

    return Error_t::OK_;
}