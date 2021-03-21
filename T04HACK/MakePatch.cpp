
#include "MakePatch.h"

    /* TODO:
     * Need to make normal isBadPtr func for Linux OS
     */
    bool isBadPtr( const void* ptr );

/* file patch stuff here */
void fileChange( char* outBuffPtr );

/* function to get file length
 * 
 * !!! REWINDS FILE TO BEGIN !!!
 */
size_t getFileLength( FILE* filePtr );

/* Main patching function */
patchError makePatch( const char* inFileName, const char* outFileName )
{
  /* input & output files names ptrs check */
    if (isBadPtr (inFileName) || isBadPtr (outFileName))
        return patchError::BAD_PTR_;

  /* opening input file */
    FILE* inFilePtr = fopen (inFileName, "rb");
    if (inFilePtr == nullptr)
        return patchError::FILE_ERROR_;

  /* getting num of bytes in input file */
    size_t bytesNum = getFileLength (inFilePtr);
    if (bytesNum == 0)
        return patchError::FILE_ERROR_;
  /* allocating memory for output buff - with patched file */
    char* outBuffPtr = (char* )calloc (sizeof (char), bytesNum);
    if (outBuffPtr == nullptr)
    {
        fclose (inFilePtr);
        return patchError::MEM_ERROR_;
    }

  /* reading info and closing input file */
    fread (outBuffPtr, sizeof (char), bytesNum, inFilePtr);
    fclose (inFilePtr);

  /* patching file */
    fileChange (outBuffPtr);

  /* opening output file */
    FILE* outFilePtr = fopen (outFileName, "wb");
    if (outFilePtr == nullptr)
        return patchError::FILE_ERROR_;

  /* writing data and closing output file */
    fwrite (outBuffPtr, sizeof (char), bytesNum, outFilePtr);
    fclose (outFilePtr);

  /* free outBuffPtr memory */
    free (outBuffPtr);

  /* all is good, patch finished */
    return patchError::OK_;
}

bool isBadPtr( const void* ptr )
{
    if (ptr == nullptr)
        return true;

    return false;
}

void fileChange( char* outBuffPtr )
{
    const size_t PATCH_STR0_BEGIN_POS_ = 0x0000;
    const char*  PATCH_STR0_ = "\xB0\xFE\xB4\x0A" "\x90\x90\x90\x90\x90\x90\x90\x90\x90";
    memcpy (outBuffPtr + PATCH_STR0_BEGIN_POS_, PATCH_STR0_, strlen (PATCH_STR0_));

    const size_t PATCH_STR1_BEGIN_POS_ = 0x01F6 - 0x0100;
    const char*  PATCH_STR1_ = "bcdefghijk";
    memcpy (outBuffPtr + PATCH_STR1_BEGIN_POS_, PATCH_STR1_, strlen (PATCH_STR1_));

    const size_t PATCH_STR2_BEGIN_POS_ = 0x025A - 0x0100;
    const char* PATCH_STR2_ = "HHHHHHHHHH";
    memcpy (outBuffPtr + PATCH_STR2_BEGIN_POS_, PATCH_STR2_, strlen (PATCH_STR2_));
}

size_t getFileLength( FILE* filePtr )
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