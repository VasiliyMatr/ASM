
#include "jit.hpp"

Error_t JITCompiler::translate( char const * const inNameP,
                                char const * const outNameP )
{

    Error_t error = readFile2Buff (inNameP, (_BYTE**) &inBuffP_, &inBuffSize_);
    if (error != Error_t::OK_)
        return error;

    inBuffSize_ /= sizeof (_AL_TYPE);

    outBuffP_ = (_BYTE*) calloc (sizeof (_BYTE), inBuffSize_ * MAX_CMD_SIZE_ + HEADERS_SIZE_ );
    if (outBuffP_ == nullptr)
        return Error_t::MEM_ERR_;

    error = putHeaders ();
    if (error != Error_t::OK_)
        return error;

    // error = translateCode ();
    // if (error != Error_t::OK_)
    //     return error;

    outBuffP_ [outBuffSize_] = 0x9A;
    *(_DWRD*) (outBuffP_ + outBuffSize_ + 1) = 0x00;
    *(_WORD*) (outBuffP_ + outBuffSize_ + 5) = 0x80;

    outBuffSize_ += 7;

    error = writeBuff2File (outNameP, outBuffP_, outBuffSize_);
    if (error != Error_t::OK_)
        return error;

    return Error_t::OK_;
}

Error_t JITCompiler::putHeaders()
{
    void* writeP = (void*) outBuffP_;

  /* Writing data */
    /* Headers */
    memcpy (writeP, (void*) &ELFHeader_, sizeof (ELFHeader_t));
    outBuffSize_ += sizeof (ELFHeader_t);
    memcpy (writeP + outBuffSize_, (void*) &programHeader_, sizeof (ProgramHeader_t));
    outBuffSize_ += sizeof (ProgramHeader_t);

    /* Std procs */
    memcpy (writeP + outBuffSize_, (void*) IN_PROC_CODE_, sizeof (IN_PROC_CODE_));
    outBuffSize_ += sizeof (IN_PROC_CODE_);
    memcpy (writeP + outBuffSize_, (void*) OUT_PROC_CODE_, sizeof (OUT_PROC_CODE_));
    outBuffSize_ += sizeof (OUT_PROC_CODE_);
    memcpy (writeP + outBuffSize_, (void*) EXIT_PROC_CODE_, sizeof (EXIT_PROC_CODE_));
    outBuffSize_ += sizeof (EXIT_PROC_CODE_);

    return Error_t::OK_;
}

Error_t JITCompiler::translateCode()
{
    size_t inBuffShift = 0;

    for (; inBuffShift < inBuffSize_;)
    {
        retOff_t retOffset = { 0, 0 };

        switch ((CMDId_t)inBuffP_ [inBuffShift++])
        {

            case CMDId_t:: CMD_ADD_     : retOffset = putAdd     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_SUB_     : retOffset = putSub     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_MUL_     : retOffset = putMul     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_DIV_     : retOffset = putDiv     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_ADDS_    : retOffset = putAdds    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_SUBS_    : retOffset = putSubs    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_MULS_    : retOffset = putMuls    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_DIVS_    : retOffset = putDivs    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_PUSH_    : retOffset = putPush    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_POP_     : retOffset = putPop     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_CMPS_    : retOffset = putCmps    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_CALL_    : retOffset = putCall    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_EXIT_    : retOffset = putExit    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_JE_      : retOffset = putJe      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_JNE_     : retOffset = putJne     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_JAE_     : retOffset = putJae     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_JLE_     : retOffset = putJle     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_JA_      : retOffset = putJa      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_JL_      : retOffset = putJl      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_JMP_     : retOffset = putJmp     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_MOV_     : retOffset = putMov     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_IN_      : retOffset = putIn      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_OUT_     : retOffset = putOut     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_POPA_    : retOffset = putPopa    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_PUSHA_   : retOffset = putPusha   ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t:: CMD_RET_     : retOffset = putRet     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
        }

        if (retOffset.outBuffOff_ == 0)
            return Error_t::CMD_ERR_;

        inBuffShift += retOffset.inBuffOff_;
        outBuffSize_ += retOffset.outBuffOff_;

    }

    programHeader_.setPHSize (outBuffSize_ - HEADERS_SIZE_);

    return Error_t::OK_;
}