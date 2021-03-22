
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

/* just to perform joke */
void outPatchInfo();

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

  /* printing patch info */
    outPatchInfo ();

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

  /* num of patch pastes */
    const size_t       PATCH_PASTES_NUMBER_                              = 3;
  /* addrs to add pastes */
    const size_t       PATCH_PASTES_POSES_  [PATCH_PASTES_NUMBER_]       = { 0x0000, 0x00F6, 0x015A };
  /* patch pastes to add to code */
    const char         PATCH_PASTES_        [PATCH_PASTES_NUMBER_][100]  = {

        "\xB0\xFE\xB4\x0A" "\x90\x90\x90\x90\x90\x90\x90\x90\x90" ,
        "bcdefghijk"                                              ,
        "HHHHHHHHHH"

     };

  /* pasting */
    for (size_t pasteID = 0; pasteID < PATCH_PASTES_NUMBER_; ++pasteID)
        memcpy (PATCH_PASTES_POSES_[pasteID] + outBuffPtr ,
                PATCH_PASTES_[pasteID]                    ,
                strlen (PATCH_PASTES_[pasteID]));
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

void outPatchInfo()
{
    printf ("Hacking ..." "\n")     , fflush (stdout), sleep (3);
    printf ("I'm in  ... ")         , fflush (stdout), sleep (1.5);
    printf ("Your mom"    "\n\n");

    printf ("Patch version: 1.3.3.7." "\n\n");

    printf ("Patched by Vasiliy" "\n");
}