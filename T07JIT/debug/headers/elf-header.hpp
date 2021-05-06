
#include "utils.hpp"
#include <elf.h>

#ifndef ELF_HEADER_HPP_INCL_
#define ELF_HEADER_HPP_INCL_

/* IN / OUT / EXIT PROCS */
    /* input proc bytecode */
    static const BYTE__ IN_PROC_CODE_ [] = {

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

    /* output proc bytecode */
    static const BYTE__ OUT_PROC_CODE_ [] = {

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

    /* exit proc bytecode */
    static const BYTE__ EXIT_PROC_CODE_ [] = {

        0xB8, 0x3C, 0x00, 0x00,
        0x00, 0x48, 0x31, 0xFF,
        0x0F, 0x05
    };

    /* addrs for calls (and entry point calc) */
    static const DWRD__ IN_PROC_ADDR_ = 0x00;
    static const DWRD__ OUT_PROC_ADDR_ = IN_PROC_ADDR_ + sizeof (IN_PROC_CODE_);
    static const DWRD__ EXIT_PROC_ADDR_ = OUT_PROC_ADDR_ + sizeof (OUT_PROC_CODE_);

/* progr load virtual offset to entry point */
    static const DWRD__ CODE_OFFSET_ = EXIT_PROC_ADDR_ + sizeof (EXIT_PROC_CODE_) + 0x80;

/* progr load virtual addres */
    static const QWRD__ LOAD_VIRT_ADDR_ = 0x400000;

/* Struct with all ELF file header bytes */
class ELFHeader_t {

    /* ELF identifier */

      /* File signature */
        BYTE__ EH_MAG_ [4]       = {

            0x7F, 'E', 'L', 'F'
        };

      /* File class */
        BYTE__ EH_CLASS_         = ELFCLASS64;

      /* Byte order */
        BYTE__ EHBYTE___ORDER_    = ELFDATA2LSB;

      /* ELF header version */
        BYTE__ EH_VERSION_       = EV_CURRENT;

      /* OS ABI extentions */
        BYTE__ EH_OSABI_         = ELFOSABI_NONE;

      /* ABI Version - default */
        BYTE__ EH_ABIVERSION_    = 0x00;

      /* Padding bytes - all zeroes */
        BYTE__ EH_PAD_ [7]       = { 0x00 };

    /* ELF type */
        WORD__ EH_TYPE_          = ET_EXEC;

    /* Machine architecture */
        WORD__ EH_MACHINE_       = EM_X86_64;

    /* ELF format version */
        DWRD__ EH_F_VERSION_     = EV_CURRENT;

    /* Entry point address */
        QWRD__ EH_ENTRY_         = LOAD_VIRT_ADDR_ + CODE_OFFSET_;

    /* Programm headers tablet offset - default */
        QWRD__ EH_P_HEADERS_OFF_ = 0x40;

    /* Sections headers tablet offset - default */
        QWRD__ EH_S_HEADERS_OFF_ = 0x00;

    /* Processor dependent flags - default */
        DWRD__ EH_FLAGS_         = 0x00;

    /* File header size - 64 bytes for 64 bit ELF */
        WORD__ EH_FH_SIZE_       = 0x40;

    /* Programm header size - 56 bytes for 64 bit ELF */
        WORD__ EH_PH_SIZE_       = 0x38;

    /* Number of programm headers */
        WORD__ EH_PH_NUM_        = 0x01;

    /* Section header size - 64 bytes for 64 bit ELF */
        WORD__ EH_SH_SIZE_       = 0x40;

    /* Number of sections headers */
        WORD__ EH_SH_NUM_        = 0x00;

    /* SHSTRTAB index */
        WORD__ EH_SHSTRT_        = 0x00;
};

class ProgramHeader_t {

    /* Segment type */
        DWRD__ PH_TYPE_      = PT_LOAD;

    /* ELF64 flags */
        DWRD__ PH_FLAGS_     = PF_X;

    /* Program header offset */
        QWRD__ PH_OFFSET_    = 0x00;

    /* Load virtual address */
        QWRD__ PH_VADDR_     = LOAD_VIRT_ADDR_;

    /* Load physical address */
        QWRD__ PH_PADDR_     = LOAD_VIRT_ADDR_;

    /* Program header file size - just big enough */
        QWRD__ PH_FSIZE_     = 0x1000;

    /* Program header memory size - equal to file size */
        QWRD__ PH_MSIZE_     = 0x1000;

    /* Program header align */
        QWRD__ PH_ALIGN_     = 0x1000;

    /* Additional zeroes for align */
        BYTE__ PH_ZEROES_ [8] = { };

public:

    /* Setter for program header size */
    void setPHSize( QWRD__ newSize )
    {
        PH_FSIZE_ = newSize;
        PH_MSIZE_ = newSize;
    }

};

#endif