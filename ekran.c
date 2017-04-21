*/* ekran.c: демонстрирует использование модуля 
 * ЖК-дисплея выводя на нем простое сообщение.
 * Часть материала взята :
 *   ::  Copyright 2011, MSPSCI
 *   ::  http://mspsci.blogspot.com
 *    Часть переработана мной 
 */


#include <msp430g2553.h>

#define RS BIT0
#define E BIT1
#define D4 BIT4
#define D5 BIT5
#define D6 BIT6
#define D7 BIT7

void pulse(void);//посыл стробирующего импульса(импульс для определения конца  
void timerA(void) __attribute((interrupt(TIMER0_A0_VECTOR)));
void LCM_init(void);//инициализация дисплея
void SendByte(char,char);
int overflows = 0;



void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;//останавливаем сторожевой таймер
 
  P1OUT = 0;//обнуляем выходы
  P1DIR = 0xF0;//выбираем работу на выход P1.4-P1.8
  
  DCOCTL = 0x10FE ;
  BCSCTL1 = 0x10FF ;


  
  TACTL |= ID_3 + TASSEL_2 + MC_1;  //  SMCLK / 8 = 1.1 MHz / 8 = 137.5 kHz
  TACCTL0 = CCIE;//Вызвать прерывание по достижению TACCR0
  LCM_init();

  __bis_SR_register(GIE);//разрешаем прерывания 
  
  for(int i = 0;i<1;++i)
    {}
    __delay_cycles(100000);
    SendByte(0x58,1);
    __delay_cycles(100000);
    SendByte(0x59,1);
    __delay_cycles(100000);
    SendByte(0x69,1);
    __delay_cycles(100000);
    SendByte(0x6A,1);
    }

}


void LCM_init(void)
{
 P1DIR |= (RS+E+D4+D5+D6+D7);//делаем все ножки на выход
 P1OUT &= ~(RS+E+D4+D5+D6+D7); 

 __delay_cycles(100000);
 
 SendByte(0x30,0);
__delay_cycles(100000);

 SendByte(0x28,0);
__delay_cycles(100000);

 SendByte(0x28,0);
__delay_cycles(100000);

 SendByte(0x08,0);
__delay_cycles(100000);

 SendByte(0x01,0);
__delay_cycles(100000);

 SendByte(0x06,0);
__delay_cycles(100000);

 SendByte(0x0F,0);
__delay_cycles(100000);
}


void pulse()
{
P1OUT |= E;//подаем импульс для считывания
__delay_cycles(200);

P1OUT &= ~E;//снимаем импульс для формирования заднего фронта по которому производится считывание
__delay_cycles(200);
}
void SendByte(char ByteToSend,char isData)
{
 P1OUT &= ~(RS+E+D4+D5+D6+D7);
 
 if(isData == 1)P1OUT |= RS;
 else P1OUT &= ~RS;

 P1OUT |= (ByteToSend & 0xF0);

 pulse();
 
 
 P1OUT &= ~(RS+E+D4+D5+D6+D7);

 if(isData == 1)P1OUT |= RS;
 else P1OUT &= ~RS;  
 
 P1OUT |= ((ByteToSend & 0x0F)<<4);
 
 pulse();
}   


void timerA(void)
{
overflows++;
}


