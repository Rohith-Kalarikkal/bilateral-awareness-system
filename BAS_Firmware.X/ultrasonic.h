/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef ULTRASONIC_H
#define	ULTRASONIC_H

#include "include.h"

float Distance,Distance2, dist=0.0, dist2=0.0;
int Time,Time2, i=0,j=0, distance, distance2;

void find_distance();
void find_distance2();
void Trigger_Pulse_10us();
void Trigger_Pulse2_10us();

#endif	/* ULTRASONIC_H */

