#include <stdint.h>
#include <stdio.h>
#include <RTL.h>
#include <MKL25Z4.h>

#include "TFT_LCD.h"
#include "font.h"
#include "tasks.h"
#include "MMA8451.h"
#include "sound.h"
#include "DMA.h"
#include "gpio_defs.h"


U64 RA_Stack[64];

OS_TID t_Read_TS, t_Read_Accelerometer, t_Sound_Manager, t_US, t_Refill_Sound_Buffer;
OS_MUT LCD_mutex;
OS_MUT TS_mutex;

int16_t coin_X_pos=100;
int16_t coin_Y_pos=100;
int16_t score=0;
int16_t life=3;
void Init_Debug_Signals(void) {
	// Enable clock to port B
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Make 3 pins GPIO
	PORTB->PCR[DEBUG_T0_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[DEBUG_T0_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[DEBUG_T1_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[DEBUG_T1_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[DEBUG_T2_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[DEBUG_T2_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[DEBUG_T3_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[DEBUG_T3_POS] |= PORT_PCR_MUX(1);          
	
	// Set ports to outputs
	PTB->PDDR |= MASK(DEBUG_T0_POS);
	PTB->PDDR |= MASK(DEBUG_T1_POS);
	PTB->PDDR |= MASK(DEBUG_T2_POS);
	PTB->PDDR |= MASK(DEBUG_T3_POS);
	
	// Initial values are 0
	PTB->PCOR = MASK(DEBUG_T0_POS);
	PTB->PCOR = MASK(DEBUG_T1_POS);
	PTB->PCOR = MASK(DEBUG_T2_POS);
	PTB->PCOR = MASK(DEBUG_T3_POS);
}	

__task void Task_Init(void) {
	
	os_mut_init(&LCD_mutex);
	
	t_Read_TS = os_tsk_create(Task_Read_TS, 4);
	t_Read_Accelerometer = os_tsk_create_user(Task_Read_Accelerometer, 3, RA_Stack, 512);
	t_Sound_Manager = os_tsk_create(Task_Sound_Manager, 2);
	t_US = os_tsk_create(Task_Update_Screen, 5);
	t_Refill_Sound_Buffer = os_tsk_create(Task_Refill_Sound_Buffer, 1);


  os_tsk_delete_self ();
}

__task void Task_Read_TS(void) {
	PT_T p, pp;
	COLOR_T c;
	
	c.R = 150;
	c.G = 200;
	c.B = 255;
	
	os_itv_set(TASK_READ_TS_PERIOD_TICKS);

/*	TFT_Text_PrintStr_RC(TFT_MAX_ROWS-3, 0, "Mute");
	TFT_Text_PrintStr_RC(TFT_MAX_ROWS-3, 12, "Unmute");
	*/
	while (1) {
		os_itv_wait();
		PTB->PSOR = MASK(DEBUG_T1_POS);
		if (TFT_TS_Read(&p)) { 
			// Send message indicating screen was pressed
			// os_evt_set(EV_PLAYSOUND, t_Sound);

			if (p.Y > 240) { 
				if (p.X < TFT_WIDTH/2) {
					Sound_Disable_Amp();
				} else {
					Sound_Enable_Amp();
				}
			} else {
				// Now draw on screen
				if ((pp.X == 0) && (pp.Y == 0)) {
					pp = p;
				}
				os_mut_wait(&LCD_mutex, WAIT_FOREVER);
				TFT_Draw_Line(&p, &pp, &c);
				os_mut_release(&LCD_mutex);
				pp = p;
			} 
		} else {
			pp.X = 0;
			pp.Y = 0;
		}
		PTB->PCOR = MASK(DEBUG_T1_POS);
	}
}

__task void Task_Read_Accelerometer(void) {
	char buffer[16];
	
	os_itv_set(TASK_READ_ACCELEROMETER_PERIOD_TICKS);

	while (1) {
		os_itv_wait();
		PTB->PSOR = MASK(DEBUG_T0_POS);
		read_full_xyz();
		convert_xyz_to_roll_pitch();

		sprintf(buffer, "Score: %d", score);
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Text_PrintStr_RC(2, 0, buffer);
		os_mut_release(&LCD_mutex);

		sprintf(buffer, "Life: %d", life);
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Text_PrintStr_RC(3, 0, buffer);
		os_mut_release(&LCD_mutex);

		PTB->PCOR = MASK(DEBUG_T0_POS);
	}
}




__task void Task_Update_Screen(void) {
	int16_t paddle_pos=TFT_WIDTH/2;
	//int16_t coin_pos=100;
	PT_T p1, p2,c1,c2,d1;
	COLOR_T paddle_color, black;
	
	d1.X=150;
	d1.Y=150;
	paddle_color.R = 100;
	paddle_color.G = 200;
	paddle_color.B = 50;

	black.R = 0;
	black.G = 0;
	black.B = 0;
	
	os_itv_set(TASK_UPDATE_SCREEN_PERIOD_TICKS);

	while (1) {
		os_itv_wait();
		PTB->PSOR = MASK(DEBUG_T3_POS);
		
		if ((roll < -2.0) || (roll > 2.0)) {
			p1.X = paddle_pos;
			p1.Y = PADDLE_Y_POS;
			p2.X = p1.X + PADDLE_WIDTH;
			p2.Y = p1.Y + PADDLE_HEIGHT;
			TFT_Fill_Rectangle(&p1, &p2, &black); 		
			
			paddle_pos += roll;
			paddle_pos = MAX(0, paddle_pos);
			paddle_pos = MIN(paddle_pos, TFT_WIDTH-PADDLE_WIDTH-1); // this needs to be cahnged to TFT_wdith - paddle width -1
			
			p1.X = paddle_pos;
			p1.Y = PADDLE_Y_POS;
			p2.X = p1.X + PADDLE_WIDTH;
			p2.Y = p1.Y + PADDLE_HEIGHT;
			TFT_Fill_Rectangle(&p1, &p2, &paddle_color); 	
		}		
			TFT_Fill_Rectangle(&c1, &c2, &black);
		
			c1.X = coin_X_pos;
			c1.Y=coin_Y_pos;
			c2.X= coin_X_pos+COIN_RADIUS;
			c2.Y= coin_Y_pos+COIN_RADIUS;
			if(coin_Y_pos<300)
			coin_Y_pos=coin_Y_pos+5;
				
			if(coin_Y_pos>=300)
				{ 
					if((coin_X_pos>paddle_pos)&&(coin_X_pos<paddle_pos+PADDLE_WIDTH))
						score+=1;
					else
						life=life-1;
			if(life <0 )
			{
				TFT_Text_PrintStr_RC(TFT_MAX_ROWS-4, 4, "Game Over");
				TFT_Text_PrintStr_RC(TFT_MAX_ROWS-3, 4, "Press Reset");
				while(1);
			}
					coin_Y_pos=100;
			
					coin_X_pos=rand()%240;
				}
			TFT_Fill_Rectangle(&c1,&c2,&paddle_color);
		//  TFT_Plot_Pixel(&d1,&paddle_color);
		
		PTB->PCOR = MASK(DEBUG_T3_POS);
	}
}
