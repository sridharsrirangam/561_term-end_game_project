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
U64 RA2_Stack[64];

OS_TID t_Read_TS, t_Read_Accelerometer, t_Sound_Manager, t_US,
							t_Refill_Sound_Buffer, t_GameStats, t_CPUStats;
OS_MUT LCD_mutex;
OS_MUT TS_mutex;
OS_MUT PAUSE_GAME_mutex;


OS_MBX ACCL_mailbox;
OS_MBX TS_mailbox;
os_mbx_declare(ACCL_mailbox,2);
os_mbx_declare(TS_mailbox,2);

int16_t coin_X_pos=100;
int16_t coin_Y_pos=100;

//float roll=0.0;
int test;
int test2;

extern uint32_t count_t_Read_TS,
        count_t_Read_Accelerometer,
	 			count_t_Sound_Manager,
				count_t_US,
				count_t_Refill_Sound_Buffer,
				count_t_GameStats,
				count_t_CPUStats,
				count_idle,
				count_total;


  float util_t_Read_TS=0,
        util_t_Read_Accelerometer=0,
	 			util_t_Sound_Manager=0,
				util_t_US=0,
				util_t_Refill_Sound_Buffer=0,
				util_t_GameStats=0,
				util_t_CPUStats=0,
				util_idle=0,
       	util_total=0;	

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
	os_mut_init(&PAUSE_GAME_mutex);
	
	
	t_Read_TS 						= os_tsk_create(Task_Read_TS, 5);
	t_Read_Accelerometer 	= os_tsk_create_user(Task_Read_Accelerometer, 4, RA_Stack, 512);
	t_Sound_Manager 			= os_tsk_create(Task_Sound_Manager, 2);
	t_US 									= os_tsk_create(Task_Update_Screen, 7);
	t_Refill_Sound_Buffer = os_tsk_create(Task_Refill_Sound_Buffer, 1);
	t_GameStats 					= os_tsk_create(Task_GameStats, 6);
	t_CPUStats 						= os_tsk_create(Task_CPUStats,  3);
// t_CPUStats 						= os_tsk_create_user(Task_CPUStats,  3, RA2_Stack,512);

  os_tsk_delete_self ();
}

__task void Task_Read_TS(void) {
	PT_T p;
	int* TRT_mbx_buf;
	uint16_t temp_Yval;
	int debounce=0;
	
	//os_mbx_init(&TS_mailbox,sizeof(TS_mailbox)); // number of elements being sent
	
	
//	Sound_Disable_Amp();
	
	os_itv_set(TASK_READ_TS_PERIOD_TICKS);
	
	
	while (1) {
		
		os_itv_wait();
		
    
		PTB->PSOR = MASK(DEBUG_T1_POS);
		
		temp_Yval =0;
		
	//	if(os_evt_wait_and(EV_RECD_CPU_STAT,0x00)==OS_R_TMO)
//		{
			if(debounce>0)
				--debounce;
			
			if ((TFT_TS_Read(&p))&&(debounce==0)) {
				debounce=25;
				temp_Yval = p.Y;
			
				if(temp_Yval>=50)
				{
					os_evt_set(EV_TOUCH_DETECTED,t_US);
					//temp_Yval = 0;
				}
				else
				{// send mailbox
					//*TRT_mbx_buf = (int)temp_Yval;
					//os_mbx_send(&TS_mailbox,TRT_mbx_buf,0xFFFF);
					//os_evt_set(EV_CPU_STAT_UPDATE,t_CPUStats);
					//temp_Yval = 0;
				}
				//roll = p.Y;
				//test=temp_Yval;
				//test = temp_Yval;
				//*TRT_mbx_buf = test;
				//free(TRT_mbx_buf);
				TRT_mbx_buf = (int*) malloc(sizeof(int));
				*TRT_mbx_buf = (int)temp_Yval;
				os_mbx_send(&TS_mailbox,TRT_mbx_buf,0x0);
				
				if(os_evt_wait_and(EV_RECD_CPU_STAT,0x00) !=OS_R_TMO)
				{ 
					test=100;
					os_evt_set(EV_CPU_STAT_DONE,t_CPUStats);
					//while(1);
				}
			}//if touch
	/*	}
//		else
	//	{//send CPU Stat request
			if (TFT_TS_Read(&p)) {
				os_evt_set(EV_CPU_STAT_DONE,t_CPUStats);
			}//if touch anywhere
			
		}
*/
		PTB->PCOR = MASK(DEBUG_T1_POS);
	}
}
//-------------------------------------------------------------------
__task void Task_Read_Accelerometer(void) {
	char buffer[16];
	
	//buffer to send data using a mailbox
	float* TRA_mbx_buf;
	
	//------------------------------------------------------------
	
	os_mbx_init(&ACCL_mailbox,sizeof(ACCL_mailbox)); // number of elements being sent
	
	
	//TRA_mbx_buf = (float*) malloc(sizeof(float));
		
	
	os_itv_set(TASK_READ_ACCELEROMETER_PERIOD_TICKS);

	while (1) {
		//os_mut_wait(&PAUSE_GAME_mutex,WAIT_FOREVER);
		
		os_itv_wait();
		PTB->PSOR = MASK(DEBUG_T0_POS);
		//roll = read_full_xyz();
		
		
		TRA_mbx_buf = (float*) malloc(sizeof(float));
		
		*TRA_mbx_buf = read_full_xyz();
		
		
		 
		
		// send with maximum timeout less than infinity
		if((os_mbx_send(&ACCL_mailbox,TRA_mbx_buf,0xFFFF)) != OS_R_OK)
	  //if(0)
		{
			sprintf(buffer, "Error in MBX Send!!");
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Text_PrintStr_RC(TFT_MAX_ROWS-5, 3, buffer);
			os_mut_release(&LCD_mutex);
			//while(1); // only for debugging
		}			
		
		//convert_xyz_to_roll_pitch();
#if 0
		sprintf(buffer, "Roll: %6.2f", *TRA_mbx_buf);
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Text_PrintStr_RC(2, 0, buffer);
		os_mut_release(&LCD_mutex);
#endif
		
		PTB->PCOR = MASK(DEBUG_T0_POS);
		//sos_mut_release(&PAUSE_GAME_mutex);
	}
}


__task void Task_Update_Screen(void) {
	int16_t paddle_pos=TFT_WIDTH/2;
	//int16_t coin_pos=100;
	int16_t score=0;
  int16_t life=3;
	
	PT_T p1, p2,c1,c2,d1;
	COLOR_T paddle_color, black,coin_color;
	char buffer[16];
	int i,j;
	int flag=1;
	
	//For mailboxes
	float *TUS_mbx_buf;// for Accel
	float rollVal;
	
	uint8_t fastFlag,speed=5;
	
	
	
	int8_t array_number [10][10] = {0,0,0,1,1,1,0,0,0,0,
																	0,0,1,1,1,1,1,1,0,0,
																	0,1,1,1,1,1,1,1,1,0,
																	0,1,1,1,1,1,1,1,1,0,
																	1,1,1,1,1,1,1,1,1,1,
																	1,1,1,1,1,1,1,1,1,1,
																	0,1,1,1,1,1,1,1,1,1,
																	0,1,1,1,1,1,1,1,1,0,
																	0,0,1,1,1,1,1,1,0,0,
																	0,0,0,0,1,1,1,0,0,0};
	

																	
	d1.X=150;
	d1.Y=150;
	paddle_color.R = 100;
	paddle_color.G = 200;
	paddle_color.B = 50;
  
	coin_color.R = 200;
	coin_color.G= 50;
	coin_color.B = 50;
	black.R = 0;
	black.G = 0;
	black.B = 0;
	
	os_itv_set(TASK_UPDATE_SCREEN_PERIOD_TICKS);

	while (1) {
		
		//os_mut_wait(&PAUSE_GAME_mutex,WAIT_FOREVER);
		os_itv_wait();
		
		
		if(os_evt_wait_and(EV_CPU_STAT_DISP,0x00)!= OS_R_TMO)
		{// wait here until the CPU stats is done dispaying
			flag = 0;
		}
		
		if(os_evt_wait_and(EV_CPU_STAT_DONE,0x00) != OS_R_TMO)
		{
			flag = 1;
		}
		
		if(flag)
		{
	
		Sound_Enable_Amp();
		
		
		
		if(os_mbx_wait(&ACCL_mailbox, (void*)&TUS_mbx_buf,0xFFFF) == OS_R_TMO)
		//if(0)
		{ 
			sprintf(buffer, "Error in MBX Receive!!");
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Text_PrintStr_RC(TFT_MAX_ROWS - 8, 4, buffer);
			os_mut_release(&LCD_mutex);
			//while(1); // only for debugging
			
		}
		
		rollVal = *TUS_mbx_buf;
		
		free(TUS_mbx_buf);
		
		
		
	  if(os_evt_wait_and(EV_TOUCH_DETECTED,0x00)!= OS_R_TMO)
		{
			speed += 5; 
		}
		//test= speed;

		
		PTB->PSOR = MASK(DEBUG_T3_POS);
		
		if ((rollVal < -2.0) || (rollVal > 2.0)) {
			p1.X = paddle_pos;
			p1.Y = PADDLE_Y_POS;
			p2.X = p1.X + PADDLE_WIDTH;
			p2.Y = p1.Y + PADDLE_HEIGHT;
			TFT_Fill_Rectangle(&p1, &p2, &black); 		
			
			paddle_pos += rollVal;
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
			{
				//coin_Y_pos=coin_Y_pos+5;
				
					coin_Y_pos=coin_Y_pos+speed;
				
			}
			

			if(coin_Y_pos>=300)
				{ 
					speed = 5;
					
					
					if((coin_X_pos>paddle_pos)&&(coin_X_pos<paddle_pos+PADDLE_WIDTH))
					{	
						//score+=1;
						os_evt_set(EV_SCORE_UPDATE,t_GameStats);
						os_evt_set(EV_PLAYSOUND,t_Sound_Manager);
					}
					else
					{
						os_evt_set(EV_LIFE_UPDATE,t_GameStats);
					}
			
					coin_Y_pos=100;
			
					coin_X_pos=(rand()) % 230;
				}
	//		TFT_Fill_Rectangle(&c1,&c2,&paddle_color);
		//  TFT_Plot_Pixel(&d1,&paddle_color);
		
				for(i=0;i<10;i++)
				{
					for(j=0;j<10;j++)
					{ d1.X =c1.X + i;
						d1.Y = c1.Y +j;
						if(array_number[i][j]==1)
						TFT_Plot_Pixel(&d1,&coin_color);
						else
						TFT_Plot_Pixel(&d1,&black);
				}
					
			//os_mut_release(&PAUSE_GAME_mutex);	
			}
		PTB->PCOR = MASK(DEBUG_T3_POS);
		}// flag
	}//while(1)
}

__task void Task_GameStats(void)
{
	char buffer[16];
	uint16_t score=0;
	uint8_t life=3;
	
	
	while(1)
	{
		
		os_evt_wait_or((EV_LIFE_UPDATE|EV_SCORE_UPDATE), WAIT_FOREVER);
		
		if(os_evt_get() == EV_LIFE_UPDATE)
		{
			if(life!=0)
			{
				--life;
			}
			else
			{
				TFT_Text_PrintStr_RC(TFT_MAX_ROWS-4, 4, "Game Over");
				TFT_Text_PrintStr_RC(TFT_MAX_ROWS-3, 4, "Press Reset");
				while(1);
			}
		}
		else if(os_evt_get() == EV_SCORE_UPDATE)
		{
			++score;
		/*	if(score>5)
			{os_evt_set(EV_CPU_STAT_UPDATE,t_CPUStats);}*/
			//Play_Tone();
		}
		
		sprintf(buffer, "Score: %d", score);
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Text_PrintStr_RC(0, 0, buffer);
		os_mut_release(&LCD_mutex);

		sprintf(buffer, "Life: %d", life);
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Text_PrintStr_RC(1, 0, buffer);
		os_mut_release(&LCD_mutex);
	}
}


__task void Task_CPUStats(void)
{ 
	
	char buffer[16];
	int y_pos;
	int* CPUS_mbx_buf;
	int iter;
	
	os_itv_set(1000);
	
	while(1)
	{
		os_itv_wait();
	//os_evt_wait_and(EV_CPU_STAT_UPDATE,WAIT_FOREVER);
		
	  os_mbx_wait(&TS_mailbox, (void*)&CPUS_mbx_buf,0xffff);
	  
		
		y_pos= *CPUS_mbx_buf;
	
		//test2=y_pos;
		
		free(CPUS_mbx_buf);
    //test=y_pos;
		//y_pos = 51;
		//TFT_Erase();
		
  	 if(y_pos<50)
		 {
			 //while(os_evt_wait_and(EV_END_CPU_STAT,0x0000)!=OS_R_OK)
			// {
       //os_mbx_wait(&TS_mailbox, (void*)&CPUS_mbx_buf,0xffff);			
			 
				//y_pos= *CPUS_mbx_buf;
				// test=y_pos;
				os_tsk_prio_self(8);
				//os_tsk_prio(t_Read_TS,9);
				// tsk_lock();
				os_evt_set(EV_CPU_STAT_DISP,t_US);
				//	os_mut_wait(&PAUSE_GAME_mutex,WAIT_FOREVER);
					os_evt_set(EV_RECD_CPU_STAT,t_Read_TS);
			 
					TFT_Erase();
					//Delay(50);
					//os_dly_wait(0x000f);
			 
					util_t_Read_Accelerometer = ((float)count_t_Read_Accelerometer/count_total)*100;
					util_t_Read_TS = ((float)count_t_Read_TS/count_total)*100;
					util_t_Sound_Manager=((float)count_t_Sound_Manager/count_total)*100;
					util_t_US=((float)count_t_US/count_total)*100;
					util_t_Refill_Sound_Buffer=((float)count_t_Refill_Sound_Buffer/count_total)*100;
					util_t_GameStats=((float)count_t_GameStats/count_total)*100;
					util_t_CPUStats=((float)count_t_CPUStats/count_total)*100;
					util_idle=((float)count_idle/count_total)*100;
		
	 //print the cpu utilisatoin values

					sprintf(buffer, "Task name , CPU Utilsation");
		
					os_mut_wait(&LCD_mutex, WAIT_FOREVER);

					TFT_Text_PrintStr_RC(0, 0, buffer);

					sprintf(buffer,"read accel , %f",util_t_Read_Accelerometer);
					TFT_Text_PrintStr_RC(1,0,buffer);

					sprintf(buffer,"Read TS , %f",util_t_Read_TS);
					TFT_Text_PrintStr_RC(2,0,buffer);
					sprintf(buffer,"sound mngr , %f",util_t_Sound_Manager);
					TFT_Text_PrintStr_RC(3,0,buffer);
					sprintf(buffer,"updt scrn , %f",util_t_US);
					TFT_Text_PrintStr_RC(4,0,buffer);
					sprintf(buffer,"rfll snd , %f",util_t_Refill_Sound_Buffer);
					TFT_Text_PrintStr_RC(5,0,buffer);
					sprintf(buffer,"game stats , %f",util_t_GameStats);
					TFT_Text_PrintStr_RC(6,0,buffer);
					sprintf(buffer,"idle time , %f",util_idle);
					TFT_Text_PrintStr_RC(7,0,buffer);
		
					os_mut_release(&LCD_mutex);
	  
			//y_pos=0;
			
			//Delay(100);
			//while(1);
			//os_dly_wait(0x01ff);
			//for(iter= (100*10000); iter>0; iter--);
			
			//for(iter= (100*10000); iter>0; iter--);
			
			//TFT_Erase();
			//Delay(50);
			
			//tsk_unlock();
			
			
		
			os_evt_wait_and(EV_CPU_STAT_DONE,WAIT_FOREVER);
			
			TFT_Erase();
			os_dly_wait(0x000f);
			
			os_evt_set(EV_CPU_STAT_DONE,t_US);
			//os_mut_release(&PAUSE_GAME_mutex);
			
			os_tsk_prio_self(3); //restore to old prio
			//os_tsk_prio(t_Read_TS,5);
		//	}
		
	}// if pos
		 
		
			//os_tsk_prio_self(3);
			
	//os_mbx_wait(&TS_mailbox, (void*)&CPUS_mbx_buf,0x01);
	

	
	//free(CPUS_mbx_buf);
	}
}

