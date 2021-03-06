
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <inttypes.h>
//#include "peschke.h"


#include <C:\Users\andreas.knaup\Desktop\Projekte\GccApplication3\GccApplication3\lcd.h>
#include <C:\Users\andreas.knaup\Desktop\Projekte\GccApplication3\GccApplication3\lcd.c>
#define USART_BAUDRATE 2400
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

uint8_t onechar[11];
int uart_putc(unsigned char c)
{
	while (!(UCSRA & (1<<UDRE)))  /* warten bis Senden moeglich */
	{
	}
	
	UDR = c;                      /* sende Zeichen */
	return 0;
}


void uart_init(void)
{
	int UBRR_VAL=416;
	UBRRH = UBRR_VAL >> 8;
	UBRRL = UBRR_VAL & 0xFF;
	
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);  // Asynchron 8N1
	UCSRB |= (1<<RXEN);    
	UCSRB |=(1<<TXEN);                    // UART RX einschalten
}

uint8_t USART0ReceiveByte()
{
	// Wait for byte to be received
	while(!(UCSRA&(1<<RXC))){};
	// Return received data
	return UDR;
}
///////
///////
//////
void uart_gets( char* Buffer, uint8_t MaxLen )
{
	uint8_t NextChar;
	uint8_t StringLen = 0;
	int xi=0;
	NextChar =  USART0ReceiveByte();         // Warte auf und empfange das n�chste Zeichen
	onechar[0]=NextChar;
	// Sammle solange Zeichen, bis:
	// * entweder das String Ende Zeichen kam
	// * oder das aufnehmende Array voll ist
	while( NextChar != '\r' && StringLen < MaxLen - 1 ) {
		xi++;
		*Buffer++ = NextChar;
		StringLen++;
		NextChar = USART0ReceiveByte();
		onechar[xi]=NextChar;
	}
	onechar[0]=NextChar;
	// Noch ein '\0' anh�ngen um einen Standard
	// C-String daraus zu machen
	*Buffer = '\0';
}
/////
/////

///


int main (void) {
	uint8_t TempData;
	 int lcd_pos_x=0; 
	 int lcd_pos_y=0;
	 int merker=0;
	 int i=0;

	  uart_init();
	  /* Initialisiere Display, Cursor aus */
	  lcd_init(LCD_DISP_ON);

	  /* loesche das LCD Display und Cursor auf 1 Zeile, 1 Spalte */
	
	  lcd_clrscr();
	  
	DDRC =0xFF; //Port C als Ausgang
	   while (1)
	   {
		   
		   
		   
		
		
		char MUX_Kanal_1[11];
		char MUX_Kanal_2[11];
		char Line[10];      // String mit maximal 11 zeichen
		
	
			//PORTC &=~(1<<PC0); //Pin f�r MUX Kanalauswahl auf High
		
			
			
			uart_gets( MUX_Kanal_1, sizeof( MUX_Kanal_1 ) );
			
			if(onechar[2]==0b00000001)		{
				lcd_gotoxy(0,0);
				lcd_puts("E1:");
				lcd_puts(" ");
				lcd_putc(onechar[3]);
				lcd_puts(".");
				lcd_putc(onechar[4]);
				lcd_putc(onechar[5]);
				lcd_puts("V");
				lcd_puts(" ");
				
				lcd_putc(onechar[3]);
			
				lcd_putc(onechar[4]);
				lcd_putc(onechar[5]);
				lcd_putc(onechar[6]);
				lcd_putc(onechar[7]);
			}
			
				if(onechar[2]==0b00000010)		{
					lcd_gotoxy(0,1);
					lcd_puts("E2:");
							lcd_puts(" ");
							lcd_putc(onechar[3]);
							lcd_puts(".");
							lcd_putc(onechar[4]);
							lcd_putc(onechar[5]);
							lcd_puts("V");
							lcd_puts(" ");
							
							lcd_putc(onechar[3]);
							
							lcd_putc(onechar[4]);
							lcd_putc(onechar[5]);
							lcd_putc(onechar[6]);
							lcd_putc(onechar[7]);
					
				}
			
		//	lcd_puts("E1: ");
		//	lcd_puts(MUX_Kanal_1);
			
			
		//	lcd_gotoxy(0,1);
			//lcd_puts(onechar[2]);
				//lcd_gotoxy(0,1);
			//for(i=0;i<11;i++){
				uart_putc(onechar[2]);
		//	lcd_putc(onechar[i]);
			//}
			/*

			PORTC |= (1<<PC0);
			
			merker=0;
			
			uart_gets( MUX_Kanal_2, sizeof( MUX_Kanal_2 ) );
			lcd_gotoxy(0,1);
			lcd_puts("E2: ");
			lcd_puts(MUX_Kanal_2);
			
			*/
			
			
	//	_delay_ms(800);
	//	}
	if(merker==0){
		PORTC |= (1<<PC0);
		merker=1;
	}
	else
	{
		PORTC &=~(1<<PC0);
		merker=0;
	}
		
		
		
}
	return 0;
}