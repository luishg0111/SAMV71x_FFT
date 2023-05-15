/****************************************************************************************************/
/**
  \mainpage
  \n 
  \brief        Main application (main module)
  \author       Abraham Tezmol Otero, M.S.E.E
  \project      Tau 
  \version      1.0
  \date         12/Jun/2016
   

/*~~~~~~  Headers ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Main group of includes for board definitions, chip definitions and type definitions */
#include    "Std_types.h"
/** Task scheduler definitions */
#include    "SchM.h"
/** LED control definitions */ 
#include    "Led_Ctrl.h"
/** Watchdog control function prototypes definitions */
#include    "Wdg.h"
/** Button control operations */
#include    "Button_Ctrl.h"
/** Floating Point Unit */
#include    "Fpu.h"
/*  DMA library    */
#include    "DMA_Codec.h"
/*  AudioCodec wm8904 library    */
#include "wm8904.h"
/*  SSC_Codec library    */
#include "SSC_Codec.h"

/*~~~~~~  Local definitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define BUFF_SIZE (2048)
/** TWI High Speed clock */
#define TWI_CLOCK		  400000

/*~~~~~~  Global variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Auxiliary input buffer to accomodate data as FFT function expects it */
float       fft_inputData[BUFF_SIZE] = {0.0};
/** Output magnitude data */
float       fft_signalPower[BUFF_SIZE/2] = {0.0};
/** Auxiliary output variable that holds the frequency bin with the highest level of signal power */
uint32_t    u32fft_maxPowerIndex = 0;
/** Auxiliary output variable that holds the maximum level of signal power */
float       fft_maxPower = 0.0;

uint16_t AudioBuffer[2048];     
float fft_signalPower[FFT_BUFF_SIZE / 2];  

/** configuracion de los pines de TWI0, pinesde trasmision de data, clock, frame, RD, RK, RF . */
static const Pin SscTwiPins[] = { PIN_TWI_TWD0,
                                  PIN_TWI_TWCK0,
                                  PIN_SSC_TD,
                                  PIN_SSC_TK,
                                  PIN_SSC_TF,
                                  PIN_SSC_RD,
                                  PIN_SSC_RK,
                                  PIN_SSC_RF,
                                  PIN_PCK2 };

/*    TWI instancia    */
static Twid twid;

/*~~~~~~  Local functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void fft_process(void);

pfun pFFT = &fft_process;

/*    Interrupcion de TWI    */
void TWIHS0_Handler(void)
{
	TWID_Handler(&twid);
}

/*      Transferir el buffer del Codec al de enrtrada de FFT  */
static void Codec_To_InputFFT(void)
{
  int i = 0;
	for (i = 0; i < FFT_BUFF_SIZE; i ++) {
		fft_inputData[i] = (float)AudioBuffer[i];
	}
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
/**
 *  \brief getting-started Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{
  uint16_t data = 0;
  
	/* Disable watchdog */
	Wdg_Disable();

  /* Enable I and D cache */
	SCB_EnableICache();
	SCB_EnableDCache(); 

  /* Enable Floating Point Unit */
  Fpu_Enable();
    
	printf( "\n\r-- Scheduler Project %s --\n\r", SOFTPACK_VERSION ) ;
	printf( "-- %s\n\r", BOARD_NAME ) ;
	printf( "-- Compiled: %s %s With %s --\n\r", __DATE__, __TIME__ , COMPILER_NAME);

  printf("Configuracion del SystemSystick a 1ms.\n\r");
  if (TimeTick_Configure())
		printf("Error en la config. Systick.\n\r");

	/* Configuracion de los pines */
	PIO_Configure(SscTwiPins, PIO_LISTSIZE(SscTwiPins));

	/* Configure SSC */
	SSC_Config();

	/* Configuracion del DMA */
	DMA_Configure();

  	/* Configuracion y habilitacion de TWIHS0  */
	PMC_EnablePeripheral(ID_TWIHS0);
	TWI_ConfigureMaster(TWIHS0, TWI_CLOCK, BOARD_MCK);
	TWID_Initialize(&twid, TWIHS0);

	/* Configuracion de la interrupcion de TWI */
	NVIC_ClearPendingIRQ(TWIHS0_IRQn);
	NVIC_EnableIRQ(TWIHS0_IRQn);

	/* check that WM8904 is present */
	WM8904_Write(&twid, WM8904_SLAVE_ADDRESS, 22, 0);
	data = WM8904_Read(&twid, WM8904_SLAVE_ADDRESS, 0);
	if (data != 0x8904) {
		printf("WM8904 not found!\n\r");
		while (1);
	}

	/* Initialize the audio DAC */
	WM8904_Init(&twid, WM8904_SLAVE_ADDRESS, PMC_MCKR_CSS_SLOW_CLK);
	/* Enable the DAC master clock */
	PMC_ConfigurePCK2(PMC_MCKR_CSS_SLOW_CLK, PMC_MCKR_PRES_CLK_1); 

	// Record data from codec using SSC
	DMA_PlayRecording();

	//Cast DMA uint16 data array to float for usage with the FFT function
	Codec_To_InputFFT();

	//Apply FFT to get frequency of the obtained data  
	fft_process();
	//Nothing else to do
	while (1);
	
	/* Should never reach this code */
  for(;;)
  {
	  printf( "-- Unexpected Error at Scheduler Initialization --\n\r" ) ;
	}
}

void fft_process(void)
{
  /** Perform FFT on the input signal */
  fft(fft_inputData, fft_signalPower, BUFF_SIZE/2, &u32fft_maxPowerIndex, &fft_maxPower);
        
  /* Publish through emulated Serial */
  printf("%5d  %5.4f \r\n", u32fft_maxPowerIndex, fft_maxPower);
}