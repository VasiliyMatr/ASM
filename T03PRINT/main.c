
#include <stdio.h>

extern void bruhPrint( char* formatStr, ... );

int main()
{
    /* all features test */
    bruhPrint ("I love $h$h" "!$b$d$c", 0xF0, 0x0D, 1, 1, '\n');
    bruhPrint ("$s", "BRUH!", 5);

    /* std func is working after my func */
    printf    ("\n");

    return 0;
}