#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define UART_BAUDRATE 19200
#ifndef F_CPU
#warning "F_CPU war noch nicht definiert, wird nun nachgeholt mit 8000000"
#define F_CPU 8000000UL    // Systemtakt in Hz - Definition als unsigned long beachten >> Ohne ergeben Fehler in der Berechnung
#endif
char line[255];

int main(void)
{
    lcd_init();
    uart_init();
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    
    
    while(1)
    {
        sleep_mode();
    }
 
    return 0;
}

ISR(USART_RXC_vect) 
{
    char posys[2];
    char inhalt[21];
    int posx, posy;
    char *pointer = line;
    uart_gets(line,20);
    memcpy(posys,(pointer), 1);
    memcpy(inhalt,(pointer) + 1, 20);
    inhalt[20] = '\0';
    posy = atoi(posys);
    //lcd_clear();
    lcd_gotoxy(0,posy);
    lcd_string(inhalt);
}
