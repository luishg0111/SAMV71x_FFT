
/*~~~~~~  Headers ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "board.h"
#include "stdio.h"
#include "Std_types.h"

#include "SSC_Config.h"

/*      Macros */
#define SAMPLE_RATE             (48000)
#define SLOT_BY_FRAME           (2)
#define BITS_BY_SLOT            (16)
#define AUDIO_IF                SSC

//Cofiguraciones I2C
#define I2S_SLAVE_TX_SETTING     ((SSC_TCMR_CKS_TK) |     \
	(SSC_TCMR_CKO_NONE) |                        \
	(SSC_TCMR_START_TF_EDGE) |                   \
	(SSC_TCMR_STTDLY(1)) |                       \
	(SSC_TCMR_PERIOD(0)))

#define I2S_SLAVE_RX_SETTING     ((SSC_RCMR_CKS_TK) |   \
	(SSC_RCMR_CKO_NONE) |                      \
	(SSC_RCMR_CKI) |                           \
	(SSC_RCMR_START_RF_EDGE) |                 \
	(SSC_RCMR_STTDLY(1)) |                     \
	(SSC_RCMR_PERIOD(0)))

#define I2S_SLAVE_TX_FRM_SETTING ((SSC_TFMR_DATLEN(BITS_BY_SLOT - 1)) | \
	(SSC_TFMR_MSBF) |                                          \
	(SSC_TFMR_DATNB(SLOT_BY_FRAME - 1)) |                      \
	(SSC_TFMR_FSOS_NONE))

#define I2S_SLAVE_RX_FRM_SETTING ((SSC_RFMR_DATLEN(BITS_BY_SLOT - 1)) | \
	(SSC_RFMR_MSBF) |                                          \
	(SSC_RFMR_DATNB(SLOT_BY_FRAME - 1)) |                      \
	(SSC_RFMR_FSOS_NONE))


    /*      Funcion publica     */

void SSC_Config(void){
        SSC_Configure(AUDIO_IF , 0 , BOARD_MCK);
	    SSC_ConfigureReceiver(AUDIO_IF,I2S_SLAVE_RX_SETTING,I2S_SLAVE_RX_FRM_SETTING);
	    SSC_DisableReceiver(AUDIO_IF);
	    SSC_ConfigureTransmitter(AUDIO_IF,I2S_SLAVE_TX_SETTING,I2S_SLAVE_TX_FRM_SETTING);
	    SSC_DisableTransmitter(AUDIO_IF);
    }