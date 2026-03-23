/*
 * File:   main.c
 * Author: R K
 *
 * Created on 15 March, 2021, 3:12 PM
 */

#include "include.h"

void main(void) {
    float Total_distance[10];
    float Total_distance2[10];
    TRISB = 0xFF;           /* define PORTB as Input port*/
    TRISD = 0x00;              /* define PORTD as Output port*/
    TRISC = 0x00;
    INTCON2bits.RBPU=0;     /*enable PORTB Pull-ups */
    LCD_Init();
    Trigger_Pulse = 0;
    Trigger_Pulse2 = 0;
    T1CON = 0x80;           /* enable 16-bit TMR1 Register,No pre-scale,
                             * use internal clock,Timer OFF */
    TMR1IF = 0;             /* make Timer1 Overflow Flag to '0' */
    TMR1=0;                 /* load Timer1 with 0 */
    LCD_String_xy(1,1," Distance:");
    while(1)
    { 
        find_distance();
        if(distance > 0) {
            sprintf(Total_distance,"%d",distance);
            LCD_String_xy(2,1,Total_distance);
            LCD_String(" cm   ");
//                MSdelay(10);
        }
        else {
            LCD_String_xy(2,1,"0");
            LCD_String(" cm   ");
        }
        
        find_distance2();
        if(distance2 > 0) {
            sprintf(Total_distance2,"%d",distance2);
            LCD_String_xy(2,10,Total_distance2);
            LCD_String(" cm   ");
//                MSdelay(10);
        }
        else {
            LCD_String_xy(2,10,"0");
            LCD_String(" cm   ");
        }
        
        if(distance < 100 && distance2 < 100) {
            PORTC = 0xF0;
        }
        else if (distance > 100 && distance2 <= 100) {
            PORTC = 0x30;
        }
        else if (distance <= 100 && distance2 > 100) {
            PORTC = 0xC0;
        }
        else {
            PORTC = 0x00;
        }
    }   
}

