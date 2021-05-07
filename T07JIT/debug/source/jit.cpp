
#include "jit.hpp"

/* compare func for Jumps & Calls arguments sort */
int JnCArgsCmp ( const void * a, const void * b );

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

    /* sorting jumps & calls args locations - to recount it in translateCode func */
    qsort (JnCArgs_, JnCArgsNum_, sizeof (LocInfo_t), JnCArgsCmp);

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

int JnCArgsCmp ( const void * a, const void * b )
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
    size_t JnCId = 0;

    for (; inBuffShift < inBuffSize_;)
    {
        /* Calculating out format callable and jumpable locations */
        if (JnCArgs_ [JnCId].inLoc_ == inBuffShift)
            JnCArgs_ [JnCId++].outLoc_ = outBuffSize_;

        cmdSizes_t cmdSizes = { 0, 0 };

        /* translating code & remembering all jumps & calls locations */
        switch ((CMDId_t) inBuffP_ [inBuffShift++])
        {
            case ADD__  .id_: cmdSizes = ADD__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case SUB__  .id_: cmdSizes = SUB__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case MUL__  .id_: cmdSizes = MUL__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case DIV__  .id_: cmdSizes = DIV__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case ADDS__ .id_: cmdSizes = ADDS__ .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case SUBS__ .id_: cmdSizes = SUBS__ .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case MULS__ .id_: cmdSizes = MULS__ .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case DIVS__ .id_: cmdSizes = DIVS__ .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case PUSH__ .id_: cmdSizes = PUSH__ .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case POP__  .id_: cmdSizes = POP__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case CMPS__ .id_: cmdSizes = CMPS__ .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;

            case CALL__ .id_: cmdSizes = CALL__ .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case EXIT__ .id_: cmdSizes = EXIT__ .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case JEQ__  .id_: cmdSizes = JEQ__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case JNE__  .id_: cmdSizes = JNE__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case JAE__  .id_: cmdSizes = JAE__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case JLE__  .id_: cmdSizes = JLE__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case JAA__  .id_: cmdSizes = JAA__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case JLL__  .id_: cmdSizes = JLL__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case JMP__  .id_: cmdSizes = JMP__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case MOV__  .id_: cmdSizes = MOV__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;

            case IN__   .id_: cmdSizes = IN__   .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case OUT__  .id_: cmdSizes = OUT__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ );
                                         JnCLocs_ [JnCLocsNum_++] = { inBuffShift - 1, outBuffSize_ };
                                         break;

            case POPA__ .id_: cmdSizes = POPA__ .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case PUSHA__.id_: cmdSizes = PUSHA__.putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;
            case RET__  .id_: cmdSizes = RET__  .putF_( inBuffP_ + inBuffShift, outBuffP_ + outBuffSize_ ); break;

            default:
                break;
        }

        if (cmdSizes.outBuffSize_ == 0)
            return Error_t::CMD_ERR_;

        /* adding translated cmd sizes */
        inBuffShift  += cmdSizes.inBuffSize_;
        outBuffSize_ += cmdSizes.outBuffSize_;
    }

    return Error_t::OK_;
}

Error_t JITCompiler::firstPass()
{
    if (isBadPtr (inBuffP_))
        return Error_t::PTR_ERR_;

    /* getting all callable and jumpable locations (in input format for now) */
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

                JnCArgs_ [JnCArgsNum_++].inLoc_ = inBuffP_ [inBuffOff + 1];

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
    size_t JnCDestLocsId = 0;

    /* Calculating calls & jumps arguments */
    for (size_t JnCLocsId = 0; JnCLocsId < JnCLocsNum_; ++JnCLocsId)
    {
        LocInfo_t location = JnCLocs_ [JnCLocsId];

        switch ((CMDId_t) inBuffP_ [location.inLoc_])
        {
            case CMDId_t::CMD_IN_   : /* check in cmd put func for 5 value explanation */
                *(DWRD__*) (outBuffP_ + location.outLoc_ + 1) =
                            HEADERS_SIZE_ + IN_PROC_ADDR_ - location.outLoc_ - 5;
                break;
            case CMDId_t::CMD_OUT_  :/* check in cmd put func for 8 value explanation */
                *(DWRD__*) (outBuffP_ + location.outLoc_ + 4) =
                            HEADERS_SIZE_ + OUT_PROC_ADDR_ - location.outLoc_ - 8;
                break;
            case CMDId_t::CMD_EXIT_ :/* check in cmd put func for 5 value explanation */
                *(DWRD__*) (outBuffP_ + location.outLoc_ + 1) =
                            HEADERS_SIZE_ + EXIT_PROC_ADDR_ - location.outLoc_ - 5;
                break;

            case CMDId_t::CMD_CALL_ :
            case CMDId_t::CMD_JMP_  :
                *(DWRD__*) (outBuffP_ + location.outLoc_ + sizeof (CALL_CODE_)) =
                            seekOutLoc (inBuffP_ [location.inLoc_ + 1]) - location.outLoc_ -
                            (sizeof (CALL_CODE_) + sizeof (DWRD__));
                break;

            case CMDId_t::CMD_JE_   :
            case CMDId_t::CMD_JNE_  :
            case CMDId_t::CMD_JAE_  :
            case CMDId_t::CMD_JLE_  :
            case CMDId_t::CMD_JA_   :
            case CMDId_t::CMD_JL_   :
                *(DWRD__*) (outBuffP_ + location.outLoc_ + sizeof (JEQ_CODE_)) =
                            seekOutLoc (inBuffP_ [location.inLoc_ + 1]) - location.outLoc_ -
                            (sizeof (JEQ_CODE_) + sizeof (DWRD__));
                break;

            default:
                return Error_t::CMD_ERR_;
        }
    }

    return Error_t::OK_;
}

int JITCompiler::seekOutLoc (size_t inLoc)
{
    /* TODO: bin search */
    for (size_t locId = 0; locId < JnCArgsNum_; ++locId)
    {
        if (inLoc == JnCArgs_ [locId].inLoc_)
            return JnCArgs_ [locId].outLoc_;
    }

    return BAD_LOC_;
}