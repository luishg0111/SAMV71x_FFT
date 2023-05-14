#ifndef CODEC_DMA_H_
#define CODEC_DMA_H_

#include "Std_Types.h"

/** DMA Descriptor */
#define TOTAL_Buffers            2
#define AUDIO_IF                SSC

/** MAX size of the recorded sound */
#define MAX_RECORD_SIZE         0xFFFFFFFF

/** MAX size of one DMA transfer */
#define MAX_DMA_SIZE            0x1000

/* Buffer Size*/
#define FFT_BUFF_SIZE (2048)

extern void DMA_Configure(void);
extern void PlayRecording(void);

#endif