#include "cal.h"
#include "UART.h" 
//uint16_t receivedADCread[5];
//uint8_t  receivedcal_val;
uint16_t Observed_Standing_values[5] = {0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xCB00};
uint16_t Observed_Bent_values[5] = {0x5100, 0x0120, 0xAE00, 0xAF00, 0x6800};
uint32_t Standing_Ranges[10] = {0,0,0,0,0,0,0,0,0,0};
uint32_t Bent_Ranges[10] = {0,0,0,0,0,0,0,0,0,0};
uint32_t Middle_Ranges[10] = {0,0,0,0,0,0,0,0,0,0};
uint16_t LUT[26][5];  
uint16_t counterstand = 0;
uint16_t counterbend = 0;
int pinky_flag, ring_flag, middle_flag, index_flag, thumb_flag;
//flag = 0: standing 
//flag = 1: middle 
//flag = 2: bent
//cal_val = 0 indicates performance mode 
//cal_val = 1 indicates calibration of standing up 
//cal_val = 2 indicates calibration of bent position

void ranges(void)
{
	if (receivedcal_val > 0)
	{
		if (receivedcal_val == 1)
		{
			Standing_Ranges[pinky_LB]  += receivedADCread[pinky_index];
			Standing_Ranges[pinky_UB]  = Observed_Standing_values[pinky_index];
			Standing_Ranges[ring_LB]   += receivedADCread[ring_index];
			Standing_Ranges[ring_UB]   = Observed_Standing_values[ring_index];
			Standing_Ranges[middle_LB] += receivedADCread[middle_index];
			Standing_Ranges[middle_UB] = Observed_Standing_values[middle_index];
			Standing_Ranges[index_LB]  += receivedADCread[index_index]; //lol
			Standing_Ranges[index_UB]  = Observed_Standing_values[index_index];
			Standing_Ranges[thumb_LB]  += receivedADCread[thumb_index];
			Standing_Ranges[thumb_UB]  = Observed_Standing_values[thumb_index];
			counterstand++;
		}
		if (receivedcal_val == 2)
		{
			Bent_Ranges[pinky_LB]  += receivedADCread[pinky_index];
			Bent_Ranges[pinky_UB]  = Observed_Bent_values[pinky_index];
			Bent_Ranges[ring_LB]   += receivedADCread[ring_index];
			Bent_Ranges[ring_UB]   = Observed_Bent_values[ring_index];
			Bent_Ranges[middle_LB] += receivedADCread[middle_index];
			Bent_Ranges[middle_UB] = Observed_Bent_values[middle_index];
			Bent_Ranges[index_LB]  += receivedADCread[index_index]; //lol
			Bent_Ranges[index_UB]  = Observed_Bent_values[index_index];
			Bent_Ranges[thumb_LB]  += receivedADCread[thumb_index];
			Bent_Ranges[thumb_UB]  = Observed_Bent_values[thumb_index];
			counterbend++; 
		}
		/*
		for (int i = 0; i < 10; i++)
		{
			Standing_Ranges[i] = Standing_Ranges[i]/counterstand;
			Bent_Ranges[i] = Bent_Ranges[i]/counterbend;
		}
		counterstand = 0;
		counterbend = 0; 
		for (int i = 0; i < 10; i=i+2)
		{
			uint16_t min = Min(Standing_Ranges[i], Standing_Ranges[i+1]);
			uint16_t max = Max(Standing_Ranges[i], Standing_Ranges[i+1]);
			Standing_Ranges[i] = min; 
			Standing_Ranges[i+1] = max; 
			min = Min(Bent_Ranges[i], Bent_Ranges[i+1]);
			max = Max(Bent_Ranges[i], Bent_Ranges[i+1]);
			Bent_Ranges[i] = min; 
			Bent_Ranges[i+1] = max; 		
		}
		
		Middle_Ranges[pinky_LB]  = (Standing_Ranges[pinky_LB] + Bent_Ranges[pinky_LB])/2;
		Middle_Ranges[pinky_UB]  = (Standing_Ranges[pinky_UB] + Bent_Ranges[pinky_UB])/2;
		Middle_Ranges[ring_LB]   = (Standing_Ranges[ring_LB] + Bent_Ranges[ring_LB])/2;
		Middle_Ranges[ring_UB]   = (Standing_Ranges[ring_UB] + Bent_Ranges[ring_UB])/2;
		Middle_Ranges[middle_LB] = (Standing_Ranges[middle_LB] + Bent_Ranges[middle_LB])/2;
		Middle_Ranges[middle_UB] = (Standing_Ranges[middle_UB] + Bent_Ranges[middle_UB])/2;
		Middle_Ranges[index_LB]  = (Standing_Ranges[index_LB] + Bent_Ranges[index_LB])/2;
		Middle_Ranges[index_UB]  = (Standing_Ranges[index_UB] + Bent_Ranges[index_UB])/2;
		Middle_Ranges[thumb_LB]  = (Standing_Ranges[thumb_LB] + Bent_Ranges[thumb_LB])/2;
		Middle_Ranges[thumb_UB]  = (Standing_Ranges[thumb_UB] + Bent_Ranges[thumb_UB])/2;*/
	}
}

void AVG()
{
		for (int i = 0; i < 10; i= i+2)
		{
			if(counterstand != 0)
				Standing_Ranges[i] = Standing_Ranges[i]/counterstand;
			if(counterbend!=0)
				Bent_Ranges[i] = Bent_Ranges[i]/counterbend;
		}
		counterbend = 0;
		counterstand = 0; 
		for (int i = 0; i < 10; i=i+2)
		{
			uint16_t min = Min(Standing_Ranges[i], Standing_Ranges[i+1]);
			uint16_t max = Max(Standing_Ranges[i], Standing_Ranges[i+1]);
			Standing_Ranges[i] = min; 
			Standing_Ranges[i+1] = max; 
			min = Min(Bent_Ranges[i], Bent_Ranges[i+1]);
			max = Max(Bent_Ranges[i], Bent_Ranges[i+1]);
			Bent_Ranges[i] = min; 
			Bent_Ranges[i+1] = max; 		
		}
		
		Middle_Ranges[pinky_LB]  = (Standing_Ranges[pinky_LB] + Bent_Ranges[pinky_LB])/2;
		Middle_Ranges[pinky_UB]  = (Standing_Ranges[pinky_UB] + Bent_Ranges[pinky_UB])/2;
		Middle_Ranges[ring_LB]   = (Standing_Ranges[ring_LB] + Bent_Ranges[ring_LB])/2;
		Middle_Ranges[ring_UB]   = (Standing_Ranges[ring_UB] + Bent_Ranges[ring_UB])/2;
		Middle_Ranges[middle_LB] = (Standing_Ranges[middle_LB] + Bent_Ranges[middle_LB])/2;
		Middle_Ranges[middle_UB] = (Standing_Ranges[middle_UB] + Bent_Ranges[middle_UB])/2;
		Middle_Ranges[index_LB]  = (Standing_Ranges[index_LB] + Bent_Ranges[index_LB])/2;
		Middle_Ranges[index_UB]  = (Standing_Ranges[index_UB] + Bent_Ranges[index_UB])/2;
		Middle_Ranges[thumb_LB]  = (Standing_Ranges[thumb_LB] + Bent_Ranges[thumb_LB])/2;
		Middle_Ranges[thumb_UB]  = (Standing_Ranges[thumb_UB] + Bent_Ranges[thumb_UB])/2;
	}




void LUT_func(void){
	
	/*
	LUT[A][0][pinky_index] = Bent_Ranges[0];
	LUT[A][1][pinky_index] = Bent_Ranges[1];
	LUT[A][0][ring_index] = Bent_Ranges[1];
	LUT[A][1][ring_index] = Bent_Ranges[2];
	LUT[A][0][middle_index] = Bent_Ranges[3];
	LUT[A][1][middle_index] = Bent_Ranges[4];
	LUT[A][0][index_index] = Bent_Ranges[5];
	LUT[A][1][index_index] = Bent_Ranges[6];
	LUT[A][0][thumb_index] = Standing_Ranges[7];
	LUT[A][1][thumb_index] = Standing_Ranges[8];
	*/
	//Determing positions of each finger - what flag to set for each finger
	//pinky
	
	
	//LUT 
	
	//Letter A - SAME AS T - use fsr. 
	LUT[A][pinky_index]  = bent; 
	LUT[A][ring_index]   = bent;
	LUT[A][middle_index] = bent;
	LUT[A][index_index]  = bent;
	LUT[A][thumb_index]  = standing;
	
	//Letter B
	LUT[B][pinky_index]  = standing; 
	LUT[B][ring_index]   = standing;
	LUT[B][middle_index] = standing;
	LUT[B][index_index]  = standing;
	LUT[B][thumb_index]  = bent;
	
	//Letter C
	LUT[C][pinky_index]  = middle; 
	LUT[C][ring_index]   = middle;
	LUT[C][middle_index] = middle;
	LUT[C][index_index]  = middle;
	LUT[C][thumb_index]  = middle;
	
	//Letter D
	LUT[D][pinky_index]  = middle; 
	LUT[D][ring_index]   = middle;
	LUT[D][middle_index] = middle;
	LUT[D][index_index]  = standing;
	LUT[D][thumb_index]  = middle;
	
	//Letter E
	LUT[E][pinky_index]  = bent; 
	LUT[E][ring_index]   = bent;
	LUT[E][middle_index] = bent;
	LUT[E][index_index]  = bent;
	LUT[E][thumb_index]  = bent;
	
	//Letter F
	LUT[F][pinky_index]  = standing; 
	LUT[F][ring_index]   = standing;
	LUT[F][middle_index] = standing;
	LUT[F][index_index]  = middle;
	LUT[F][thumb_index]  = middle;
	
	//Letter G 
	//Note: thumb can be interpreted as middle or standing, 
	//in our implementation it's middle
	LUT[G][pinky_index]  = bent; 
	LUT[G][ring_index]   = bent;
	LUT[G][middle_index] = bent;
	LUT[G][index_index]  = standing;
	LUT[G][thumb_index]  = middle;
	
	//Letter H
	LUT[H][pinky_index]  = bent; 
	LUT[H][ring_index]   = bent;
	LUT[H][middle_index] = standing;
	LUT[H][index_index]  = standing;
	LUT[H][thumb_index]  = middle;
	
	//Letter I
	LUT[I][pinky_index]  = standing; 
	LUT[I][ring_index]   = bent;
	LUT[I][middle_index] = bent;
	LUT[I][index_index]  = bent;
	LUT[I][thumb_index]  = bent;
	
	//Letter J
	LUT[J][pinky_index]  = standing; 
	LUT[J][ring_index]   = bent;
	LUT[J][middle_index] = bent;
	LUT[J][index_index]  = bent;
	LUT[J][thumb_index]  = bent;
	
	//Letter K //SAME AS U
	LUT[K][pinky_index]  = bent; 
	LUT[K][ring_index]   = bent;
	LUT[K][middle_index] = standing;
	LUT[K][index_index]  = standing;
	LUT[K][thumb_index]  = standing;
	
	//Letter L
	LUT[L][pinky_index]  = bent; 
	LUT[L][ring_index]   = bent;
	LUT[L][middle_index] = bent;
	LUT[L][index_index]  = standing;
	LUT[L][thumb_index]  = standing;
	
	//Letter M //SAME AS E!!!!!!!!
	LUT[M][pinky_index]  = bent; 
	LUT[M][ring_index]   = bent;
	LUT[M][middle_index] = bent;
	LUT[M][index_index]  = bent;
	LUT[M][thumb_index]  = bent;
	
	//Letter N //SAME AS S!!!!!!!!
	LUT[N][pinky_index]  = bent; 
	LUT[N][ring_index]   = bent;
	LUT[N][middle_index] = bent;
	LUT[N][index_index]  = bent;
	LUT[N][thumb_index]  = middle;
	
	//Letter O
	LUT[O][pinky_index]  = middle; 
	LUT[O][ring_index]   = middle;
	LUT[O][middle_index] = middle;
	LUT[O][index_index]  = middle;
	LUT[O][thumb_index]  = middle;
	
	//Letter P
	LUT[P][pinky_index]  = bent; 
	LUT[P][ring_index]   = bent;
	LUT[P][middle_index] = middle;
	LUT[P][index_index]  = standing;
	LUT[P][thumb_index]  = standing;
	
	//Letter Q
	LUT[Q][pinky_index]  = bent; 
	LUT[Q][ring_index]   = bent;
	LUT[Q][middle_index] = bent;
	LUT[Q][index_index]  = standing;
	LUT[Q][thumb_index]  = standing;
	
	//Letter R
	LUT[R][pinky_index]  = bent; 
	LUT[R][ring_index]   = bent;
	LUT[R][middle_index] = standing;
	LUT[R][index_index]  = standing;
	LUT[R][thumb_index]  = bent;
	
	//Letter S
	LUT[S][pinky_index]  = bent; 
	LUT[S][ring_index]   = bent;
	LUT[S][middle_index] = bent;
	LUT[S][index_index]  = bent;
	LUT[S][thumb_index]  = middle;
	
	//Letter T
	LUT[T][pinky_index]  = bent; 
	LUT[T][ring_index]   = bent;
	LUT[T][middle_index] = bent;
	LUT[T][index_index]  = bent;
	LUT[T][thumb_index]  = standing;
	
	//Letter U //SAME AS K!!!
	LUT[U][pinky_index]  = bent; 
	LUT[U][ring_index]   = bent;
	LUT[U][middle_index] = standing;
	LUT[U][index_index]  = standing;
	LUT[U][thumb_index]  = standing;
	
	//Letter V //SAME AS K!!!!
	LUT[V][pinky_index]  = bent; 
	LUT[V][ring_index]   = bent;
	LUT[V][middle_index] = bent;
	LUT[V][index_index]  = bent;
	LUT[V][thumb_index]  = standing;
	
	//Letter W 
	LUT[W][pinky_index]  = bent; 
	LUT[W][ring_index]   = standing;
	LUT[W][middle_index] = standing;
	LUT[W][index_index]  = standing;
	LUT[W][thumb_index]  = bent;
	
	//Letter X
	LUT[X][pinky_index]  = bent; 
	LUT[X][ring_index]   = bent;
	LUT[X][middle_index] = bent;
	LUT[X][index_index]  = bent;
	LUT[X][thumb_index]  = standing;
	
	//Letter Y
	LUT[Y][pinky_index]  = standing; 
	LUT[Y][ring_index]   = bent;
	LUT[Y][middle_index] = bent;
	LUT[Y][index_index]  = bent;
	LUT[Y][thumb_index]  = standing;
	
	//Letter Z 
	LUT[Z][pinky_index]  = bent; 
	LUT[Z][ring_index]   = bent;
	LUT[Z][middle_index] = bent;
	LUT[Z][index_index]  = standing;
	LUT[Z][thumb_index]  = bent;
	
}

//this function returns 0-25, indicating A-Z which will be used as input to screen
int letter_decode(void)
{
	int bent_middle_mid_point;
	int stand_middle_mid_point;
	if(receivedADCread[pinky_index] >= Bent_Ranges[pinky_LB] && receivedADCread[pinky_index] <= Bent_Ranges[pinky_UB])
			pinky_flag = bent; 
	else if(receivedADCread[pinky_index] >= Middle_Ranges[pinky_LB] && receivedADCread[pinky_index] <= Middle_Ranges[pinky_UB])
			pinky_flag = middle; 
	else if(receivedADCread[pinky_index] >= Standing_Ranges[pinky_LB] && receivedADCread[pinky_index] <= Standing_Ranges[pinky_UB])
			pinky_flag = standing; 
	else if (receivedADCread[pinky_index]  < Bent_Ranges[pinky_LB])
			pinky_flag = bent; 
	else if (receivedADCread[pinky_index]  > Standing_Ranges[pinky_UB])
			pinky_flag = standing; 
	else
	{
		bent_middle_mid_point =  (Bent_Ranges[pinky_UB] + Middle_Ranges[pinky_LB])/2;
		stand_middle_mid_point = (Middle_Ranges[pinky_UB] + Standing_Ranges[pinky_LB])/2;
		if (receivedADCread[pinky_index]  < Standing_Ranges[pinky_LB] && receivedADCread[pinky_index]  > stand_middle_mid_point)
			pinky_flag = standing;
		if (receivedADCread[pinky_index]  > Middle_Ranges[pinky_LB] && receivedADCread[pinky_index]  < stand_middle_mid_point)
			pinky_flag = middle;
		if (receivedADCread[pinky_index]  < Middle_Ranges[pinky_LB] && receivedADCread[pinky_index]  > bent_middle_mid_point)
			pinky_flag = middle;
		if (receivedADCread[pinky_index]  > Bent_Ranges[pinky_UB] && receivedADCread[pinky_index]  < bent_middle_mid_point)
			pinky_flag = bent;
	}
	
	//ring
	if(receivedADCread[ring_index] >= Bent_Ranges[ring_LB] && receivedADCread[ring_index] <= Bent_Ranges[ring_UB])
			ring_flag = bent; 
	else if(receivedADCread[ring_index] >= Middle_Ranges[ring_LB] && receivedADCread[ring_index] <= Middle_Ranges[ring_UB])
			ring_flag = middle; 
	else if(receivedADCread[ring_index] >= Standing_Ranges[ring_LB] && receivedADCread[ring_index] <= Standing_Ranges[ring_UB])
			ring_flag = standing; 
	else if (receivedADCread[ring_index]  < Bent_Ranges[ring_LB])
			ring_flag = bent; 
	else if (receivedADCread[ring_index]  > Standing_Ranges[ring_UB])
			ring_flag = standing; 
	else
	{
		bent_middle_mid_point =  (Bent_Ranges[ring_UB] + Middle_Ranges[ring_LB])/2;
		stand_middle_mid_point = (Middle_Ranges[ring_UB] + Standing_Ranges[ring_LB])/2;
		if (receivedADCread[ring_index]  < Standing_Ranges[ring_LB] && receivedADCread[ring_index]  > stand_middle_mid_point)
			ring_flag = standing;
		if (receivedADCread[ring_index]  > Middle_Ranges[ring_LB] && receivedADCread[ring_index]  < stand_middle_mid_point)
			ring_flag = middle;
		if (receivedADCread[ring_index]  < Middle_Ranges[ring_LB] && receivedADCread[ring_index]  > bent_middle_mid_point)
			ring_flag = middle;
		if (receivedADCread[ring_index]  > Bent_Ranges[ring_UB] && receivedADCread[ring_index]  < bent_middle_mid_point)
			ring_flag = bent;
	}
	
	//middle
	if(receivedADCread[middle_index] >= Bent_Ranges[middle_LB] && receivedADCread[middle_index] <= Bent_Ranges[middle_UB])
			middle_flag = bent; 
	else if(receivedADCread[middle_index] >= Middle_Ranges[middle_LB] && receivedADCread[middle_index] <= Middle_Ranges[middle_UB])
			middle_flag = middle; 
	else if(receivedADCread[middle_index] >= Standing_Ranges[middle_LB] && receivedADCread[middle_index] <= Standing_Ranges[middle_UB])
			middle_flag = standing; 
	else if (receivedADCread[middle_index]  < Bent_Ranges[middle_LB])
			middle_flag = bent; 
	else if (receivedADCread[middle_index]  > Standing_Ranges[middle_UB])
			middle_flag = standing; 
	else
	{
		bent_middle_mid_point =  (Bent_Ranges[middle_UB] + Middle_Ranges[middle_LB])/2;
		stand_middle_mid_point = (Middle_Ranges[middle_UB] + Standing_Ranges[middle_LB])/2;
		if (receivedADCread[middle_index]  < Standing_Ranges[middle_LB] && receivedADCread[middle_index]  > stand_middle_mid_point)
			middle_flag = standing;
		if (receivedADCread[middle_index]  > Middle_Ranges[middle_LB] && receivedADCread[middle_index]  < stand_middle_mid_point)
			middle_flag = middle;
		if (receivedADCread[middle_index]  < Middle_Ranges[middle_LB] && receivedADCread[middle_index]  > bent_middle_mid_point)
			middle_flag = middle;
		if (receivedADCread[middle_index]  > Bent_Ranges[middle_UB] && receivedADCread[middle_index]  < bent_middle_mid_point)
			middle_flag = bent;
	}
	
	//index
	if(receivedADCread[index_index] >= Bent_Ranges[index_LB] && receivedADCread[index_index] <= Bent_Ranges[index_UB])
			index_flag = bent; 
	else if(receivedADCread[index_index] >= Middle_Ranges[index_LB] && receivedADCread[index_index] <= Middle_Ranges[index_UB])
			index_flag = middle; 
	else if(receivedADCread[index_index] >= Standing_Ranges[index_LB] && receivedADCread[index_index] <= Standing_Ranges[index_UB])
			index_flag = standing; 
	else if (receivedADCread[index_index]  < Bent_Ranges[index_LB])
			index_flag = bent; 
	else if (receivedADCread[index_index]  > Standing_Ranges[index_UB])
			index_flag = standing; 
	else
	{
		bent_middle_mid_point =  (Bent_Ranges[index_UB] + Middle_Ranges[index_LB])/2;
		stand_middle_mid_point = (Middle_Ranges[index_UB] + Standing_Ranges[index_LB])/2;
		if (receivedADCread[index_index]  < Standing_Ranges[index_LB] && receivedADCread[index_index]  > stand_middle_mid_point)
			index_flag = standing;
		if (receivedADCread[index_index]  > Middle_Ranges[index_LB] && receivedADCread[index_index]  < stand_middle_mid_point)
			index_flag = middle;
		if (receivedADCread[index_index]  < Middle_Ranges[index_LB] && receivedADCread[index_index]  > bent_middle_mid_point)
			index_flag = middle;
		if (receivedADCread[index_index]  > Bent_Ranges[index_UB] && receivedADCread[index_index]  < bent_middle_mid_point)
			index_flag = bent;
	}
	
	//thumb
	if(receivedADCread[thumb_index] >= Bent_Ranges[thumb_LB] && receivedADCread[thumb_index] <= Bent_Ranges[thumb_UB])
			thumb_flag = bent; 
	else if(receivedADCread[thumb_index] >= Middle_Ranges[thumb_LB] && receivedADCread[thumb_index] <= Middle_Ranges[thumb_UB])
			thumb_flag = middle; 
	else if(receivedADCread[thumb_index] >= Standing_Ranges[thumb_LB] && receivedADCread[thumb_index] <= Standing_Ranges[thumb_UB])
			thumb_flag = standing; 
	else if (receivedADCread[thumb_index]  < Bent_Ranges[thumb_LB])
			thumb_flag = bent; 
	else if (receivedADCread[thumb_index]  > Standing_Ranges[thumb_UB])
			thumb_flag = standing; 
	else
	{
		bent_middle_mid_point =  (Bent_Ranges[thumb_UB] + Middle_Ranges[thumb_LB])/2;
		stand_middle_mid_point = (Middle_Ranges[thumb_UB] + Standing_Ranges[thumb_LB])/2;
		if (receivedADCread[thumb_index]  < Standing_Ranges[thumb_LB] && receivedADCread[thumb_index]  > stand_middle_mid_point)
			thumb_flag = standing;
		if (receivedADCread[thumb_index]  > Middle_Ranges[thumb_LB] && receivedADCread[thumb_index]  < stand_middle_mid_point)
			thumb_flag = middle;
		if (receivedADCread[thumb_index]  < Middle_Ranges[thumb_LB] && receivedADCread[thumb_index]  > bent_middle_mid_point)
			thumb_flag = middle;
		if (receivedADCread[thumb_index]  > Bent_Ranges[thumb_UB] && receivedADCread[thumb_index]  < bent_middle_mid_point)
			thumb_flag = bent;
	}
	

	//Letter A - 0
	if ((pinky_flag == LUT[A][pinky_index]) && (ring_flag == LUT[A][ring_index]) && (middle_flag == LUT[A][middle_index]) && (index_flag == LUT[A][index_index]) && (thumb_flag == LUT[A][thumb_index]))
	{
		return A; 
	}
	
	//Letter B - 1
	if ((pinky_flag == LUT[B][pinky_index]) && (ring_flag == LUT[B][ring_index]) && (middle_flag == LUT[B][middle_index]) && (index_flag == LUT[B][index_index]) && (thumb_flag == LUT[B][thumb_index]))
	{
		return B; 
	}
	
	//Letter C - 2
	if ((pinky_flag == LUT[C][pinky_index]) && (ring_flag == LUT[C][ring_index]) && (middle_flag == LUT[C][middle_index]) && (index_flag == LUT[C][index_index]) && (thumb_flag == LUT[C][thumb_index]))
	{
		return C; 
	}
	
	//Letter D - 3
	if ((pinky_flag == LUT[D][pinky_index]) && (ring_flag == LUT[D][ring_index]) && (middle_flag == LUT[D][middle_index]) && (index_flag == LUT[D][index_index]) && (thumb_flag == LUT[D][thumb_index]))
	{
		return D; 
	}
	
	//Letter E - 4
	if ((pinky_flag == LUT[E][pinky_index]) && (ring_flag == LUT[E][ring_index]) && (middle_flag == LUT[E][middle_index]) && (index_flag == LUT[E][index_index]) && (thumb_flag == LUT[E][thumb_index]))
	{
		return E; 
	}
	
	//Letter F - 5
	if ((pinky_flag == LUT[F][pinky_index]) && (ring_flag == LUT[F][ring_index]) && (middle_flag == LUT[F][middle_index]) && (index_flag == LUT[F][index_index]) && (thumb_flag == LUT[F][thumb_index]))
	{
		return F; 
	}
	
	//Letter G - 6
	if ((pinky_flag == LUT[G][pinky_index]) && (ring_flag == LUT[G][ring_index]) && (middle_flag == LUT[G][middle_index]) && (index_flag == LUT[G][index_index]) && (thumb_flag == LUT[G][thumb_index]))
	{
		return G; 
	}
	
	//Letter H - 7 
	if ((pinky_flag == LUT[H][pinky_index]) && (ring_flag == LUT[H][ring_index]) && (middle_flag == LUT[H][middle_index]) && (index_flag == LUT[H][index_index]) && (thumb_flag == LUT[H][thumb_index]))
	{
		return H; 
	}
	
	//Letter I - 8 
	if ((pinky_flag == LUT[I][pinky_index]) && (ring_flag == LUT[I][ring_index]) && (middle_flag == LUT[I][middle_index]) && (index_flag == LUT[I][index_index]) && (thumb_flag == LUT[I][thumb_index]))
	{
		return I; 
	}
	
	//Letter J - 9 
	if ((pinky_flag == LUT[J][pinky_index]) && (ring_flag == LUT[J][ring_index]) && (middle_flag == LUT[J][middle_index]) && (index_flag == LUT[J][index_index]) && (thumb_flag == LUT[J][thumb_index]))
	{
		return J; 
	}
	
	//Letter K - 10
	if ((pinky_flag == LUT[K][pinky_index]) && (ring_flag == LUT[K][ring_index]) && (middle_flag == LUT[K][middle_index]) && (index_flag == LUT[K][index_index]) && (thumb_flag == LUT[K][thumb_index]))
	{
		return K; 
	}
	
	//Letter L - 11
	if ((pinky_flag == LUT[L][pinky_index]) && (ring_flag == LUT[L][ring_index]) && (middle_flag == LUT[L][middle_index]) && (index_flag == LUT[L][index_index]) && (thumb_flag == LUT[L][thumb_index]))
	{
		return L; 
	}
	
	//Letter M - 12
	if ((pinky_flag == LUT[M][pinky_index]) && (ring_flag == LUT[M][ring_index]) && (middle_flag == LUT[M][middle_index]) && (index_flag == LUT[M][index_index]) && (thumb_flag == LUT[M][thumb_index]))
	{
		return M; 
	}
	
	//Letter N - 13
	if ((pinky_flag == LUT[N][pinky_index]) && (ring_flag == LUT[N][ring_index]) && (middle_flag == LUT[N][middle_index]) && (index_flag == LUT[N][index_index]) && (thumb_flag == LUT[N][thumb_index]))
	{
		return N; 
	}
	
	//Letter O - 14
	if ((pinky_flag == LUT[O][pinky_index]) && (ring_flag == LUT[O][ring_index]) && (middle_flag == LUT[O][middle_index]) && (index_flag == LUT[O][index_index]) && (thumb_flag == LUT[O][thumb_index]))
	{
		return O; 
	}
	
	//Letter P - 15
	if ((pinky_flag == LUT[P][pinky_index]) && (ring_flag == LUT[P][ring_index]) && (middle_flag == LUT[P][middle_index]) && (index_flag == LUT[P][index_index]) && (thumb_flag == LUT[P][thumb_index]))
	{
		return P; 
	}
	
	//Letter Q - 16
	if ((pinky_flag == LUT[Q][pinky_index]) && (ring_flag == LUT[Q][ring_index]) && (middle_flag == LUT[Q][middle_index]) && (index_flag == LUT[Q][index_index]) && (thumb_flag == LUT[Q][thumb_index]))
	{
		return Q; 
	}
	
	//Letter R - 17
	if ((pinky_flag == LUT[R][pinky_index]) && (ring_flag == LUT[R][ring_index]) && (middle_flag == LUT[R][middle_index]) && (index_flag == LUT[R][index_index]) && (thumb_flag == LUT[R][thumb_index]))
	{
		return R; 
	}
	
	//Letter S - 18
	if ((pinky_flag == LUT[S][pinky_index]) && (ring_flag == LUT[S][ring_index]) && (middle_flag == LUT[S][middle_index]) && (index_flag == LUT[S][index_index]) && (thumb_flag == LUT[S][thumb_index]))
	{
		return S; 
	}
	
	//Letter T - 19
	if ((pinky_flag == LUT[T][pinky_index]) && (ring_flag == LUT[T][ring_index]) && (middle_flag == LUT[T][middle_index]) && (index_flag == LUT[T][index_index]) && (thumb_flag == LUT[T][thumb_index]))
	{
		return T; 
	}
	
	//Letter U - 20 
	if ((pinky_flag == LUT[U][pinky_index]) && (ring_flag == LUT[U][ring_index]) && (middle_flag == LUT[U][middle_index]) && (index_flag == LUT[U][index_index]) && (thumb_flag == LUT[U][thumb_index]))
	{
		return U; 
	}
	
	//Letter V - 21
	if ((pinky_flag == LUT[V][pinky_index]) && (ring_flag == LUT[V][ring_index]) && (middle_flag == LUT[V][middle_index]) && (index_flag == LUT[V][index_index]) && (thumb_flag == LUT[V][thumb_index]))
	{
		return V; 
	}
	
	//Letter W - 22
	if ((pinky_flag == LUT[W][pinky_index]) && (ring_flag == LUT[W][ring_index]) && (middle_flag == LUT[W][middle_index]) && (index_flag == LUT[W][index_index]) && (thumb_flag == LUT[W][thumb_index]))
	{
		return W; 
	}
	
	//Letter X - 23
	if ((pinky_flag == LUT[X][pinky_index]) && (ring_flag == LUT[X][ring_index]) && (middle_flag == LUT[X][middle_index]) && (index_flag == LUT[X][index_index]) && (thumb_flag == LUT[X][thumb_index]))
	{
		return X; 
	}
	
	//Letter Y - 24
	if ((pinky_flag == LUT[Y][pinky_index]) && (ring_flag == LUT[Y][ring_index]) && (middle_flag == LUT[Y][middle_index]) && (index_flag == LUT[Y][index_index]) && (thumb_flag == LUT[Y][thumb_index]))
	{
		return Y; 
	}
	
	//Letter Z - 25
	if ((pinky_flag == LUT[Z][pinky_index]) && (ring_flag == LUT[Z][ring_index]) && (middle_flag == LUT[Z][middle_index]) && (index_flag == LUT[Z][index_index]) && (thumb_flag == LUT[Z][thumb_index]))
	{
		return Z; 
	}

return 26; 	
}

uint32_t Max(uint32_t num1, uint32_t num2)
{
	if (num1 >= num2)
		return num1;
	else
		return num2; 
}

uint32_t Min(uint32_t num1, uint32_t num2)
{
	if (num1 <= num2)
		return num1;
	else
		return num2; 
}
