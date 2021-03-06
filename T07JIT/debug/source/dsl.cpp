
#include "dsl.hpp"

    cmdSizes_t putAdd     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        AL_TYPE__ modifier = inBuffP [0];
        size_t ftRegId = ((modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;
        size_t sdRegId = ((modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            *(DWRD__*) outBuffP = ADD_RR_START_CODE_ + REG_NUM_ * sdRegId + ftRegId;

            return { 3, 1 };
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            *(DWRD__*) outBuffP = ADD_RN_START_CODE_ + ftRegId;
            *(DWRD__*) (outBuffP + 3) = inBuffP [1];

            return { 7, 2 }; 
        }

        return { 0, 0 };

    }
    cmdSizes_t putSub     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        AL_TYPE__ modifier = inBuffP[0];
        size_t ftRegId = ((modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;
        size_t sdRegId = ((modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            *(DWRD__*) outBuffP = SUB_RR_START_CODE_ + REG_NUM_ * sdRegId + ftRegId;

            return { 3, 1 };
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            *(DWRD__*) (outBuffP + 0) = SUB_RN_START_CODE_ + ftRegId;
            *(DWRD__*) (outBuffP + 3) = inBuffP[1];

            return { 7, 2 };
        }

        return { 0, 0 };

    }
    cmdSizes_t putMul     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        AL_TYPE__ modifier = inBuffP [0];
        size_t ftRegId = ((modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;
        size_t sdRegId = ((modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            /* mov ft reg to eax, mov sd reg to ebx, mul, mov result to dest */
            *(WORD__*) (outBuffP) = XOR_EDX_EDX_CODE_;
            *(DWRD__*) (outBuffP + 2) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;
            *(DWRD__*) (outBuffP + 5) = MOV_R_EBX_START_CODE_ + REG_NUM_ * sdRegId;

            *(WORD__*) (outBuffP + 8) = 0xF7E3;

            *(DWRD__*) (outBuffP + 10) = MOV_EAX_R_START_CODE_ + (ftRegId << 16);
            
            return { 13, 1 };
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            /* mov number to eax, mov reg to ebx, mul, mov result to dest */
            outBuffP [0] = MOV_N_EBX_CODE_;
            *(DWRD__*) (outBuffP +  1) = inBuffP [1];
            *(DWRD__*) (outBuffP +  5) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;

            *(WORD__*) (outBuffP +  8) = 0xF7E3;
            
            *(DWRD__*) (outBuffP + 10) = MOV_EAX_R_START_CODE_ + (ftRegId << 16);

            return { 13, 2 };
        }

        return { 0, 0 };
    }
    cmdSizes_t putDiv     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        AL_TYPE__ modifier = inBuffP [0];
        DWRD__ ftRegId = ((modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;
        DWRD__ sdRegId = ((modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_) << 16;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            /* mov ft reg to eax, mov sd reg to ebx, div, mov result to dest */
            *(DWRD__*) (outBuffP +  0) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;
            *(DWRD__*) (outBuffP +  3) = MOV_R_EBX_START_CODE_ + REG_NUM_ * sdRegId;

            *(WORD__*) (outBuffP +  6) = XOR_EDX_EDX_CODE_;
            /* div ebx */
            *(WORD__*) (outBuffP +  8) = 0xF7F3;

            *(DWRD__*) (outBuffP + 10) = MOV_EAX_R_START_CODE_ + (ftRegId << 16);

            return { 13, 1 };
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            /* mov number to eax, mov reg to ebx, div, mov result to dest */
            outBuffP [0] = MOV_N_EBX_CODE_;
            *(DWRD__*) (outBuffP +  1) = inBuffP [1];
            *(DWRD__*) (outBuffP +  5) = MOV_R_EAX_START_CODE_ + ((REG_NUM_ * ftRegId) << 16);

            *(WORD__*) (outBuffP +  8) = XOR_EDX_EDX_CODE_;
            /* div ebx */
            *(WORD__*) (outBuffP + 10) = 0xF7F3;
            
            *(DWRD__*) (outBuffP + 12) = MOV_EAX_R_START_CODE_ + (ftRegId << 16);

            return { 15, 2 };
        }

        return { 0, 0 };
    }

/* ON-STACK BINARY OPERATIONS STUFF */

    cmdSizes_t putAdds    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        /* pop to eax & ebx, add, push */
        outBuffP [0] = POP_EAX_CODE_ + 3;
        outBuffP [1] = POP_EAX_CODE_;

        /* add eax, ebx */
        *(WORD__*) (outBuffP +  2) = 0xD801;

        outBuffP [4] = PUSH_EAX_CODE_;

        return { 5, 0 };
    }
    cmdSizes_t putSubs    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        /* pop to eax & ebx, sub, push */
        outBuffP [0] = POP_EAX_CODE_ + 3;
        outBuffP [1] = POP_EAX_CODE_;

        /* sub eax, ebx */
        *(WORD__*) (outBuffP +  2) = 0xC32B;

        outBuffP [4] = PUSH_EAX_CODE_;

        return { 5, 0 };
    }
    cmdSizes_t putMuls    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        /* pop to eax & ebx, mul, push */
        outBuffP [0] = POP_EAX_CODE_ + 3;
        outBuffP [1] = POP_EAX_CODE_;

        *(WORD__*) (outBuffP + 2) = XOR_EDX_EDX_CODE_;

        /* mul ebx */
        *(WORD__*) (outBuffP + 4) = 0xE3F7;
        
        outBuffP [6] = PUSH_EAX_CODE_;

        return { 7, 0 };
    }
    cmdSizes_t putDivs    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        /* pop to eax & ebx, mul, push */
        outBuffP [0] = POP_EAX_CODE_ + 3;
        outBuffP [1] = POP_EAX_CODE_;

        /* div ebx */
        *(WORD__*) (outBuffP +  2) = XOR_EDX_EDX_CODE_;
        *(WORD__*) (outBuffP +  4) = 0xF3F7;
        
        outBuffP [6] = PUSH_EAX_CODE_;

        return { 7, 0 };
    }

/* PUSH STUFF */

    cmdSizes_t putPush    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        /* modifier info */
        AL_TYPE__ modifier = inBuffP [0];

        /* push number */
        if (modifier == 0)
        {
            /* num push code */
            outBuffP [0] = PUSH_NUM_CODE_;

            *(DWRD__*) (outBuffP + 1) = inBuffP [1];

            return { 5 , 2 };
        }

        /* push reg */
        size_t regId = modifier >> 16;
        if (regId >= REG_NUM_)
            return { 0, 0 };

        *(WORD__*) (outBuffP + 0) = PUSH_R_START_CODE_ + (regId << 8);

        return { 2, 1 };
    }

/* POP STUFF */

    cmdSizes_t putPop     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        /* cmd midifier */
        AL_TYPE__ modifier = inBuffP [0];
        size_t regId = modifier;

        /* pop reg */
        if (regId < REG_NUM_)
        {
            /* reg pop code */
            *(WORD__*) outBuffP = POP_START_CODE_ + (regId << 8);

            return { 2, 1 };
        }

        /* empty pop */
        outBuffP [0] = POP_EAX_CODE_;

        return { 1, 1 };
    }

/* ON-STACK CMP STUFF */

    cmdSizes_t putCmps    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        /* pop to eax & ebx, cmp */
        outBuffP [0] = POP_EAX_CODE_;
        outBuffP [1] = POP_EAX_CODE_ + 3;

        *(WORD__*) (outBuffP + 2) = CMP_EAX_EBX_CODE_;

        return { 4, 0 };
    }

/* CALL STUFF */

    cmdSizes_t putCall    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        outBuffP [0] = CALL_CODE_;

        *(DWRD__*) (outBuffP + 1) = inBuffP [0];

        return { 5, 1 };
    }

/* EXIT STUFF */

    cmdSizes_t putExit    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        outBuffP [0] = CALL_CODE_;

        *(DWRD__*) (outBuffP + 1) = 0;

        return { 5, 0 };
    }

/* JUMPS STUFF */

    cmdSizes_t putJe      ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        *(WORD__*) outBuffP = JEQ_CODE_;
        *(DWRD__*) (outBuffP + 2) = inBuffP [0];

        return { 6, 1 };
    }
    cmdSizes_t putJne     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        *(WORD__*) outBuffP = JNE_CODE_;
        *(DWRD__*) (outBuffP + 2) = inBuffP [0];

        return { 6, 1 };
    }
    cmdSizes_t putJae     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        *(WORD__*) outBuffP = JAE_CODE_;
        *(DWRD__*) (outBuffP + 2) = inBuffP [0];

        return { 6, 1 };
    }
    cmdSizes_t putJle     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        *(WORD__*) outBuffP = JLE_CODE_;
        *(DWRD__*) (outBuffP + 2) = inBuffP [0];

        return { 6, 1 };
    }
    cmdSizes_t putJa      ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        *(WORD__*) outBuffP = JAA_CODE_;
        *(DWRD__*) (outBuffP + 2) = inBuffP [0];

        return { 6, 1 };
    }
    cmdSizes_t putJl      ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        *(WORD__*) outBuffP = JLL_CODE_;
        *(DWRD__*) (outBuffP + 2) = inBuffP [0];

        return { 6, 1 };
    }
    cmdSizes_t putJmp     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        outBuffP [0] = JMP_CODE_;
        *(DWRD__*) (outBuffP + 1) = inBuffP [0];

        return { 5, 1 };
    }

/* MOV STUFF */

    cmdSizes_t putMov     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        AL_TYPE__ modifier = inBuffP [0];

        AL_TYPE__ ftRegId = (modifier & FT_REG_ID_) >> 4;
        AL_TYPE__ sdRegId = (modifier & SD_REG_ID_);

        /* nulling regs ids */
        modifier = (modifier >> 8) << 8;

        switch (modifier)
        {
            case FT_REG_ON_ | SD_REG_ON_:
                *(DWRD__*) outBuffP = MOV_RR_CODE_ + ((ftRegId + sdRegId * REG_NUM_) << 16);
                return { 3, 1 };

            /* TODO: */
            default:
                return { 0, 0 };
        }

        return { 0, 0 };
    }

/* IN/OUT STUFF */

    cmdSizes_t putIn      ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        AL_TYPE__ regId = inBuffP [0];

        outBuffP [0] = CALL_CODE_;

        *(DWRD__*) (outBuffP + 1) = 0;

        *(DWRD__*) (outBuffP + 5) = MOV_EAX_R_START_CODE_ + (regId << 16);

        return { 8, 1 };
    }
    cmdSizes_t putOut     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        AL_TYPE__ regId = inBuffP [0];

        *(DWRD__*) outBuffP = MOV_R_EAX_START_CODE_ + (regId << 16);

        outBuffP [3] = CALL_CODE_;

        *(DWRD__*) (outBuffP + 4) = 0;

        return { 8, 1 };
    }

/* POPA/PUSHA STUFF */

    cmdSizes_t putPopa    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        *(WORD__*) (outBuffP +  0) = POP_START_CODE_ + (6 << 8);
        *(WORD__*) (outBuffP +  2) = POP_START_CODE_ + (5 << 8);
        *(WORD__*) (outBuffP +  4) = POP_START_CODE_ + (4 << 8);
        *(WORD__*) (outBuffP +  6) = POP_START_CODE_ + (3 << 8);
        *(WORD__*) (outBuffP +  8) = POP_START_CODE_ + (2 << 8);
        *(WORD__*) (outBuffP + 10) = POP_START_CODE_ + (1 << 8);
        *(WORD__*) (outBuffP + 12) = POP_START_CODE_;

        return { 14, 0 };
    }
    cmdSizes_t putPusha   ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        *(WORD__*) (outBuffP +  0) = PUSH_R_START_CODE_;
        *(WORD__*) (outBuffP +  2) = PUSH_R_START_CODE_ + (1 << 8);
        *(WORD__*) (outBuffP +  4) = PUSH_R_START_CODE_ + (2 << 8);
        *(WORD__*) (outBuffP +  6) = PUSH_R_START_CODE_ + (3 << 8);
        *(WORD__*) (outBuffP +  8) = PUSH_R_START_CODE_ + (4 << 8);
        *(WORD__*) (outBuffP + 10) = PUSH_R_START_CODE_ + (5 << 8);
        *(WORD__*) (outBuffP + 12) = PUSH_R_START_CODE_ + (6 << 8);

        return { 14, 0 };
    }

/* RET STUFF */

    cmdSizes_t putRet     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP )
    {
        outBuffP [0] = RET_CODE_;

        return { 1, 0 };
    }