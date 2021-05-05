
#include "utils.hpp"
#include <elf.h>

#ifndef ELF_HEADER_HPP_INCL_
#define ELF_HEADER_HPP_INCL_

/* IN / OUT / EXIT PROCS */
    static const _BYTE IN_PROC_CODE_ [] = {

        0x48, 0x83, 0xEC, 0x10,
        0x48, 0x31, 0xFF, 0x48,
        0x89, 0xE6, 0xBA, 0x0A,
        0x00, 0x00, 0x00, 0xB8,
        0x00, 0x00, 0x00, 0x00,
        0x0F, 0x05, 0xB9, 0x0A,
        0x00, 0x00, 0x00, 0x48,
        0x89, 0xC3, 0x48, 0x01,
        0xF3, 0x48, 0xFF, 0xCB,
        0xB8, 0x00, 0x00, 0x00,
        0x00, 0xBA, 0x00, 0x00,
        0x00, 0x00, 0x48, 0x39,
        0xDE, 0x74, 0x19, 0x48,
        0xF7, 0xE1, 0x48, 0x8B,
        0x3E, 0x48, 0x81, 0xE7,
        0xFF, 0x00, 0x00, 0x00,
        0x48, 0x83, 0xEF, 0x30,
        0x48, 0x01, 0xF8, 0x48,
        0xFF, 0xC6, 0xEB, 0xE2,
        0x48, 0x83, 0xC4, 0x10,
        0xC3
    };

    static const _BYTE OUT_PROC_CODE_ [] = {

        0xB9, 0x0A, 0x00, 0x00,
        0x00, 0x48, 0x31, 0xDB,
        0x6A, 0x0A, 0x48, 0xFF,
        0xC3, 0x48, 0x31, 0xD2,
        0x48, 0xF7, 0xF1, 0x48,
        0x83, 0xC2, 0x30, 0x52,
        0x48, 0x83, 0xF8, 0x00,
        0x75, 0xEC, 0x48, 0x89,
        0xDA, 0x48, 0xC1, 0xE2,
        0x03, 0x48, 0xFF, 0xC2,
        0x48, 0xFF, 0xC3, 0x48,
        0x89, 0xE6, 0x48, 0x31,
        0xFF, 0xB8, 0x01, 0x00,
        0x00, 0x00, 0x0F, 0x05,
        0x48, 0x83, 0xC4, 0x08,
        0x48, 0xFF, 0xCB, 0x48,
        0x83, 0xFB, 0x00, 0x75,
        0xF3, 0xC3
    };

    static const _BYTE EXIT_PROC_CODE_ [] = {

        0xB8, 0x3C, 0x00, 0x00,
        0x00, 0x48, 0x31, 0xFF,
        0x0F, 0x05
    };

    /* size of in & out & exit procs bin code */
    static const size_t PROCS_SIZE_ = sizeof (IN_PROC_CODE_)  +
                                      sizeof (OUT_PROC_CODE_) +
                                      sizeof (EXIT_PROC_CODE_);

    /* addrs for far calls */
    static const _DWRD IN_PROC_ADDR_    = 0x400080;
    static const _DWRD OUT_PROC_ADDR_   = IN_PROC_ADDR_ + sizeof (IN_PROC_CODE_);
    static const _DWRD EXIT_PROC_ADDR_  = OUT_PROC_ADDR_ + sizeof (OUT_PROC_CODE_);

    /* entry point address - for header */
    static const _DWRD ENTRY_POINT_ADDR_ = EXIT_PROC_ADDR_ + sizeof (EXIT_PROC_CODE_);

/* Struct with all ELF file header bytes */
class ELFHeader_t {

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
        size_t EH_ENTRY_ = ENTRY_POINT_ADDR_;

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

class ProgramHeader_t {

    /* Segment type */
        _BYTE PH_TYPE_ [4] = {

            PT_LOAD,
            0x00,
            0x00,
            0x00
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
        unsigned long long PH_FSIZE_ = 0x1000;

    /* Program header memory size */
        unsigned long long PH_MSIZE_ = 0x1000;

    /* Program header align */
        _BYTE PH_ALIGN_ [8] = {

            0x00, 0x10,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00
        };

    /* Additional zeroes */
        _BYTE PH_ZEROES_ [8] = { };

public:

    /* Setter for program header size */
    void setPHSize( size_t newSize )
    {
        PH_FSIZE_ = newSize;
        PH_MSIZE_ = newSize;
    }

};

#endif