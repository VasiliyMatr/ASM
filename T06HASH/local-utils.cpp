
#include "local-utils.hpp"

int printData( char* buff, const int &data )
{
    int shift = 0;

    sprintf (buff, "\t\t%d" "\n" "%n", data, &shift);
    return shift;
}