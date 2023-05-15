#ifndef _SSC_Codec_Cfg_H_
#define _SSC_Codec_Cfg_H_

#include "board.h"
#include "Std_types.h"

/*      Macros */
#define SAMPLE_RATE             (48000)
#define SLOT_BY_FRAME           (1)
#define BITS_BY_SLOT            (16)
#define SSC_IF                  SSC
#define SSC_MCK                 BOARD_MCK

#endif