
#include "elf-header.hpp"
#include "dsl.hpp"

#ifndef JIT_HPP_INCL_
#define JIT_HPP_INCL_

class JITCompiler
{
/* Structs */
public:
  /* struct with cmd locations info (in my bin code and in x86 bin code) */
    struct LocInfo_t
    {
      /* my format loc */
          size_t inLoc_ = 0;
      /* x86-64 format loc */
          size_t outLoc_ = 0;
    };

/* Fields */
private:

  /* Where to write executable bin code */
    static const size_t HEADERS_SIZE_ = sizeof (ELFHeader_t) + sizeof (ProgramHeader_t);
  /* Max translated cmd size */
    static const size_t MAX_CMD_SIZE_ = 0x40;

  /* Headers for file */
    ELFHeader_t ELFHeader_;
    ProgramHeader_t programHeader_;

  /* In & out buffers pointers */
    AL_TYPE__ * inBuffP_ = nullptr;
    BYTE__ * outBuffP_   = nullptr;

  /* In & out buffers sizes */
    size_t inBuffSize_  = 0;
    size_t outBuffSize_ = 0;

  /* Jumps & Calls locations stuff */
    /* Jumps and calls locations */
    LocInfo_t JnCLocs_ [0x400] = { };
    /* Jumps and calls arg locations */
    LocInfo_t JnCArgs_ [0x400] = { };

    size_t JnCLocsNum_ = 0;
    size_t JnCArgsNum_ = 0;

/* Methods */
public:

    /* To translate my format executable file to ELF file */
    Error_t translate( char const * const inNameP,
                       char const * const outNameP );

private:

    /* To put headers */
    Error_t putHeaders();

    /* To translate my format bin code to x86-64 bin code */
    Error_t translateCode();

    /* To get info about labels & funcs */
    Error_t firstPass();

    /* To calculate & place all addrs */
    Error_t addrsCalcNPlace();

    static const int BAD_LOC_ = -1;
    /* To seek out loc by in loc in dest locs massive */
    int seekOutLoc (size_t inLoc);

    /* Optimize */
    Error_t optPass();

};

#endif