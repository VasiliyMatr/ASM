
#include "dsl.hpp"

/* BINARY OPERATIONS STUFF */

    /* mask to check if second operand in binary operation cmd is number */
        static const _AL_TYPE MASK_BIN_OP_SD_OPERAND_T_NUM_ = 1;
    /* mask to check if second operand in binary operation cmd is register */
        static const _AL_TYPE MASK_BIN_OP_SD_OPERAND_T_REG_ = 1 << 1;

    /* mask to get first register id for binary operation cmd */
        static const _AL_TYPE MASK_BIN_OP_FT_REG_ = 0xF << 16;
    /* mask to get second register id for binary operation cmd */
        static const _AL_TYPE MASK_BIN_OP_FT_REG_ = 0xF << 8;

    size_t putAdd     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = *inBuffP;

        return 0;
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