/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include "gpio_defs.h"

#include "tft_lcd.h"
#include "font.h"

#include "LEDs.h"
#include "timers.h"
#include "sound.h"
#include "DMA.h"

#include "I2C.h"
#include "mma8451.h"
#include "delay.h"
#include "profile.h"
#include "math.h"

#include <RTL.h>
#include "tasks.h"

#define USE_GFX_LCD

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

	//char buffer[16];
	//
	PIT_Init(2399); // 24 MHz/(23999+1) = 1000 samples per second
	PIT_Start();
	//
	Init_Debug_Signals();
	Init_RGB_LEDs();                                                                                                                                                                                                                                                                                                                                                                                                                                             
	Sound_Init();	
	// Sound_Disable_Amp();
	Play_Tone();
	
	TFT_Init();
	TFT_Text_Init(1);
	TFT_Erase();
	//TFT_Text_PrintStr_RC(0,0, "Test Code");
	//sprintf(buffer, "Score: 0");
	TFT_Text_PrintStr_RC(0, 0,"Score: 0");
	//sprintf(buffer, "Life : 3");
	TFT_Text_PrintStr_RC(1, 0, "Life : 3");

/*
	Graphics_Test();
	while (1) 
		;
*/

//	TFT_TS_Calibrate();
//	TFT_TS_Test();

//	TFT_Text_PrintStr_RC(1,0, "Accel...");

	i2c_init();											// init I2C peripheral
	if (!init_mma()) {							// init accelerometer
		Control_RGB_LEDs(1,0,0);			// accel initialization failed, so turn on red error light
		while (1)
			;
	}
	//TFT_Text_PrintStr_RC(1,9, "Done");

	Play_Waveform_with_DMA();

	Delay(70);

	os_sys_init(&Task_Init);

	while (1)
		;
}

// *******************************ARM University Program Copyright � ARM Ltd 2013*************************************   
