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

/*~~~~~~  Local definitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  #define SAMP_PER  (50)
  #define BUFF_SIZE (2048)

/*~~~~~~  Global variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Auxiliary input buffer to accomodate data as FFT function expects it */
float       fft_inputData[BUFF_SIZE];
/** Output magnitude data */
float       fft_signalPower[BUFF_SIZE/2];
/** Auxiliary output variable that holds the frequency bin with the highest level of signal power */
uint32_t    u32fft_maxPowerIndex;
/** Auxiliary output variable that holds the maximum level of signal power */
float       fft_maxPower;

/*~~~~~~  Local functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void fft_process(void);

pfun pFFT = &fft_process;

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
	/* Disable watchdog */
	Wdg_Disable();
	/* Configure LEDs */
	LedCtrl_Configure();
  /* Configure Button */  
  ButtonCtrl_ConfigureSW0Button();
  /* Enable I and D cache */
	SCB_EnableICache();
	SCB_EnableDCache(); 
  /* Enable Floating Point Unit */
  Fpu_Enable();
    
	printf( "\n\r-- Scheduler Project %s --\n\r", SOFTPACK_VERSION ) ;
	printf( "-- %s\n\r", BOARD_NAME ) ;
	printf( "-- Compiled: %s %s With %s --\n\r", __DATE__, __TIME__ , COMPILER_NAME);

  /* Scheduler Inititalization */
  printf( "-- Scheduler Initialization --\n\r" ) ;
	SchM_Init(ScheduleConfig);
	
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