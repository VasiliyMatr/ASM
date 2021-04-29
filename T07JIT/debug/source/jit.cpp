
#include "jit.hpp"

Error_t JITCompiler::translate( char const * const inNameP,
                                char const * const outNameP )
{

    Error_t error = readFile2Buff (inNameP, (_BYTE**) &inBuffP_, &inBuffSize_);
    if (error != Error_t::OK_)
        return error;

    outBuffP_ = (_BYTE*) calloc (sizeof (_BYTE), inBuffSize_ + CODE_ENTRY_POINT_OFFSET_);
    if (outBuffP_ == nullptr)
        return Error_t::MEM_ERR_;

    inBuffSize_ /= sizeof (_AL_TYPE);

    error = translateCode ();
    if (error != Error_t::OK_)
        return error;

    error = putHeaders ();
    if (error != Error_t::OK_)
        return error;

    error = writeBuff2File (outNameP, outBuffP_, outBuffSize_ + CODE_ENTRY_POINT_OFFSET_);
    if (error != Error_t::OK_)
        return error;

    return Error_t::OK_;
}

Error_t JITCompiler::putHeaders()
{

    /* Writing data */
    memcpy ((void*) outBuffP_, (void*) &ELFHeader_, sizeof (ELFHeader_t));
    memcpy ((void*) (outBuffP_ + sizeof (ELFHeader_t)),
    (void*) &programHeader_, sizeof (ProgramHeader_t));

    return Error_t::OK_;
}

Error_t JITCompiler::translateCode()
{
    _BYTE code [] = { 0x6A, 0x41, 0x48, 0x89, 0xE6, 0xBA, 0x01, 0x00, 0x00, 0x00, 0xB8, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x05, 0x48, 0x83, 0xC4, 0x08, 0xB8, 0x3C, 0x00, 0x00, 0x00, 0x48, 0x31, 0xFF, 0x0F, 0x05 };

    outBuffSize_ = sizeof (code);
    programHeader_.setPHSize (outBuffSize_);

    memcpy ((void*) (outBuffP_ + CODE_ENTRY_POINT_OFFSET_), code , sizeof (code));

    return Error_t::OK_;
}