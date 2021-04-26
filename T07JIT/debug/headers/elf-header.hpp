
#include "utils.hpp"
#include <elf.h>

#ifndef ELF_HEADER_HPP_INCL_
#define ELF_HEADER_HPP_INCL_

typedef const unsigned char _BYTE;


/* Struct with all ELF file header bytes */
struct ELFHeader_t {

    /* ELF identifier */

      /* File signature */
        _BYTE EH_MAG_ [4]       = {

            0x7F, 'E', 'L', 'F'
        };

      /* File class */
        _BYTE EH_CLASS_         = ELFCLASS64;

      /* Byte order */
        _BYTE EH_BYTE_ORDER_    = ELFDATA2LSB;

      /* ELF header version */
        _BYTE EH_VERSION_       = EV_CURRENT;

      /* OS ABI extentions */
        _BYTE EH_OSABI_         = ELFOSABI_NONE;

      /* ABI Version - default one */
        _BYTE EH_ABIVERSION_    = 0;

      /* Padding bytes - all zeroes */
        _BYTE EH_PAD_ [7]       = { 0 };

    /* ELF type */
        _BYTE EH_TYPE_ [2]      = { ET_EXEC, 0 };

    /* Machine architecture */
        _BYTE EH_MACHINE_ [2]   = { EM_X86_64, 0};

    /* ELF format version */
        _BYTE EH_F_VERSION_ [4] = { EV_CURRENT, 0, 0, 0 };

    /* Entry point address */
        _BYTE EH_ENTRY_   [8]   = {

            0x80, 0x00,
            0x40, 0x00,
            0x00, 0x00,
            0x00, 0x00
        };

    /* Programm headers tablet offset - default */
        _BYTE EH_P_HEADERS_OFF_ [8] = {

            0x40, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00
        };

    /* Sections headers tablet offset - default */
        _BYTE EH_S_HEADERS_OFF_ [8] = {

            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00
        };

    /* Processor dependent flags - default */
        _BYTE EH_FLAGS_ [4] = {

            0x00, 0x00,
            0x00, 0x00
        };

    /* File header size - 64 bytes for 64 bit ELF */
        _BYTE EH_FH_SIZE_ [2] = {

            0x40, 0x00
        };

    /* Programm header size - 56 bytes for 64 bit ELF */
        _BYTE EH_PH_SIZE_ [2] = {

            0x38, 0x00
        };

    /* Number of programm headers */
        _BYTE EH_PH_NUM_ [2] = {

            0x01, 0x00
        };

    /* Section header size - 64 bytes for 64 bit ELF */
        _BYTE EH_SH_SIZE_ [2] = {

            0x40, 0x00
        };

    /* Number of sections headers */
        _BYTE EH_SH_NUM_ [2] = {

            0x00, 0x00
        };

    /* SHSTRTAB index */
        _BYTE EH_SHSTRT_ [2] = {

            0x00, 0x00
        };

};

struct ProgramHeader_t {

    /* Segment type */
        _BYTE PH_TYPE_ [4] = {

            PT_LOAD, 0x00,
            0x00   , 0x00        
        };

    /* ELF64 flags */
        _BYTE PH_FLAGS_ [4] = {

            PF_X, 0x00,
            0x00, 0x00
        };

    /* Program header offset */
        _BYTE PH_OFFSET_ [8] = {

            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00
        };

    /* Program header virtual address */
        _BYTE PH_VADDR_ [8] = {

            0x00, 0x00,
            0x40, 0x00,
            0x00, 0x00,
            0x00, 0x00
        };

    /* Program header physical address */
        _BYTE PH_PADDR_ [8] = {

            0x00, 0x00,
            0x40, 0x00,
            0x00, 0x00,
            0x00, 0x00
        };

    /* Program header file size */
        unsigned long long PH_FSIZE_ = 0;

    /* Program header memory size */
        unsigned long long PH_MSIZE_ = 0;

    /* Program header align */
        _BYTE PH_ALIGN_ [8] = {

            0x00, 0x10,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00
        };

    /* Additional zeroes */
        _BYTE PH_ZEROES_ [8] = { };

};

#endif