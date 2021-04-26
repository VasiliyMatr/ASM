
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

    programHeader.PH_FSIZE_ = 32;
    programHeader.PH_MSIZE_ = 32;

    memcpy (outBuffP + sizeof (ELFHeader_t)   , (void* )&programHeader  , sizeof (ProgramHeader_t));

    _BYTE code [32] = { 0x6A, 0x41, 0x48, 0x89, 0xE6, 0xBA, 0x01, 0x00, 0x00, 0x00, 0xB8, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x05, 0x48, 0x83, 0xC4, 0x08, 0xB8, 0x3C, 0x00, 0x00, 0x00, 0x48, 0x31, 0xFF, 0x0F, 0x05 };

    memcpy (outBuffP + sizeof (ELFHeader_t) + sizeof (ProgramHeader_t)   , code , 32);

    writeBuff2File ("jit-out", outBuffP, sizeof (ProgramHeader_t) + sizeof (ELFHeader_t) + 32);

    free (outBuffP);

    return 0;
}