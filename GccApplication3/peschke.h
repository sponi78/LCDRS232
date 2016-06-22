/*
Include-Datei peschke.h

Zum Einbinden von häufig benutzten Header-Dateien
und selbst erstellten Funktionen. 

von Adrian Peschke
10.11.2014
*/



#ifndef _INCLUDE_PESCHKE_H
#define _INCLUDE_PESCHKE_H 1


#define F_CPU 16000000UL
#include <util/delay.h>

#include <math.h>
#include <avr/interrupt.h>


#define BELL 0x07

/*	Die Funktion lcd_put_number() zeigt eine übergebene Zahl auf dem LCD-Display an.
	Mit der Funktion können sowohl positive wie negative ganze Zahlen angezeigt werden.
	Anwendungsbeispiel:    lcd_put_number(123); --> Führt zur Anzeige "123" auf dem LCD-Display 
*/
void lcd_put_number(long int zahl);		//Prototyp der Funktion lcd_put_number()


//Konstanten Deklarieren für die Anschlüsse des Keypads
#define KEYDDR DDRB				//Definieren des benötigten DDR
#define KEYPORT PORTB			//Definieren des benötigten PORT
#define KEYPIN PINB			//Definieren des benötigten PIN
#define	S1_PIN 5				//Definieren des Bits für Spalte 1
#define	S2_PIN 3				//Definieren des Bits für Spalte 2
#define S3_PIN 1				//Definieren des Bits für Spalte 3
#define	R1_PIN 0				//Definieren des Bits für 1. Reihe
#define R2_PIN 2				//Definieren des Bits für 2. Reihe
#define R3_PIN 4				//Definieren des Bits für 3. Reihe
#define R4_PIN 6				//Definieren des Bits für 4. Reihe

/*Funktion zum Initialisieren des Keypads mit den eingestellten Ports und Bits 
	Beispiel:    keypad_init();
*/
void keypad_init(void);

/*Funktion zum lesen eines Zeichens von der Matrix-Tastatur
	Wenn Tastatur nicht gedrückt ist Ausgabewert gleich 0
	Abhängig von der gedrückten Taste gibt die Funktion ein Zeichen aus
	Mögliche Zeichen: '0','1','2','3','4','5','6','7','8','9','#','*'
	Beispiel:		x = keypad_getc();
*/
char keypad_getc(void);

//Initialisiert den AD-Wandler des ATmega32 mit einer Wandlungsfrequenz von 125kHz
//Beispiel: ADC_init();
void ADC_init(void);

//lesen eines AD-Wertes von einem vorher gewählten Pin
//Beispiel: adc_wert = ADC_read(2); um von PINA2 den gewandelten Wert in adc_wert zu Speichern
int ADC_read(char MUX);






void lcd_put_number(long int zahl)
{
	char ziffer[20];
	char i =0;
	
	//Programmteil zum Umwandeln der Ganzzahl in Zeichen für LCD
	if(zahl < 0)					//wenn Zahl negativ
	{
		lcd_putc('-');				//Schreibe ein Minus zum LCD
		zahl = zahl * (-1);			//Rechne Zahl mal -1
	}

	if(zahl == 0)					//wenn Zahl gleich 0
	lcd_putc('0');					//schreibe 0 zum Display

	while(zahl > 0)					//Durchlaufe die Schleife solang die Zahl größer 0 ist
	{
		ziffer[i]=zahl%10;			//Rechne Zahl Modulo 10 und speicher das ergebnis in Ziffer an der Stell i
		zahl = zahl/10;				//Zahl durch 10 dividieren
		i++;						//i um 1 hochzählen
	}

	while(i>0)						//Durchlaufe Schleife solange i größer 0
	{
		i--;						//i um 1 herunterzählen
		lcd_putc(ziffer[i]+0x30);	//Sende Zahl in Ziffer an stelle i an LCD
	}
}//Ende des Programmteils für Wertumwandlung


void keypad_init(void)
{
KEYDDR = KEYDDR|(1<<S1_PIN)|(1<<S2_PIN)|(1<<S3_PIN);
KEYPORT = KEYPORT & ~((1<<R1_PIN)|(1<<R2_PIN)|(1<<R3_PIN)|(1<<R4_PIN)|(1<<S1_PIN)|(1<<S2_PIN)|(1<<S3_PIN));
}

		
char keypad_getc(void)
{
char zeichen = 0,spalte = 0;	//Variablen Deklarieren und initialisieren
while(spalte<3)					//Schleife soll solange durchlaufen werden bis Variable spalte kleiner 3 ist
	{
	KEYPORT = KEYPORT & ~((1<<R1_PIN)|(1<<R2_PIN)|(1<<R3_PIN)|(1<<R4_PIN)|(1<<S1_PIN)|(1<<S2_PIN)|(1<<S3_PIN));	//Keypad-Port auf 0 setzen
	switch(spalte)
		{
		case 0:	{KEYPORT = KEYPORT | (1<<S1_PIN); break;}		//Wenn Variable spalte gleich 0 ist, soll die Spalte 1 aktiviert
		case 1:	{KEYPORT = KEYPORT | (1<<S2_PIN); break;}		//Wenn Variable spalte gleich 1 ist, soll die Spalte 2 aktiviert	
		case 2:	{KEYPORT = KEYPORT | (1<<S3_PIN); break;}		//Wenn Variable spalte gleich 2 ist, soll die Spalte 3 aktiviert
		}
	_delay_us(100);			//Warten bis Controller-Ausgänge Ihren zustand angenommen haben 	
	switch(KEYPIN & ((1<<R1_PIN)|(1<<R2_PIN)|(1<<R3_PIN)|(1<<R4_PIN)|(1<<S1_PIN)|(1<<S2_PIN)|(1<<S3_PIN)))	//Abhängig von meinen gewälten Spalten und aktiven Eingängen,
		{																									// soll ein bestimmtes Zeichen in die Variable zeichen gespeichert werden
		case (1<<R1_PIN)|(1<<S1_PIN):	{zeichen = '1'; break;}
		case (1<<R2_PIN)|(1<<S1_PIN):	{zeichen = '4'; break;}
		case (1<<R3_PIN)|(1<<S1_PIN):	{zeichen = '7'; break;}
		case (1<<R4_PIN)|(1<<S1_PIN):	{zeichen = '*'; break;}
		case (1<<R1_PIN)|(1<<S2_PIN):	{zeichen = '2'; break;}
		case (1<<R2_PIN)|(1<<S2_PIN):	{zeichen = '5'; break;}
		case (1<<R3_PIN)|(1<<S2_PIN):	{zeichen = '8'; break;}
		case (1<<R4_PIN)|(1<<S2_PIN):	{zeichen = '0'; break;}
		case (1<<R1_PIN)|(1<<S3_PIN):	{zeichen = '3'; break;}
		case (1<<R2_PIN)|(1<<S3_PIN):	{zeichen = '6'; break;}
		case (1<<R3_PIN)|(1<<S3_PIN):	{zeichen = '9'; break;}
		case (1<<R4_PIN)|(1<<S3_PIN):	{zeichen = '#'; break;}
		}		
	++spalte;						//Variable spalte um 1 hochzählen
	}
KEYPORT = KEYPORT & ~((1<<R1_PIN)|(1<<R2_PIN)|(1<<R3_PIN)|(1<<R4_PIN)|(1<<S1_PIN)|(1<<S2_PIN)|(1<<S3_PIN));      //Keypad-Port auf 0 setzen
return zeichen;
}

//Initialisiert den AD-Wandler des ATmega32 mit einer Wandlungsfrequenz von 125kHz
void ADC_init(void)
{
	ADCSRA= 0;
	ADCSRA= ADCSRA | 0x87;
	ADMUX = 0;
	ADMUX = ADMUX | 0x40;
}

//lesen eines AD-Wertes von einem vorher gewählten Pin
int ADC_read(char MUX)
{
	ADMUX = ADMUX & 0xe0;
	ADMUX = ADMUX | (MUX & 0x1f);
	ADCSRA = ADCSRA | 0x40;
	while(ADCSRA & 0x40);
	return ADCW;
}

void USART_init(long int Baudrate, short int Daten_b, short int Stop_b, short int Parity_b, short int d_speed)
{
long int Baud_reload;
unsigned short int controlC = 0;
UCSRA = 0x00;
UCSRB = 0x00;
UCSRC = 0x00;
if(d_speed == 1)
	{
	UCSRA = UCSRA | (1<<U2X);
	}
else
	UCSRA = UCSRA & ~(1<<U2X);

UCSRB = UCSRB | ((1<<RXEN) | (1<<TXEN));

if(Daten_b==8)
	{
	UCSRB = UCSRB &	~(1<<UCSZ2);
	controlC = controlC | ((1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0));
	}

else if(Daten_b == 6)
	{
	UCSRB = UCSRB &	~(1<<UCSZ2);
	controlC = controlC | ((1<<URSEL) | (1<<UCSZ0));
	}

else if(Daten_b == 7)
	{
	UCSRB = UCSRB &	~(1<<UCSZ2);
	controlC = controlC  | ((1<<URSEL) | (1<<UCSZ1));
	}

else if(Daten_b == 9)
	{
	UCSRB = UCSRB |	(1<<UCSZ2);
	controlC = controlC | ((1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0));
	}

if (Stop_b == 2)
	{
	controlC = controlC | ((1<<URSEL) | (1<<USBS));
	}

if (Parity_b == 1)
	{
	controlC = controlC | 0xa0;
	}

else if (Parity_b == 2)
	{
	controlC = controlC | 0xb0;
	}
	
if((UCSRA & (1<<U2X))==(1<<U2X))
	Baud_reload = (F_CPU / (8*Baudrate))-1;
else
	Baud_reload = (F_CPU / (16*Baudrate))-1;

UBRRL = Baud_reload & 0x00ff;
UBRRH = (Baud_reload & 0x0f00)>>8; 
UCSRC = controlC;
}

void USART_putc(char byte)
{
	
while(bit_is_clear(UCSRA,UDRE));
UDR = byte;	
while(!(UCSRA&(1<<TXC))); 
UCSRA= UCSRA | (1<<TXC);
} 

void USART_puts(char *s)
{
while (*s != 0)
	{
	USART_putc(*s);
	s++;
	}
}

short USART_byte_avail(void)
{
if(bit_is_set(UCSRA,RXC))
	return 1;
else
	return 0;	
}

char USART_getc(void)
{
while(bit_is_clear(UCSRA,RXC));
return UDR;	
}

void Term_reset(void)
{
USART_puts("\x1b""c");	
}

void Term_clr(void)
{
	USART_puts("\x1b""[2J");
}

void Term_xy(int x,int y)
{
char buffer[9]={};
short i=2;
buffer[0] = 0x1b;
buffer[1] = '[';
if (y>9)
		{
		buffer[i] = (y/10)+0x30;
		i++;
		}
buffer[i] = (y%10)+0x30;
i++;
buffer[i] = ';';
i++;
if (x>9)
		{
		buffer[i] = (x/10)+0x30;
		i++;	
		}
buffer[i] = (x%10)+0x30;
i++;
buffer[i] = 'H';
USART_puts(buffer);
}
#endif
