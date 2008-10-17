#ifndef F_CPU
/* In neueren Version der WinAVR/Mfile Makefile-Vorlage kann
   F_CPU im Makefile definiert werden, eine nochmalige Definition
   hier wuerde zu einer Compilerwarnung fuehren. Daher "Schutz" durch
   #ifndef/#endif 
 
   Dieser "Schutz" kann zu Debugsessions führen, wenn AVRStudio 
   verwendet wird und dort eine andere, nicht zur Hardware passende 
   Taktrate eingestellt ist: Dann wird die folgende Definition 
   nicht verwendet, sondern stattdessen der Defaultwert (8 MHz?) 
   von AVRStudio - daher Ausgabe einer Warnung falls F_CPU
   noch nicht definiert: */
#warning "F_CPU war noch nicht definiert, wird nun nachgeholt mit 8000000"
#define F_CPU 8000000UL    // Systemtakt in Hz - Definition als unsigned long beachten >> Ohne ergeben Fehler in der Berechnung
#endif
 
#define BAUD 19200UL          // Baudrate
 
// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
 
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif

#include <inttypes.h>
#include <avr/io.h>

int uart_init(void)
{
    UCSRB |= (1<<TXEN)|(1<< RXEN)|(1<<RXCIE);                // UART TX einschalten
    UCSRC |= (1<<URSEL)|(1 << UCSZ1)|(1 << UCSZ0);    // Asynchron 8N1
    
    
    UBRRH = UBRR_VAL >> 8;
    UBRRL = UBRR_VAL & 0xFF;
}

int uart_putc(unsigned char c)
{
    while (!(UCSRA & (1<<UDRE)))  /* warten bis Senden moeglich */
    {
    }                             
 
    UDR = c;                      /* sende Zeichen */
    return 0;
}
 
 
/* puts ist unabhaengig vom Controllertyp */
void uart_puts (char *s)
{
    while (*s)
    {   /* so lange *s != '\0' also ungleich dem "String-Endezeichen" */
        uart_putc(*s);
        s++;
    }
}
uint8_t uart_getc(void)
{
    while (!(UCSRA & (1<<RXC)))   // warten bis Zeichen verfuegbar
        ;
    return UDR;                   // Zeichen aus UDR an Aufrufer zurueckgeben
}
 
void uart_gets( char* Buffer, uint8_t MaxLen )
{
  uint8_t NextChar;
  uint8_t StringLen = 0;
 
  NextChar = uart_getc();         // Warte auf und empfange das nächste Zeichen
 
                                  // Sammle solange Zeichen, bis:
                                  // * entweder das String Ende Zeichen kam
                                  // * oder das aufnehmende Array voll ist
  while( NextChar != '\n' && NextChar != '\t' && NextChar != '\r') {
    *Buffer++ = NextChar;
    StringLen++;
    NextChar = uart_getc();
  }
 
                                  // Noch ein '\0' anhängen um einen Standard
                                  // C-String daraus zu machen
  *Buffer = '\0';
}
