
#include "utils.hpp"

/* all cmds identifiers */
enum class CMDId_t {

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

    CMD_UNDEF_ = -0xFF  ,

};

typedef size_t (*cmdPutFunc_t) ( _AL_TYPE const * const inBuffP, _BYTE * outBuffP );

struct CMD_t {

  /* cmd identifier */
    CMDId_t id_ = CMDId_t::CMD_UNDEF_;

  /* func to put bin code */
    cmdPutFunc_t cmdPutFunc_;
};

/* TODO: use cmdPutFuncd_t !!? */

size_t putAdd     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putSub     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putMul     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putDiv     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putAdds    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putSubs    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putMuls    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putDivs    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putPush    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putPop     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putCmps    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putCall    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putExit    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putJe      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putJne     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putJae     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putJle     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putJa      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putJl      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putJmp     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putMov     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putIn      ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putOut     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putPopa    ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putPusha   ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );
size_t putRet     ( _AL_TYPE const * inBuffP, _BYTE * outBuffP );

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