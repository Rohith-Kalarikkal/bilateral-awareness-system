#include "ultrasonic.h"

void find_distance() {
    dist = 0;
    for(i=0; i<10; i++) {
        Trigger_Pulse_10us();               /* transmit at least 10 us pulse to HC-SR04 */
        while(PORTBbits.RB0==0);            /* wait for rising edge at Echo pin of HC-SR04 */
        TMR1=0;                             /* Load Timer1 register with 0 */
        TMR1ON=1;                           /* turn ON Timer1*/
        while(PORTBbits.RB0==1 && !TMR1IF); /* wait for falling edge at Echo pin of HC-SR04*/
        Time = TMR1;                        /* copy Time when echo is received from an object */
        TMR1ON=0;                           /* turn OFF Timer1 */
        Distance = ((float)Time/140.00);           /* distance = (velocity x Time)/2 */ 
        dist = dist + Distance;
    }
    distance = dist/10 *10;
}

void Trigger_Pulse_10us()
{
    Trigger_Pulse = 1;
    __delay_us(10);
    Trigger_Pulse = 0;
}

void find_distance2() {
    dist2 = 0;
    for(j=0; j<10; j++) {
        Trigger_Pulse2_10us();               /* transmit at least 10 us pulse to HC-SR04 */
        while(PORTBbits.RB1==0);            /* wait for rising edge at Echo pin of HC-SR04 */
        TMR1=0;                             /* Load Timer1 register with 0 */
        TMR1ON=1;                           /* turn ON Timer1*/
        while(PORTBbits.RB1==1 && !TMR1IF); /* wait for falling edge at Echo pin of HC-SR04*/
        Time2 = TMR1;                        /* copy Time when echo is received from an object */
        TMR1ON=0;                           /* turn OFF Timer1 */
        Distance2 = ((float)Time2)/140.00;           /* distance = (velocity x Time)/2 */ 
        dist2 = dist2 + Distance2;
    }
    distance2 = dist2/10 *10;
}

void Trigger_Pulse2_10us()
{
    Trigger_Pulse2 = 1;
    __delay_us(10);
    Trigger_Pulse2 = 0;
}