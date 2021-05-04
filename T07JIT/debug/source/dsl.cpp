
#include "dsl.hpp"

    retOff_t putAdd     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = inBuffP [0];
        size_t ftRegId = ((modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;
        size_t sdRegId = ((modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            *(_DWRD*) outBuffP = ADD_RR_START_CODE_ + REG_NUM_ * sdRegId + ftRegId;

            return { 3, 1 };
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            *(_DWRD*) outBuffP = ADD_RN_START_CODE_ + ftRegId;
            *(_DWRD*) (outBuffP + 3) = inBuffP [1];

            return { 7, 2 }; 
        }

        return { 0, 0 };

    }
    retOff_t putSub     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = inBuffP[0];
        size_t ftRegId = ((modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;
        size_t sdRegId = ((modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            *(_DWRD*) outBuffP = SUB_RR_START_CODE_ + REG_NUM_ * sdRegId + ftRegId;

            return { 3, 1 };
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            *(_DWRD*) (outBuffP + 0) = SUB_RN_START_CODE_ + ftRegId;
            *(_DWRD*) (outBuffP + 3) = inBuffP[1];

            return { 7, 2 };
        }

        return { 0, 0 };

    }
    retOff_t putMul     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = inBuffP [0];
        size_t ftRegId = ((modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;
        size_t sdRegId = ((modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            /* mov ft reg to eax, mov sd reg to ebx, mul, mov result to dest */
            *(_DWRD*) (outBuffP + 0) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;
            *(_DWRD*) (outBuffP + 3) = MOV_R_EBX_START_CODE_ + REG_NUM_ * sdRegId;

            *(_WORD*) (outBuffP + 6) = 0xF7E3;

            *(_DWRD*) (outBuffP + 8) = MOV_EAX_R_START_CODE_ + ftRegId;
            
            return { 11, 1 };
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            /* mov number to eax, mov reg to ebx, mul, mov result to dest */
            *(_BYTE*) (outBuffP +  0) = MOV_N_EBX_START_CODE_;
            *(_DWRD*) (outBuffP +  1) = inBuffP [1];
            *(_DWRD*) (outBuffP +  5) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;

            *(_WORD*) (outBuffP +  8) = 0xF7E3;
            
            *(_DWRD*) (outBuffP + 10) = MOV_EAX_R_START_CODE_ + ftRegId * REG_NUM_;

            return { 13, 2 };
        }

        return { 0, 0 };
    }
    retOff_t putDiv     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = inBuffP [0];
        _DWRD ftRegId = ((modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;
        _DWRD sdRegId = ((modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            /* mov ft reg to eax, mov sd reg to ebx, div, mov result to dest */
            *(_DWRD*) (outBuffP +  0) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;
            *(_DWRD*) (outBuffP +  3) = MOV_R_EBX_START_CODE_ + REG_NUM_ * sdRegId;

            *(_WORD*) (outBuffP +  6) = XOR_EDX_EDX_;
            /* div ebx */
            *(_WORD*) (outBuffP +  8) = 0xF7F3;

            *(_DWRD*) (outBuffP + 10) = MOV_EAX_R_START_CODE_ + ftRegId;

            return { 13, 1 };
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            /* mov number to eax, mov reg to ebx, div, mov result to dest */
            *(_BYTE*) (outBuffP +  0) = MOV_N_EBX_START_CODE_;
            *(_DWRD*) (outBuffP +  1) = inBuffP [1];
            *(_DWRD*) (outBuffP +  5) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;

            *(_WORD*) (outBuffP +  8) = XOR_EDX_EDX_;
            /* div ebx */
            *(_WORD*) (outBuffP + 10) = 0xF7F3;
            
            *(_DWRD*) (outBuffP + 12) = MOV_EAX_R_START_CODE_ + ftRegId * REG_NUM_;

            return { 15, 2 };
        }

        return { 0, 0 };
    }

/* ON-STACK BINARY OPERATIONS STUFF */

    retOff_t putAdds    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, add, push */
        *(_BYTE*) (outBuffP +  0) = POP_START_CODE_;
        *(_BYTE*) (outBuffP +  1) = POP_START_CODE_ + 3;

        /* add eax, ebx */
        *(_WORD*) (outBuffP +  2) = 0xD801;

        *(_BYTE*) (outBuffP +  4) = PUSH_START_CODE_;

        return { 5, 0 };
    }
    retOff_t putSubs    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, sub, push */
        *(_BYTE*) (outBuffP +  0) = POP_START_CODE_;
        *(_BYTE*) (outBuffP +  1) = POP_START_CODE_ + 3;

        /* sub eax, ebx */
        *(_WORD*) (outBuffP +  2) = 0xC32B;

        *(_BYTE*) (outBuffP +  4) = PUSH_START_CODE_;

        return { 5, 0 };
    }
    retOff_t putMuls    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, mul, push */
        *(_BYTE*) (outBuffP +  0) = POP_START_CODE_;
        *(_BYTE*) (outBuffP +  1) = POP_START_CODE_ + 3;

        /* mul ebx */
        *(_WORD*) (outBuffP +  2) = 0xE3F7;
        
        *(_BYTE*) (outBuffP +  4) = PUSH_START_CODE_;

        return { 5, 0 };
    }
    retOff_t putDivs    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, mul, push */
        *(_BYTE*) (outBuffP +  0) = POP_START_CODE_;
        *(_BYTE*) (outBuffP +  1) = POP_START_CODE_ + 3;

        /* div ebx */
        *(_WORD*) (outBuffP +  2) = XOR_EDX_EDX_;
        *(_WORD*) (outBuffP +  4) = 0xF3F7;
        
        *(_BYTE*) (outBuffP +  6) = PUSH_START_CODE_;

        return { 7, 0 };
    }

/* PUSH STUFF */

    retOff_t putPush    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* arg info */
        _AL_TYPE argInfo = inBuffP [0];

        /* push number */
        if (argInfo == 0)
        {
            /* num push code */
            *(_BYTE*) (outBuffP + 0) = PUSH_NUM_CODE_;

            *(_DWRD*) (outBuffP + 1) = inBuffP [1];

            return { 5 , 2 };
        }

        size_t regId = argInfo >> 16;

        *(_WORD*) (outBuffP + 0) = PUSH_R_START_CODE_ + regId;

        return { 2, 1 };
    }

/* POP STUFF */

    retOff_t putPop     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }

/* ON-STACK CMP STUFF */

    retOff_t putCmps    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }

/* CALL STUFF */

    retOff_t putCall    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }

/* EXIT STUFF */

    retOff_t putExit    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }

/* JUMPS STUFF */

    retOff_t putJe      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }
    retOff_t putJne     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }
    retOff_t putJae     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }
    retOff_t putJle     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }
    retOff_t putJa      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }
    retOff_t putJl      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }
    retOff_t putJmp     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }

/* MOV STUFF */

    retOff_t putMov     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }

/* IN/OUT STUFF */

    retOff_t putIn      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }
    retOff_t putOut     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }

/* POPA/PUSHA STUFF */

    retOff_t putPopa    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }
    retOff_t putPusha   ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }

/* RET STUFF */

    retOff_t putRet     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        return { 0, 0 };
    }