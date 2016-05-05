#include <avr/io.h>				//biblioteki podstawowe
#include <util/delay.h>				//biblioteki - funkcja _delay


#define LED1 (1<<PD4)				//deklaracje pinów (nó¿ek) do których podpiête s¹ ledy i switch
#define LED2 (1<<PD5)
#define LED4 (1<<PB7)
#define LED3 (1<<PB4)
#define SWITCH (1<<PE6)


void adc_init()							// funkcja inicjalizacji ADC
{
	ADMUX = (1<<MUX0)|(1<<MUX1);		//wybór kana³u ADC3

	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	//w³¹czenie ADC, taktowanie 8MHz/128
}



uint16_t adc_read(uint8_t ch)		//funkcja odczytu wartoœci rejestrowanej przez adc - argument to numer kana³u adc
{
	ch &= 0x07;						// suma logiczna uniemo¿liwia wybranie kana³u spoza zakresu
	ADMUX = (ADMUX & 0xF8)|ch;		// clears the bottom 3 bits before ORing, bit ADLAR=0
	ADCSRA |= (1<<ADSC);			// start konwersji
	while(ADCSRA & (1<<ADSC));		// oczekiwanie na osi¹gniecie przez bit ADSC wartoœci 0 - ukoñczenia konwersji
	return (ADC);					//zwraca wynik (zakres 10bit~1024)
}

int main(){
uint16_t k, min, max, krok, poziom1, poziom2;
DDRB= LED3|LED4;		//odpowiednie piny portu B jako wyjœcia
DDRD= LED1|LED2;		//odpowiednie piny portu D jako wyjœcia
DDRE= ~SWITCH;			//odpowiedni pin portu E jako wejœcie


PORTB = 0x00;			//zerowanie wartoœci wyjœæ
PORTD = 0x00;

PORTE |= SWITCH;		//podci¹gniêcie pinu ze switchem do Vcc przez wewnêtrzny rezystor

adc_init();				//inicjalizacja ADC

while(1){
PORTD=LED1|LED2;
PORTB=LED3|LED4;
	if (!(PINE&SWITCH)){				//oczekiwanie na naciœniêcie switcha
PORTB=0x00;
PORTD=0x00;
	min=adc_read(3);					//w przypadku naciœniêcia odczyt wartoœci ADC do zmiennej min
	_delay_ms(1000);
		while(1){
			if (!(PINE&SWITCH)){		//oczekiwanie na naciœniêcie switcha
			max=adc_read(3);			//w przypadku naciœniêcia odczyt wartoœci ADC do zmiennej max
			krok=(max-min)/3;			//obliczenia granic zapalania siê kolejnych diód
			poziom1=min+krok;
			poziom2=min+2*krok;
				while(1){							//dzia³anie programu - odczyt napiêcia z ADC
					PORTB=0x00;	
					PORTD=0x00;						// i zapalanie odpowiedniej iloœci diód
					k=adc_read(3);
					if(k<min){
					PORTB=0x00;
					PORTD=0x00;
					}
					else if(k>=min&&k<poziom1){
					PORTB=0x00;
					PORTD=LED1;
					}
					else if(k>=poziom1&&k<poziom2){
					PORTB=0x00;
					PORTD=LED1|LED2;
					}
					else if(k>=poziom2&&k<max){	
					PORTB=LED3;
					PORTD=LED1|LED2;;
					}
					else{
					PORTD=LED1|LED2;
					PORTB=LED3|LED4;
					}				
					_delay_ms(200);
					}
					}
		}
	}
}
}
