
/*~~~~~~  Headers ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "board.h"
#include "stdio.h"
#include "Std_types.h"

#include "SSC_Codec.h"
#include "SSC_Codec_Config.h"

/*      Funcion publica     */

void SSC_Config(void){
        SSC_Configure(AUDIO_IF , 0 , SSC_MCK);
	    SSC_ConfigureReceiver(AUDIO_IF,I2S_SLAVE_RX_SETTING,I2S_SLAVE_RX_FRM_SETTING);
	    SSC_DisableReceiver(AUDIO_IF);
	    SSC_ConfigureTransmitter(AUDIO_IF,I2S_SLAVE_TX_SETTING,I2S_SLAVE_TX_FRM_SETTING);
	    SSC_DisableTransmitter(AUDIO_IF);
    }