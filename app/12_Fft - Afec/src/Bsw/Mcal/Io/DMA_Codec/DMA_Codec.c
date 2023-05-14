/** Main group of includes for board definitions, chip definitions and type definitions */
#include    "Std_types.h"
#include    "Codec_DMA.h"

/** Global DMA driver for all transfer */
static sXdmad dmad;
/** DMA channel for RX */
static uint32_t sscDmaRxChannel;
/** DMA channel for TX */
static uint32_t sscDmaTxChannel;

static sXdmadCfg xdmadCfg;

extern uint16_t AudioBuffer[2048];

static uint32_t AudioNextBuffer[TOTAL_Buffers] = { 0 };

/*    Alineacion de la linklist de escritura de DMA   */
COMPILER_ALIGNED(32) static LinkedListDescriporView1 dmaWriteLinkList[TOTAL_Buffers];
/*    Alineacion de la linklist de lectura de DMA    */
COMPILER_ALIGNED(32) static LinkedListDescriporView1 dmaReadLinkList[TOTAL_Buffers];

static uint8_t buf_flag = 1;
static bool cpu_flag = false;

/*  Interrupcion por DMA  */
void XDMAC_Handler(void){
	XDMAD_Handler(&dmad);
}

static void dummy(){}

static void sscDmaRxClk(uint32_t Channel, void* pArg)
{
	
	Channel = Channel;
	pArg = pArg;

	if (cpu_flag) {
		if (AudioNextBuffer[buf_flag] == 0)
			AudioNextBuffer[buf_flag] = (dmad.pXdmacs->XDMAC_CHID[sscDmaRxChannel].XDMAC_CNDA);
		else {
			TRACE_WARNING("DMA is faster than CPU-%d\n\r",buf_flag);
			AudioNextBuffer[buf_flag] = (dmad.pXdmacs->XDMAC_CHID[sscDmaRxChannel].XDMAC_CNDA);
		}
	}

	buf_flag++;
	if (buf_flag == TOTAL_Buffers) {
		buf_flag = 0;
		/*CPU starts to handle AudioNextBuffer, the first data are abandoned*/
		cpu_flag = true;
	}
}

/*      DMA configuracion   */
void DMA_Configure(void){
	sXdmad* pDmad = &dmad;

	/* Driver initialize */
	XDMAD_Initialize(pDmad, 0);
	/* Configure TWI interrupts */
	NVIC_ClearPendingIRQ(XDMAC_IRQn);
	NVIC_EnableIRQ(XDMAC_IRQn);
	/* Allocate DMA channels for SSC */
	sscDmaRxChannel = XDMAD_AllocateChannel(pDmad, ID_SSC, XDMAD_TRANSFER_MEMORY);
	if (sscDmaRxChannel == XDMAD_ALLOC_FAILED) {
		printf("xDMA channel allocation error\n\r");
		while (1);
	}

	XDMAD_SetCallback(pDmad, sscDmaRxChannel, sscDmaRxClk, 0);
	XDMAD_PrepareChannel(pDmad, sscDmaRxChannel);
}

/*     Graba el audio  y transferir    */
void PlayRecording(void)
{
	uint16_t* src;
	uint8_t i;
	uint32_t xdmaCndc;

/*Canal de RX*/
	src = &AudioBuffer[0];
	for (i = 0; i < TOTAL_Buffers; i++) {
		dmaReadLinkList[i].mbr_ubc = XDMA_UBC_NVIEW_NDV1
			| XDMA_UBC_NDE_FETCH_EN
			| XDMA_UBC_NSEN_UPDATED
			| XDMAC_CUBC_UBLEN(MAX_DMA_SIZE);
		dmaReadLinkList[i].mbr_sa = (uint32_t) & (AUDIO_IF->SSC_RHR);
		dmaReadLinkList[i].mbr_da = (uint32_t)(src);
		if (i == (TOTAL_Buffers - 1))
			dmaReadLinkList[i].mbr_nda = (uint32_t)&dmaReadLinkList[0];
		else
			dmaReadLinkList[i].mbr_nda = (uint32_t)&dmaReadLinkList[i + 1];
		src += MAX_DMA_SIZE;
	}

	xdmadCfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN
		| XDMAC_CC_MBSIZE_SINGLE
		| XDMAC_CC_DSYNC_PER2MEM
		| XDMAC_CC_CSIZE_CHK_1
		| XDMAC_CC_DWIDTH_HALFWORD
		| XDMAC_CC_SIF_AHB_IF1
		| XDMAC_CC_DIF_AHB_IF1
		| XDMAC_CC_SAM_FIXED_AM
		| XDMAC_CC_DAM_INCREMENTED_AM
		| XDMAC_CC_PERID(XDMAIF_Get_ChannelNumber(ID_SSC, XDMAD_TRANSFER_RX));
	xdmaCndc = XDMAC_CNDC_NDVIEW_NDV1
		| XDMAC_CNDC_NDE_DSCR_FETCH_EN
		| XDMAC_CNDC_NDSUP_SRC_PARAMS_UPDATED
		| XDMAC_CNDC_NDDUP_DST_PARAMS_UPDATED;

	
	//SCB_CleanInvalidateDCache();
	SCB_CleanDCache();
	/*XDMAC_CIE_BIE make interrupts can be generated on per block basis*/
	XDMAD_ConfigureTransfer(&dmad, sscDmaRxChannel, &xdmadCfg, xdmaCndc,
		(uint32_t)&dmaReadLinkList[0], XDMAC_CIE_BIE);

/*Canal de TX*/
src = &AudioBuffer[0];
	for (i = 0; i < TOTAL_Buffers; i++) {
		dmaWriteLinkList[i].mbr_ubc = XDMA_UBC_NVIEW_NDV1
			| XDMA_UBC_NDE_FETCH_EN
			| XDMA_UBC_NSEN_UPDATED
			| XDMAC_CUBC_UBLEN(MAX_DMA_SIZE);
		dmaWriteLinkList[i].mbr_sa = (uint32_t)(src);
		dmaWriteLinkList[i].mbr_da = (uint32_t)&(AUDIO_IF->SSC_THR);
		if (i == (TOTAL_Buffers - 1))
			dmaWriteLinkList[i].mbr_nda = (uint32_t)&dmaWriteLinkList[0];
		else
			dmaWriteLinkList[i].mbr_nda = (uint32_t)&dmaWriteLinkList[i + 1];
		src += MAX_DMA_SIZE;
	}

	xdmadCfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN
		| XDMAC_CC_MBSIZE_SINGLE
		| XDMAC_CC_DSYNC_MEM2PER
		| XDMAC_CC_CSIZE_CHK_1
		| XDMAC_CC_DWIDTH_HALFWORD
		| XDMAC_CC_SIF_AHB_IF1
		| XDMAC_CC_DIF_AHB_IF1
		| XDMAC_CC_SAM_INCREMENTED_AM
		| XDMAC_CC_DAM_FIXED_AM
		| XDMAC_CC_PERID(XDMAIF_Get_ChannelNumber(ID_SSC, XDMAD_TRANSFER_TX));
	xdmaCndc = XDMAC_CNDC_NDVIEW_NDV1
		| XDMAC_CNDC_NDE_DSCR_FETCH_EN
		| XDMAC_CNDC_NDSUP_SRC_PARAMS_UPDATED
		| XDMAC_CNDC_NDDUP_DST_PARAMS_UPDATED;

	SCB_CleanDCache();
	
	XDMAD_ConfigureTransfer( &dmad, sscDmaTxChannel, &xdmadCfg, xdmaCndc,
			(uint32_t)&dmaWriteLinkList[0], XDMAC_CIE_LIE);


	/*Start recording*/    	
	SSC_EnableReceiver(AUDIO_IF);
	XDMAD_StartTransfer(&dmad, sscDmaRxChannel);
	/*Wait for DMA to collect data*/
	Wait(400);
	
	/*stop recording so that data is not overide*/
	XDMAD_StopTransfer(&dmad, sscDmaRxChannel);

	Wait(400);
		/* Enable playback(SSC TX) */
	SSC_EnableTransmitter(AUDIO_IF);
	XDMAD_StartTransfer( &dmad, sscDmaTxChannel);
  SCB_CleanInvalidateDCache();

}


