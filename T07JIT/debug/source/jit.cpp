
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
    size_t inBuffShift = 0;
    size_t outBuffShift = CODE_ENTRY_POINT_OFFSET_;

    for (; inBuffShift < inBuffSize_;)
    {
        retOff_t retOffset = { 0, 0 };

        switch ((CMDId_t)inBuffP_ [inBuffShift++])
        {

            case CMDId_t:: CMD_ADD_     : retOffset = putAdd     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_SUB_     : retOffset = putSub     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_MUL_     : retOffset = putMul     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_DIV_     : retOffset = putDiv     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_ADDS_    : retOffset = putAdds    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_SUBS_    : retOffset = putSubs    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_MULS_    : retOffset = putMuls    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_DIVS_    : retOffset = putDivs    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_PUSH_    : retOffset = putPush    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_POP_     : retOffset = putPop     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_CMPS_    : retOffset = putCmps    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_CALL_    : retOffset = putCall    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_EXIT_    : retOffset = putExit    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_JE_      : retOffset = putJe      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_JNE_     : retOffset = putJne     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_JAE_     : retOffset = putJae     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_JLE_     : retOffset = putJle     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_JA_      : retOffset = putJa      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_JL_      : retOffset = putJl      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_JMP_     : retOffset = putJmp     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_MOV_     : retOffset = putMov     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_IN_      : retOffset = putIn      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_OUT_     : retOffset = putOut     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_POPA_    : retOffset = putPopa    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_PUSHA_   : retOffset = putPusha   ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
            case CMDId_t:: CMD_RET_     : retOffset = putRet     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffShift ); break;
        }

        if (retOffset.outBuffOff_ == 0)
            return Error_t::CMD_ERR_;

        inBuffShift += retOffset.inBuffOff_;
        outBuffShift += retOffset.outBuffOff_;

    }

    outBuffSize_ = outBuffShift - CODE_ENTRY_POINT_OFFSET_;
    programHeader_.setPHSize (outBuffShift - CODE_ENTRY_POINT_OFFSET_);

    return Error_t::OK_;
}