
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
            outBuffP [0] = MOV_N_EBX_CODE_;
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

            *(_WORD*) (outBuffP +  6) = XOR_EDX_EDX_CODE_;
            /* div ebx */
            *(_WORD*) (outBuffP +  8) = 0xF7F3;

            *(_DWRD*) (outBuffP + 10) = MOV_EAX_R_START_CODE_ + ftRegId;

            return { 13, 1 };
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            /* mov number to eax, mov reg to ebx, div, mov result to dest */
            outBuffP [0] = MOV_N_EBX_CODE_;
            *(_DWRD*) (outBuffP +  1) = inBuffP [1];
            *(_DWRD*) (outBuffP +  5) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;

            *(_WORD*) (outBuffP +  8) = XOR_EDX_EDX_CODE_;
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
        outBuffP [0] = POP_EAX_CODE_;
        outBuffP [1] = POP_EAX_CODE_ + 3;

        /* add eax, ebx */
        *(_WORD*) (outBuffP +  2) = 0xD801;

        outBuffP [4] = PUSH_EAX_CODE_;

        return { 5, 0 };
    }
    retOff_t putSubs    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, sub, push */
        outBuffP [0] = POP_EAX_CODE_;
        outBuffP [1] = POP_EAX_CODE_ + 3;

        /* sub eax, ebx */
        *(_WORD*) (outBuffP +  2) = 0xC32B;

        outBuffP [4] = PUSH_EAX_CODE_;

        return { 5, 0 };
    }
    retOff_t putMuls    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, mul, push */
        outBuffP [0] = POP_EAX_CODE_;
        outBuffP [1] = POP_EAX_CODE_ + 3;

        /* mul ebx */
        *(_WORD*) (outBuffP +  2) = 0xE3F7;
        
        outBuffP [4] = PUSH_EAX_CODE_;

        return { 5, 0 };
    }
    retOff_t putDivs    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, mul, push */
        outBuffP [0] = POP_EAX_CODE_;
        outBuffP [1] = POP_EAX_CODE_ + 3;

        /* div ebx */
        *(_WORD*) (outBuffP +  2) = XOR_EDX_EDX_CODE_;
        *(_WORD*) (outBuffP +  4) = 0xF3F7;
        
        outBuffP [6] = PUSH_EAX_CODE_;

        return { 7, 0 };
    }

/* PUSH STUFF */

    retOff_t putPush    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* modifier info */
        _AL_TYPE modifier = inBuffP [0];

        /* push number */
        if (modifier == 0)
        {
            /* num push code */
            outBuffP [0] = PUSH_NUM_CODE_;

            *(_DWRD*) (outBuffP + 1) = inBuffP [1];

            return { 5 , 2 };
        }

        /* push reg */
        size_t regId = modifier >> 16;
        if (modifier >= REG_NUM_)
            return { 0, 0 };

        *(_WORD*) (outBuffP + 0) = PUSH_R_START_CODE_ + regId;

        return { 2, 1 };
    }

/* POP STUFF */

    retOff_t putPop     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* cmd midifier */
        _AL_TYPE modifier = inBuffP [0];
        size_t regId = modifier;

        /* pop reg */
        if (regId < REG_NUM_)
        {
            /* reg pop code */
            *(_WORD*) outBuffP = POP_START_CODE_ + regId;

            return { 2, 1 };
        }

        /* empty pop */
        outBuffP [0] = POP_EAX_CODE_;

        return { 1, 1 };
    }

/* ON-STACK CMP STUFF */

    retOff_t putCmps    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, cmp */
        outBuffP [0] = POP_EAX_CODE_;
        outBuffP [1] = POP_EAX_CODE_ + 3;

        *(_WORD*) (outBuffP + 2) = CMP_EAX_EBX_CODE_;

        return { 4, 0 };
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

        *(_WORD*) (outBuffP +  0) = POP_START_CODE_ + 6;
        *(_WORD*) (outBuffP +  2) = POP_START_CODE_ + 5;
        *(_WORD*) (outBuffP +  4) = POP_START_CODE_ + 4;
        *(_WORD*) (outBuffP +  6) = POP_START_CODE_ + 3;
        *(_WORD*) (outBuffP +  8) = POP_START_CODE_ + 2;
        *(_WORD*) (outBuffP + 10) = POP_START_CODE_ + 1;
        *(_WORD*) (outBuffP + 12) = POP_START_CODE_;

        return { 14, 0 };
    }
    retOff_t putPusha   ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        *(_WORD*) (outBuffP +  0) = PUSH_R_START_CODE_;
        *(_WORD*) (outBuffP +  2) = PUSH_R_START_CODE_ + 1;
        *(_WORD*) (outBuffP +  4) = PUSH_R_START_CODE_ + 2;
        *(_WORD*) (outBuffP +  6) = PUSH_R_START_CODE_ + 3;
        *(_WORD*) (outBuffP +  8) = PUSH_R_START_CODE_ + 4;
        *(_WORD*) (outBuffP + 10) = PUSH_R_START_CODE_ + 5;
        *(_WORD*) (outBuffP + 12) = PUSH_R_START_CODE_ + 6;

        return { 14, 0 };
    }

/* RET STUFF */

    retOff_t putRet     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {

        outBuffP [0] = RET_CODE_;

        return { 1, 0 };
    }