
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include <pmmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

#include <SFML/Graphics.hpp>

/* for error codes */
enum class patchError : int {

    OK_                 = 0       ,
    FILE_ERROR_         = -0xFFFF ,
    BAD_PTR_                      ,
    MEM_ERROR_                    ,

};

/* Main patching function */
patchError makePatch( const char* inFileName, const char* outFileName );