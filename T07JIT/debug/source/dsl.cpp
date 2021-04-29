
#include "dsl.hpp"

/* num of registers */
    static const size_t REG_NUM_ = 8;

/* BINARY OPERATIONS STUFF */

    /* mask to check if second operand in binary operation cmd is number */
        static const _AL_TYPE BIN_OP_SD_OPERAND_T_NUM_MASK_ = 1;
    /* mask to check if second operand in binary operation cmd is register */
        static const _AL_TYPE BIN_OP_SD_OPERAND_T_REG_MASK_ = 1 << 1;

    /* mask to get first register id for binary operation cmd */
        static const _AL_TYPE BIN_OP_FT_REG_OFFSET_ = 16;
    /* mask to get second register id for binary operation cmd */
        static const _AL_TYPE BIN_OP_SD_REG_OFFSET_ = 8;

    size_t putAdd     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* add reg to reg */
        static const unsigned short ADD_RR_START_CODE_ = 0x01C0;
        /* add number to reg */
        static const unsigned short ADD_RN_START_CODE_ = 0x81C0;

        _AL_TYPE modifier = *inBuffP;
        short opCode = 0;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            *(short*) outBuffP = ADD_RR_START_CODE_ +
            REG_NUM_ * (modifier >>  BIN_OP_SD_REG_OFFSET_) & 0x7 +
            (modifier >> BIN_OP_FT_REG_OFFSET_) & 0x7;

            return 2;
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            *(short*) outBuffP = ADD_RN_START_CODE_ + (modifier >> BIN_OP_FT_REG_OFFSET_) & 0x7;
            *(int*) ((short*) outBuffP + 2) = *(inBuffP + 1);

            return 6; 
        }

        else return 0;

    }
    size_t putSub     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = *inBuffP;

        return 0;
    }
    size_t putMul     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = *inBuffP;

        return 0;
    }
    size_t putDiv     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = *inBuffP;

        return 0;
    }

/* ON-STACK BINARY OPERATIONS STUFF */

    size_t putAdds    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putSubs    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putMuls    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putDivs    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }

/* PUSH STUFF */

    size_t putPush    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }

/* POP STUFF */

    size_t putPop     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }

/* ON-STACK CMP STUFF */

    size_t putCmps    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }

/* CALL STUFF */

    size_t putCall    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }

/* EXIT STUFF */

    size_t putExit    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }

/* JUMPS STUFF */

    size_t putJe      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putJne     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putJae     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putJle     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putJa      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putJl      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putJmp     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }

/* MOV STUFF */

    size_t putMov     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }

/* IN/OUT STUFF */

    size_t putIn      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putOut     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }

/* POPA/PUSHA STUFF */

    size_t putPopa    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }
    size_t putPusha   ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }

/* RET STUFF */

    size_t putRet     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return 0;
    }