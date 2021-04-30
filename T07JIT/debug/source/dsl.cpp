
#include "dsl.hpp"

    size_t putAdd     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = inBuffP [0];
        _BYTE ftRegId = (modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_;
        _BYTE sdRegId = (modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            *(int*) outBuffP = ADD_RR_START_CODE_ + REG_NUM_ * sdRegId + ftRegId;

            return 3;
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            *(int*) outBuffP = ADD_RN_START_CODE_ + ftRegId;
            *(int*) (outBuffP + 3) = inBuffP [1];

            return 7; 
        }

        return 0;

    }
    size_t putSub     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = inBuffP[0];
        _BYTE ftRegId = (modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_;
        _BYTE sdRegId = (modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            *(int*) outBuffP = SUB_RR_START_CODE_ + REG_NUM_ * sdRegId + ftRegId;

            return 3;
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            *(int*) (outBuffP + 0) = SUB_RN_START_CODE_ + ftRegId;
            *(int*) (outBuffP + 3) = inBuffP[1];

            return 7;
        }

        return 0;

    }
    size_t putMul     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = inBuffP [0];
        _BYTE ftRegId = (modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_;
        _BYTE sdRegId = (modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            /* mov ft reg to eax, mov sd reg to ebx, mul, mov result to dest */
            *(int*)   (outBuffP + 0) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;
            *(int*)   (outBuffP + 3) = MOV_R_EBX_START_CODE_ + REG_NUM_ * sdRegId;

            *(short*) (outBuffP + 6) = 0xF7E3;

            *(int*)   (outBuffP + 8) = MOV_EAX_R_START_CODE_ + ftRegId;
            
            return 11;
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            /* mov number to eax, mov reg to ebx, mul, mov result to dest */
            *(_BYTE*) (outBuffP +  0) = MOV_N_EBX_START_CODE_;
            *(int*)   (outBuffP +  1) = inBuffP [1];
            *(int*)   (outBuffP +  5) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;

            *(short*) (outBuffP +  8) = 0xF7E3;
            
            *(int*)   (outBuffP + 10) = MOV_EAX_R_START_CODE_ + ftRegId * REG_NUM_;

            return 13;
        }

        return 0;
    }
    size_t putDiv     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        _AL_TYPE modifier = inBuffP [0];
        _BYTE ftRegId = (modifier >> BIN_OP_FT_REG_OFFSET_) & BIN_OP_REG_MASK_;
        _BYTE sdRegId = (modifier >> BIN_OP_SD_REG_OFFSET_) & BIN_OP_REG_MASK_;

        if (modifier & BIN_OP_SD_OPERAND_T_REG_MASK_)
        {
            /* mov ft reg to eax, mov sd reg to ebx, div, mov result to dest */
            *(int*)   (outBuffP +  0) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;
            *(int*)   (outBuffP +  3) = MOV_R_EBX_START_CODE_ + REG_NUM_ * sdRegId;

            *(short*) (outBuffP +  6) = XOR_EDX_EDX_;
            /* div ebx */
            *(short*) (outBuffP +  8) = 0xF7F3;

            *(int*)   (outBuffP + 10) = MOV_EAX_R_START_CODE_ + ftRegId;

            return 13;
        }

        else if (modifier & BIN_OP_SD_OPERAND_T_NUM_MASK_)
        {
            /* mov number to eax, mov reg to ebx, div, mov result to dest */
            *(_BYTE*) (outBuffP +  0) = MOV_N_EBX_START_CODE_;
            *(int*)   (outBuffP +  1) = inBuffP [1];
            *(int*)   (outBuffP +  5) = MOV_R_EAX_START_CODE_ + REG_NUM_ * ftRegId;

            *(short*) (outBuffP +  8) = XOR_EDX_EDX_;
            /* div ebx */
            *(short*) (outBuffP + 10) = 0xF7F3;
            
            *(int*)   (outBuffP + 12) = MOV_EAX_R_START_CODE_ + ftRegId * REG_NUM_;

            return 15;
        }

        return 0;
    }

/* ON-STACK BINARY OPERATIONS STUFF */

    size_t putAdds    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, add, push */
        *(short*) (outBuffP +  0) = POP_START_CODE_;
        *(short*) (outBuffP +  2) = POP_START_CODE_ + 3;

        /* add eax, ebx */
        *(short*) (outBuffP +  4) = 0x01D8;

        *(short*) (outBuffP +  6) = PUSH_START_CODE_;

        return 8;
    }
    size_t putSubs    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, sub, push */
        *(short*) (outBuffP +  0) = POP_START_CODE_;
        *(short*) (outBuffP +  2) = POP_START_CODE_ + 3;

        /* sub eax, ebx */
        *(short*) (outBuffP +  4) = 0x2BC3;

        *(short*) (outBuffP +  6) = PUSH_START_CODE_;

        return 8;
    }
    size_t putMuls    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, mul, push */
        *(short*) (outBuffP +  0) = POP_START_CODE_;
        *(short*) (outBuffP +  2) = POP_START_CODE_ + 3;

        /* mul ebx */
        *(short*) (outBuffP +  4) = 0xF7E3;
        
        *(short*) (outBuffP +  6) = PUSH_START_CODE_;

        return 8;
    }
    size_t putDivs    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP )
    {
        /* pop to eax & ebx, mul, push */
        *(short*) (outBuffP +  0) = POP_START_CODE_;
        *(short*) (outBuffP +  2) = POP_START_CODE_ + 3;

        /* div ebx */
        *(short*) (outBuffP +  4) = XOR_EDX_EDX_;
        *(short*) (outBuffP +  6) = 0xF7F3;
        
        *(short*) (outBuffP +  8) = PUSH_START_CODE_;

        return 10;
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