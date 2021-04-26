
#include "elf-header.hpp"

    static const ELFHeader_t     ELFHeader;
    static       ProgramHeader_t programHeader;

int main()
{

    char* outBuffP = (char* )calloc (sizeof (char), 0x1000);
    if (outBuffP == nullptr)
    {
        printf ("memory error" "\n");
        return 0;
    }

    memcpy (outBuffP                          , (void* )&ELFHeader      , sizeof (ELFHeader_t));
    memcpy (outBuffP + sizeof (ELFHeader_t)   , (void* )&programHeader  , sizeof (ProgramHeader_t));

    _BYTE code [10] = { 0xB8, 0x3C, 0x00, 0x00, 0x00, 0x48, 0x31, 0xFF, 0x0F, 0x05 };

    memcpy (outBuffP + sizeof (ELFHeader_t) + sizeof (ProgramHeader_t)   , code , 10);

    writeBuff2File ("aboba", outBuffP, sizeof (ProgramHeader_t) + sizeof (ELFHeader_t) + 10);

    free (outBuffP);

    return 0;
}