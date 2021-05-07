
#include "utils.hpp"

/* all cmds identifiers */
enum class CMDId_t  : BYTE__ {

    CMD_ADD_   = 0x00   ,
    CMD_SUB_   = 0x01   ,
    CMD_MUL_   = 0x02   ,
    CMD_DIV_   = 0x03   ,

    CMD_ADDS_  = 0x50   ,
    CMD_SUBS_  = 0x51   ,
    CMD_MULS_  = 0x52  ,
    CMD_DIVS_  = 0x53   ,
    
    CMD_PUSH_  = 0x04   ,
    CMD_POP_   = 0x06   ,

    CMD_CMPS_  = 0x07   ,

    CMD_CALL_  = 0x08   ,
    
    CMD_EXIT_  = 0x09   ,
    
    CMD_JE_    = 0x0A   ,
    CMD_JNE_   = 0x0B   ,
    CMD_JAE_   = 0x0C   ,
    CMD_JLE_   = 0x0D   ,
    CMD_JA_    = 0x0E   ,
    CMD_JL_    = 0x0F   ,
    CMD_JMP_   = 0x10   ,
    
    CMD_MOV_   = 0xFF   ,
    
    CMD_IN_    = 0xEC   ,
    CMD_OUT_   = 0xED   ,
    
    CMD_POPA_  = 0xEF   ,
    CMD_PUSHA_ = 0xF0   ,
    
    CMD_RET_   = 0xF1   ,

    CMD_UNDEF_ = 0x12   ,

};

struct retOff_t {

    size_t outBuffOff_ = 0;
    size_t inBuffOff_ = 0;

};

typedef retOff_t (*cmdPutFunc_t) ( AL_TYPE__ const * const inBuffP, BYTE__ * outBuffP );

struct CMD_t {

  /* cmd identifier */
    CMDId_t id_ = CMDId_t::CMD_UNDEF_;

  /* func to put bin code */
    cmdPutFunc_t cmdPutFunc_;
};

/* TODO: use cmdPutFuncd_t !!? */
retOff_t putAdd     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putSub     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putMul     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putDiv     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putAdds    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putSubs    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putMuls    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putDivs    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putPush    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putPop     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putCmps    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putCall    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putExit    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putJe      ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putJne     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putJae     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putJle     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putJa      ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putJl      ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putJmp     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putMov     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putIn      ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putOut     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putPopa    ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putPusha   ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );
retOff_t putRet     ( AL_TYPE__ const * inBuffP, BYTE__ * outBuffP );

const CMD_t CMDS_[] = {

    { CMDId_t::CMD_ADD_      , putAdd     } ,
    { CMDId_t::CMD_SUB_      , putSub     } ,
    { CMDId_t::CMD_MUL_      , putMul     } ,
    { CMDId_t::CMD_DIV_      , putDiv     } ,
    { CMDId_t::CMD_ADDS_     , putAdds    } ,
    { CMDId_t::CMD_SUBS_     , putSubs    } ,
    { CMDId_t::CMD_MULS_     , putMuls    } ,
    { CMDId_t::CMD_DIVS_     , putDivs    } ,
    { CMDId_t::CMD_PUSH_     , putPush    } ,
    { CMDId_t::CMD_POP_      , putPop     } ,
    { CMDId_t::CMD_CMPS_     , putCmps    } ,
    { CMDId_t::CMD_CALL_     , putCall    } ,
    { CMDId_t::CMD_EXIT_     , putExit    } ,
    { CMDId_t::CMD_JE_       , putJe      } ,
    { CMDId_t::CMD_JNE_      , putJne     } ,
    { CMDId_t::CMD_JAE_      , putJae     } ,
    { CMDId_t::CMD_JLE_      , putJle     } ,
    { CMDId_t::CMD_JA_       , putJa      } ,
    { CMDId_t::CMD_JL_       , putJl      } ,
    { CMDId_t::CMD_JMP_      , putJmp     } ,
    { CMDId_t::CMD_MOV_      , putMov     } ,
    { CMDId_t::CMD_IN_       , putIn      } ,
    { CMDId_t::CMD_OUT_      , putOut     } ,
    { CMDId_t::CMD_POPA_     , putPopa    } ,
    { CMDId_t::CMD_PUSHA_    , putPusha   } ,
    { CMDId_t::CMD_RET_      , putRet     }

};

/* num of registers */
    const size_t REG_NUM_ = 8;

/* MY FORMAT BINARY OPERATIONS STUFF */

  /* binary operations (except mov) */
    /* mask to check if second operand in binary operation cmd is number */
        const AL_TYPE__ BIN_OP_SD_OPERAND_T_NUM_MASK_ = 1;
    /* mask to check if second operand in binary operation cmd is register */
        const AL_TYPE__ BIN_OP_SD_OPERAND_T_REG_MASK_ = 1 << 1;

    /* mask to get register id in first 4 bits */
        const BYTE__ BIN_OP_REG_MASK_ = 0x7;

    /* mask to get first register id for binary operation cmd */
        const AL_TYPE__ BIN_OP_FT_REG_OFFSET_ = 16;
    /* mask to get second register id for binary operation cmd */
        const AL_TYPE__ BIN_OP_SD_REG_OFFSET_ = 8;

  /* mov props masks */
        const AL_TYPE__ FT_REG_ON_ = 1 << 15;
        const AL_TYPE__ SD_REG_ON_ = 1 << 14;

        const AL_TYPE__ FT_MEM_ON_ = 1 << 13;
        const AL_TYPE__ SD_MEM_ON_ = 1 << 12;

        const AL_TYPE__ FT_CONST_ON_ = 1 << 11;
        const AL_TYPE__ SD_CONST_ON_ = 1 << 10;

        const AL_TYPE__ FT_SIGN_PLUS_ = 1 << 9;
        const AL_TYPE__ SD_SIGN_PLUS_ = 1 << 8;

        const AL_TYPE__ FT_REG_ID_ = 0xF0;
        const AL_TYPE__ SD_REG_ID_ = 0x0F;

/* X86-64 BINARY OPERATIONS STUFF */

  /* MOV */
    /* mov num to ebx */
        const BYTE__ MOV_N_EBX_CODE_ = 0xBB;

    /* mov reg to eax first code (r8b - r15b) */
        const DWRD__ MOV_R_EAX_START_CODE_ = 0xC08944;
    /* mov reg to ebx first code (r8b - r15b) */
        const DWRD__ MOV_R_EBX_START_CODE_ = 0xC38944;

    /* mov eax to reg (r8b - r15b) */
        const DWRD__ MOV_EAX_R_START_CODE_ = 0xC08941;
    
  /* ADD */
    /* add reg to reg first code (r8b - r15b) */
        const DWRD__ ADD_RR_START_CODE_ = 0xC00145;
    /* add number to reg first tode (r8b - r15b) */
        const DWRD__ ADD_RN_START_CODE_ = 0xC08141;

  /* SUB */
    /* sub reg from reg first code (r8b - r15b) */
        const DWRD__ SUB_RR_START_CODE_ = 0xC02945;
    /* sub num from reg first code (r8b - r15b) */
        const DWRD__ SUB_RN_START_CODE_ = 0xE88141;

  /* XOR */
    /* xor edx, edx - for div opr */
        const WORD__ XOR_EDX_EDX_CODE_ = 0x33D2;

  /* PUSH */
    /* push from reg (r8 - r15) */
        const WORD__ PUSH_R_START_CODE_ = 0x5041;

    /* also needed for stack oprs */
        const BYTE__ PUSH_EAX_CODE_ = 0x50;
        const BYTE__ PUSH_NUM_CODE_ = 0x68;

  /* POP */
    /* pop to reg (r8 - r15) */
        const WORD__ POP_START_CODE_ = 0x5841;

    /* for empty pops */
        const BYTE__ POP_EAX_CODE_ = 0x58;

  /* CMP */
    /* cmp eax, ebx */
        const WORD__ CMP_EAX_EBX_CODE_ = 0x39D8;

  /* RET */
        const BYTE__ RET_CODE_ = 0xC3;

  /* CALL */
        const BYTE__ CALL_CODE_ = 0xE8;

  /* JUMPS */
        const BYTE__ JMP_CODE_ = 0xE9;

        const WORD__ JAA_CODE_ = 0x0f87;
        const WORD__ JAE_CODE_ = 0x0f83;
        const WORD__ JLL_CODE_ = 0x0f82;
        const WORD__ JLE_CODE_ = 0x0f86;
        const WORD__ JEQ_CODE_ = 0x0f84;
        const WORD__ JNE_CODE_ = 0x0f85;

  /* MOV */
    /* mov reg, reg (r8b - r15b) */
        const WORD__ MOV_RR_ = 0;