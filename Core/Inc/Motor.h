#ifndef __Motor_H
#define __Motor_H



void Motor_Init(void);


void Advance_SlowSpeed(void);

void Advance_MediumSpeed(void);

void Advance_HighSpeed(void);

void Advance_FullSpeed(void);



void Recoil_SlowSpeed(void);



void Left_differential(void);

void Left_InPlace(void);

void Left_Turn(void);


void Right_differential(void);

void Right_InPlace(void);

void Right_Turn(void);



void Stop(void);



#endif


