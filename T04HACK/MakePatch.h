
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* for error codes */
enum class patchError : int {

    OK_                 = 0       ,
    FILE_ERROR_         = -0xFFFF ,
    BAD_PTR_                      ,
    MEM_ERROR_                    ,

};

/* Main patching function */
patchError makePatch( const char* inFileName, const char* outFileName );