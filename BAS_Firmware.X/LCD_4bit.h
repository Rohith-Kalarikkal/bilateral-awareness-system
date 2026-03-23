/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef LCD_4BIT_H
#define	LCD_4BIT_H

#include "include.h"

/*********************Definition of Ports********************************/

#define RS LATD2                    /*PIN 0 of PORTB is assigned for register select Pin of LCD*/
#define EN LATD3                    /*PIN 1 of PORTB is assigned for enable Pin of LCD */
#define ldata LATD                  /*PORTB(PB4-PB7) is assigned for LCD Data Output*/ 
#define LCD_Port TRISD

/*********************Proto-Type Declaration*****************************/

void MSdelay(unsigned int );        /*Generate delay in ms*/
void LCD_Init();                    /*Initialize LCD*/
void LCD_Command(unsigned char );   /*Send command to LCD*/
void LCD_Char(unsigned char x);     /*Send data to LCD*/
void LCD_String(const char *);      /*Display data string on LCD*/
void LCD_String_xy(char, char , const char *);
void LCD_Clear();                   /*Clear LCD Screen*/

#endif	/* LCD_4BIT_H */

