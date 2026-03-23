/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
  
#ifndef INCLUDE_H
#define	INCLUDE_H

#define _XTAL_FREQ 8000000

#include <xc.h>
#include <stdio.h>
#include <pic18f4580.h>
#include "config.h"
#include "LCD_4bit.h"
#include "ultrasonic.h"

#define Trigger_Pulse LATD0     /* PORTD.0 pin is connected to Trig pin of HC-SR04 */
#define Trigger_Pulse2 LATD1     /* PORTD.1 pin is connected to Trig pin of HC-SR04 2*/

#endif	/* INCLUDE_H */

