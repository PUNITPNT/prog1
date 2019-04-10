/*
 */

#include <avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
volatile int Flag1=0;
volatile int Flag2=0;
void intadc()
{
    ADMUX=(1<<REFS0);
    ADCSRA|=(1<<ADEN)|(7<<ADPS0);
}
uint16_t readadc(uint8_t ch)
{
    ADMUX&=0xF8;
    ch=ch&0b00000111;
    ADMUX&=ch;
    ADCSRA|=(1<<ADSC);
    while(!(ADCSRA&(1<<ADIF)));
    ADCSRA|=(1<<ADIF);
    return(ADC);

}

uint16_t timer1pwm()
{

    TCCR1A|=((1<<COM1A1)|(1<<WGM11)|(1<<WGM10));
    TCCR1B|=((1<WGM12)|(1<<CS01)|(1<<CS00));
    TCNT1 = 0X00;


}

void interrupt01()
{
    DDRD&=~(1<<2);
    PORTD|=(1<<2);

    DDRD&=~(1<<3);
    PORTD|=(1<<3);

    //DDRB|=(1<<PB0);LED 1
    EICRA|=(1<<ISC00); //set INT0
    EIMSK|=(1<<INT0);


    EICRA|=(1<<ISC10); //set INT1
    EIMSK|=(1<<INT1);

    sei();
}
ISR(INT0_vect)
{
    Flag1=1;

}
ISR(INT1_vect)
{

    Flag2=1;
}


int main(void)
{   intadc();
    interrupt01();
    timer1pwm();
    DDRB|=(1<<PB1);
    DDRB|=(1<<PB2);//LED 2

    DDRD&=~(1<<0);//SWITCH
    PORTD|=(1<<0);
    while(1)
    {
     OCR1A=readadc(0);
    _delay_ms(100);
        if(Flag1==1 & (PIND&1 ) )
        {
            PORTB|=(1<<2); //led ON
            _delay_ms(1000);
            PORTB&=~(1<<2);
            Flag1=0;
        }
        else
        if(Flag2==1)
        {
            PORTB|=(1<<2); //led ON
            _delay_ms(1000);
            PORTB&=~(1<<2);
            Flag2=0;
        }


    }

return 0;
}
