#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#define ULT_PORT_TX_0 PTE11
#define ULT_PORT_RX_0 PTE12
#define ULT_ID0	 12

#define ULT_TIMER_0	PIT3

#define LASER_PORT1	PTE9
#define LASER_PORT2	PTC14


#define HOARE_PORT	PTE8
#define HOARE_ID	8



void Distance_init();
void Distance_send();
void Distance_measure();
void Obstacle_avoidance();
void PORTE_Handler();
#endif