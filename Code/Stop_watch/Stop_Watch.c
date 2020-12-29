#include<stdio.h>
#include<avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
unsigned char seconds;
unsigned char minutes;
unsigned char hours;



void INT_init(void){
	GICR|=(1<<INT1)|(1<<INT0)|(1<<INT2);
	MCUCR|=(1<<ISC01)|(1<<ISC11)|(1<<ISC10);
	MCUCR&=~(1<<ISC00);
	MCUCSR&=~(1<<ISC2);
	SREG|=(1<<7);
}


void TIMER1_init(void)
{
	TCCR1B|=(1<<2)|(1<<0)|(1<<WGM12);//prescaler (1024)& compare mode
	TCCR1B&=~(1<<1);
	TCCR1B&=~(1<<WGM13);
	TCCR1A=(1<<FOC1A)|(1<<FOC1B);// non PWM mode
	OCR1A=976; //compare level
	TCNT1=0;// start from zero
	TIMSK|=(1<<OCIE1A);// interrupt enable
	SREG|=(1<<7);//global interrupt enable




}



ISR(INT0_vect){
	seconds=0;
	minutes=0;
	hours=0;

}

ISR(INT1_vect){
	TCCR1B&=0xF8;//stop clock for timer 1


}


ISR(INT2_vect){
	TCCR1B|=(1<<2)|(1<<0);//clock working again
	TCCR1B&=~(1<<1);

}


ISR(TIMER1_COMPA_vect)
{
	seconds++;
	if(seconds==60){
		minutes++;
		seconds=0;
	}
	if(minutes==60){
		hours++;
		minutes=0;
	}
	if(hours==100){
		hours=0;
	}
}



int main(void){
	DDRA|=0x0F;
	DDRC=0xFF;
	DDRB&=~(1<<2);
	PORTB|=(1<<2);
	DDRD|=0x0C;
	PORTD|=(1<<2);
	PORTD&=~(1<<3);
	INT_init();
	TIMER1_init();

	seconds=0;
	minutes=0;
	hours=0;


	while(1){

		PORTA=(PORTA&0x81)|(1<<6);
		PORTC=(PORTC&0xF0)|(seconds%10);

		_delay_ms(5);

		PORTA=(PORTA&0x81)|(1<<5);
		PORTC=(PORTC&0xF0)|(seconds/10);

		_delay_ms(5);

		PORTA=(PORTA&0x81)|(1<<4);
		PORTC=(PORTC&0xF0)|(minutes%10);

		_delay_ms(5);

		PORTA=(PORTA&0x81)|(1<<3);
		PORTC=(PORTC&0xF0)|(minutes/10);

		_delay_ms(5);

		PORTA=(PORTA&0x81)|(1<<2);
		PORTC=(PORTC&0xF0)|(hours%10);

		_delay_ms(5);

		PORTA=(PORTA&0x81)|(1<<1);
		PORTC=(PORTC&0xF0)|(hours/10);

		_delay_ms(5);




	}



	return 0;
}
