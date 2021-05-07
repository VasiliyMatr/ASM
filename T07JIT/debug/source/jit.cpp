
#include "jit.hpp"

int JNCLocCmp ( const void * a, const void * b );

Error_t JITCompiler::translate( char const * const inNameP,
                                char const * const outNameP )
{

    /* Reading input file */
    Error_t error = readFile2Buff (inNameP, (BYTE__**) &inBuffP_, &inBuffSize_);
    if (error != Error_t::OK_)
        return error;

    /* Counting num of align-typed data */
    inBuffSize_ /= sizeof (AL_TYPE__);

    /* Allocating out buff */
    outBuffP_ = (BYTE__*) calloc (sizeof (BYTE__), inBuffSize_ * MAX_CMD_SIZE_ + HEADERS_SIZE_ );
    if (outBuffP_ == nullptr)
        return Error_t::MEM_ERR_;

    /* Putting headers & std funcs */
    error = putHeaders ();
    if (error != Error_t::OK_)
        return error;

    /* Getting info about all jumps dests */
    error = firstPass ();
    if (error != Error_t::OK_)
        return error;

    /* sorting jumps & calls dest locations - to recount it in translateCode func */
    qsort (JNCDestLocs_, JNCDestLocsNum_, sizeof (LocInfo_t), JNCLocCmp);

    /* Translating code AND
     * recounting offsets of all jumps & calls dests AND
     * getting info about all jumps & calls offsets  */
    error = translateCode ();
    if (error != Error_t::OK_)
        return error;

    /* Calculating & placing all addrs */
    error = addrsCalcNPlace ();
    if (error != Error_t::OK_)
        return error;

    /* Writing out buff to out file */
    error = writeBuff2File (outNameP, outBuffP_, outBuffSize_);
    if (error != Error_t::OK_)
        return error;

    return Error_t::OK_;
}

int JNCLocCmp ( const void * a, const void * b )
{
    return ((JITCompiler::LocInfo_t*) a)->inLoc_ - ((JITCompiler::LocInfo_t*) b)->inLoc_;
}

Error_t JITCompiler::putHeaders()
{
    BYTE__* writeP = outBuffP_;

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
    size_t JNCId = 0;

    for (; inBuffShift < inBuffSize_;)
    {
        if (JNCDestLocs_ [JNCId].inLoc_ == inBuffShift)
            JNCDestLocs_ [JNCId++].outLoc_ = outBuffSize_;

        retOff_t retOffset = { 0, 0 };

        switch ((CMDId_t) inBuffP_ [inBuffShift++])
        {

            case CMDId_t::CMD_ADD_     : retOffset = putAdd     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_SUB_     : retOffset = putSub     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_MUL_     : retOffset = putMul     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_DIV_     : retOffset = putDiv     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_ADDS_    : retOffset = putAdds    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_SUBS_    : retOffset = putSubs    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_MULS_    : retOffset = putMuls    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_DIVS_    : retOffset = putDivs    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_PUSH_    : retOffset = putPush    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_POP_     : retOffset = putPop     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_CMPS_    : retOffset = putCmps    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_CALL_    : retOffset = putCall    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_EXIT_    : retOffset = putExit    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_JE_      : retOffset = putJe      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_JNE_     : retOffset = putJne     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_JAE_     : retOffset = putJae     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_JLE_     : retOffset = putJle     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_JA_      : retOffset = putJa      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_JL_      : retOffset = putJl      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_JMP_     : retOffset = putJmp     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_MOV_     : retOffset = putMov     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;

            case CMDId_t::CMD_IN_      : retOffset = putIn      ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_OUT_     : retOffset = putOut     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JNCLocs_ [JNCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case CMDId_t::CMD_POPA_    : retOffset = putPopa    ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_PUSHA_   : retOffset = putPusha   ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMDId_t::CMD_RET_     : retOffset = putRet     ( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;

            default:
                break;
        }

        if (retOffset.outBuffOff_ == 0)
            return Error_t::CMD_ERR_;

        inBuffShift += retOffset.inBuffOff_;
        outBuffSize_ += retOffset.outBuffOff_;

    }

    programHeader_.setPHSize (outBuffSize_ - HEADERS_SIZE_);

    return Error_t::OK_;
}

Error_t JITCompiler::firstPass()
{
    if (isBadPtr (inBuffP_))
        return Error_t::PTR_ERR_;

    for (size_t inBuffOff = 0; inBuffOff < inBuffSize_;)
        switch ((CMDId_t) inBuffP_ [inBuffOff])
        {
            case CMDId_t::CMD_ADD_     :
            case CMDId_t::CMD_SUB_     :
            case CMDId_t::CMD_MUL_     :
            case CMDId_t::CMD_DIV_     :

                {
                    AL_TYPE__ modifier = inBuffP_ [++inBuffOff];
                    inBuffOff += 1 + (bool) (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_);
                }
                break;

            case CMDId_t::CMD_ADDS_    :
            case CMDId_t::CMD_SUBS_    :
            case CMDId_t::CMD_MULS_    :
            case CMDId_t::CMD_DIVS_    :

                ++inBuffOff;
                break;

            case CMDId_t::CMD_PUSH_    :

                {
                    AL_TYPE__ modifier = inBuffP_ [++inBuffOff];
                    inBuffOff += 1 + !(bool) modifier;
                }
                break;

            case CMDId_t::CMD_POP_     :

                inBuffOff += 2;
                break;

            case CMDId_t::CMD_EXIT_    :
            case CMDId_t::CMD_CMPS_    :

                inBuffOff += 1;
                break;

            case CMDId_t::CMD_CALL_    :
            case CMDId_t::CMD_JE_      :
            case CMDId_t::CMD_JNE_     :
            case CMDId_t::CMD_JAE_     :
            case CMDId_t::CMD_JLE_     :
            case CMDId_t::CMD_JA_      :
            case CMDId_t::CMD_JL_      :
            case CMDId_t::CMD_JMP_     :

                JNCDestLocs_ [JNCDestLocsNum_++].inLoc_ = inBuffP_ [inBuffOff + 1];

                inBuffOff += 2;
                break;

            case CMDId_t::CMD_MOV_     :

                inBuffOff += 2;
                break;

            case CMDId_t::CMD_IN_      :
            case CMDId_t::CMD_OUT_     :

                inBuffOff += 2;
                break;

            case CMDId_t::CMD_POPA_    :
            case CMDId_t::CMD_PUSHA_   :
            case CMDId_t::CMD_RET_     :

                inBuffOff += 1;
                break;

            default:
                return Error_t::CMD_ERR_;
        }

    return Error_t::OK_;
}

Error_t JITCompiler::addrsCalcNPlace()
{
    size_t JNCDestLocsId = 0;

    for (size_t JNCLocsId = 0; JNCLocsId < JNCLocsNum_; ++JNCLocsId)
    {
        LocInfo_t location = JNCLocs_ [JNCLocsId];

        switch ((CMDId_t) inBuffP_ [location.inLoc_])
        {
            case CMDId_t::CMD_IN_   :
                *(DWRD__*) (outBuffP_ + location.outLoc_ + 1) =
                            HEADERS_SIZE_ + IN_PROC_ADDR_ - location.outLoc_ - 5;
                break;
            case CMDId_t::CMD_OUT_  :
                *(DWRD__*) (outBuffP_ + location.outLoc_ + 4) =
                            HEADERS_SIZE_ + OUT_PROC_ADDR_ - location.outLoc_ - 8;
                break;
            case CMDId_t::CMD_EXIT_ :
                *(DWRD__*) (outBuffP_ + location.outLoc_ + 1) =
                            HEADERS_SIZE_ + EXIT_PROC_ADDR_ - location.outLoc_ - 5;
                break;

            case CMDId_t::CMD_CALL_ :
            case CMDId_t::CMD_JMP_  :
                *(DWRD__*) (outBuffP_ + location.outLoc_ + 1) =
                            seekOutLoc (inBuffP_ [location.inLoc_ + 1]) - location.outLoc_ - 5;
                break;

            case CMDId_t::CMD_JE_   :
            case CMDId_t::CMD_JNE_  :
            case CMDId_t::CMD_JAE_  :
            case CMDId_t::CMD_JLE_  :
            case CMDId_t::CMD_JA_   :
            case CMDId_t::CMD_JL_   :
                *(DWRD__*) (outBuffP_ + location.outLoc_ + 2) =
                            seekOutLoc (inBuffP_ [location.inLoc_ + 1]) - location.outLoc_ - 6;
                break;

            default:
                return Error_t::CMD_ERR_;
        }
    }

    return Error_t::OK_;
}

int JITCompiler::seekOutLoc (size_t inLoc)
{
    for (size_t locId = 0; locId < JNCDestLocsNum_; ++locId)
    {
        if (inLoc == JNCDestLocs_ [locId].inLoc_)
            return JNCDestLocs_ [locId].outLoc_;
    }

    return BAD_LOC_;
}