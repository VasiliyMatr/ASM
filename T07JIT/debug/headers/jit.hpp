
#include "elf-header.hpp"
#include "dsl.hpp"

#ifndef JIT_HPP_INCL_
#define JIT_HPP_INCL_

class JITCompiler
{

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
    _AL_TYPE * inBuffP_ = nullptr;
    _BYTE * outBuffP_   = nullptr;

  /* In & out buffers sizes */
    size_t inBuffSize_  = 0;
    size_t outBuffSize_ = 0;


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

};

#endif