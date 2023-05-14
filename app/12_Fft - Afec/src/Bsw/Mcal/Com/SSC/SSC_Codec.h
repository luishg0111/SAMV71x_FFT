#ifndef _SSC_Codec_H_
#define _SSC_Codec_H_

#include "board.h"
#include "Std_types.h"

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


extern void SSC_Config(void);

#endif