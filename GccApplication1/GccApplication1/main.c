/*
 * GccApplication1.c
 *
 * Created: 10/16/2022 7:37:45 AM
 * Author : KISHANTHAN
 */ 
#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
void setDataDirection(void);
void ADC_Init();
int ADC_Read(char channel);
//sending sms
void sendSMS();
void USART_Transmit(char data);
void senddata(char string[]);
void USART_Init();


int main(void)
{
	setDataDirection();
	int in=0;
	ADC_Init();
   
	USART_Init();
	sendSMS();
    while (1) 
    {
		
		in = ADC_Read(0);
		
		if (in >700){
			
			PORTB |=0B00000000;
		}
		else if(in >600) 
		{
			PORTB |=0B00010000;
		}
		else
		{
			PORTB |=0B00110000;
		}
		if ((PIND & 0B00000010)== 0)
		{
			PORTD &= 0B11011111;
		}
		else
		{
			
			PORTD |= 0B00100000;
		}
    }
}


void setDataDirection(void)
{
	DDRB |=0B00110011;//setting pin as the output pin
	DDRC &=0B11111110;//setting the pco as the input
	DDRD |=0B00101000;//setting the pd3 as the output
	DDRD &=0B11001011;//setting the pd2 and pd3 as the input
}
void ADC_Init()
{
    DDRC=0x0;               /* Make ADC port as input */
    ADCSRA = 0x87;          /* Enable ADC, fr/128  */
    ADMUX = 0x40;           /* Vref: Avcc, ADC channel: 0 */

}

int ADC_Read(char channel)
{

     int Ain,AinLow;

     ADMUX=ADMUX|(channel & 0x0f);  /* Set input channel to read */
     ADCSRA |= (1<<ADSC);           /* Start conversion */
     while((ADCSRA&(1<<ADIF))==0);  /* Monitor end of conversion interrupt */

     _delay_ms(10);
     AinLow = (int)ADCL;                /* Read lower byte*/
    Ain = (int)ADCH*256;           /* Read higher 2 bits and 
                                //    Multiply with weight */
    Ain = Ain + AinLow;             
    return(Ain);                    /* Return digital value*/

}

void sendSMS()
{
	
	senddata("AT");
	USART_Transmit(13);
	USART_Transmit(10);
	_delay_ms(1000);
	
	senddata("AT+CMGF=1");
	USART_Transmit(13);
	USART_Transmit(10);
	_delay_ms(1000);
	
	senddata("AT+CMGW=");
	USART_Transmit(34);
	senddata("+94729690261");  //Enter Your Mobile number
	USART_Transmit(34);
	USART_Transmit(13);
	USART_Transmit(10);
	_delay_ms(1000);
	
	senddata("There is a visitor");
	USART_Transmit(13);
	USART_Transmit(10);
}

void USART_Transmit(char data )
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0  = data;
}

void senddata(char string[])
{
	int len,count;
	len = strlen(string);

	for (count=0;count<len;count++)
	{
		USART_Transmit(string[count]);
	}
}

void USART_Init()
{
	
	UBRR0H =(unsigned char)(UBRR_VALUE>>8);
	UBRR0L =(unsigned char)(UBRR_VALUE);
	UCSR0B =(1<<TXEN0);
	UCSR0C =(3<<UCSZ00);
	
	
}
