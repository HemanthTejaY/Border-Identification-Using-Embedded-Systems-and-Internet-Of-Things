#include<htc.h>
#define lcd PORTD
#define EN RE0
#define RW RE1
#define RS RE2
#define E1 RC0
#define E2 RC1
#define E3 RC3
#define vib RC2
#define sw RC5
#define V1 RB0
#define V2 RB1
#define V3 RB2
#define m1 RB3
#define m2 RB4
#define m3 RB6
#define m4 RB7
#define buzzer RB5
#define CCP PR2
#define duty_cycle CCPR1L  
void pwm_change(unsigned int);
#define _XTAL_FREQ 10000000 
unsigned int d=0,z=0;
void gsm(unsigned char *sms);
unsigned char rx();
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & CP_OFF & BOREN_ON & LVP_OFF & CPD_OFF & DEBUG_OFF);
void string(unsigned char *p);
void tx(unsigned char byte);
void lcd_init();
void lcd_data(unsigned char);
void lcd_cmd(unsigned char);
void string(unsigned char*);
void num(unsigned int);
void uart_string(const char *q);
char t;

void main()
{
nRBPU=0;
ADCON1=0x02;
TRISD=0x00;
PORTD=0x00;
TRISE=0x00;
PORTE=0x00;
TRISC=0xBf;
//PORTC=0x00; 
TRISB0=0;
TRISB1=0;
TRISB2=0;
TRISB3=0;
TRISB4=0;
TRISB5=0;
TRISB6=0;
TRISB7=0;
TXSTA=0x24;
RCSTA=0x90;
SPBRG=64; 
//T1CON=0x10;                
T2CON=0x03;
CCP=0xDC;
CCP1CON = 0x0F;                                              
INTCON = 0xC0; 
lcd_init();
E1=E2=E3=0;
string(" BOAT  SPEED");
lcd_cmd(0xc0);
string("ADJUSTING SYSTEM");
__delay_ms(3000);
V1=V2=V3=1;
while(1)
{
buzzer=0;
if(sw==1)
{
lcd_cmd(0x01);
lcd_cmd(0x80);
string("emergency");
gsm("emergency");
__delay_ms(1000);
}
if(vib==0)
{
lcd_cmd(0x01);
lcd_cmd(0x80);
string("OBSTACLE DETECTED");
gsm("OBSTACLE DETECTED - VIBRATION MOTOR IS ON");
__delay_ms(1000);
buzzer=1;
}
if(E1==1)
{
buzzer=0;
E2=E3=0;
m1=m2=1;
m3=m4=0;
lcd_cmd(0x01);
lcd_cmd(0x80);
string("long");
//pwm_change(0x0a);
V1=0;
__delay_ms(500);
V1=1;
__delay_ms(2000);

}
if(E2==1)
{
E1=E3=0;
m1=m2=1;
lcd_cmd(0x01);
lcd_cmd(0x80);
string("middle");
buzzer=0;
//pwm_change(0x05);
V2=0;
__delay_ms(500);
V2=1;
__delay_ms(2000);
}
if(E3==1)
{
m1=m2=0;
m3=m4=1;
E1=E2=0;
lcd_cmd(0x01);
lcd_cmd(0x80);
string("near");
//pwm_change(0x00);
V3=0;
__delay_ms(500);
V3=1;
gsm("WARNING : THE BOAT HAS CROSSED THE BORDER !!");
buzzer=1;
__delay_ms(2000);
}
}
}

void pwm_change(unsigned int d_c)
{
unsigned int x;
T2CON=(7<<z);
duty_cycle=(d_c*0x16);
CCP1CON &= (0x0f);
}

void lcd_data(unsigned char a)
{
RS=1;
RW=0;
__delay_ms(30);
lcd=a&0xf0;
EN=1;
__delay_ms(30);
EN=0;
__delay_ms(30);
lcd=(a<<4)&0xf0;
EN=1;
__delay_ms(30);
EN=0;
__delay_ms(30);
}
void lcd_cmd(unsigned char a)
{
RS=0;
RW=0;
__delay_ms(30);
lcd=a&0xf0;
EN=1;
__delay_ms(30);
EN=0;
__delay_ms(30);
lcd=(a<<4)&0xf0;
EN=1;
__delay_ms(30);
EN=0;
__delay_ms(30);
}
void num(unsigned int x)
{
unsigned int a,b,c,d,e;
a=x/1000;
b=(x%1000)/100;
c=(x%100);
d=(c/10);
e=(c%10);
__delay_ms(50);
lcd_cmd(0x80);
lcd_data(a+0x30);
lcd_data(b+0x30);
lcd_data(d+0x30);
lcd_data(e+0x30);
}
void lcd_init()
{
lcd_cmd(0x02);
lcd_cmd(0x28);
//lcd_cmd(0x0e);
lcd_cmd(0x0c);
lcd_cmd(0x80);
}
void string(unsigned char *p)
{
while(*p)
{
lcd_data(*p++);
}
}


void uart_string(const char *q)
{
while(*q)
{
tx(*q++);
}
}

void gsm(unsigned char *sms)
{
lcd_cmd(0X01);
   lcd_cmd(0X80);
  string("SMS SENDING...");
  __delay_ms(2000);
	uart_string("AT");
    tx(0x0d);
    tx(0x0a);
__delay_ms(2000);
	uart_string("AT+CMGF=1");
	tx(0x0d);
	tx(0x0a);
__delay_ms(2000);
	uart_string("AT+CMGS=\"");
uart_string("+916380053212");
	uart_string("\"");
__delay_ms(2000);
	tx(0x0d);
	tx(0x0a);
__delay_ms(2000);	
	uart_string(sms);
	tx(0x0d);
	tx(0x1a);
//__delay_ms(100);
tx((char)26);
//tx((char)26);
__delay_ms(2000);
}

void tx(unsigned char byte)
{
int i;
TXREG=byte;
while(!TXIF)
for(i=0;i<=400;i++);
}

unsigned char rx()
{
while(!RCIF);
return RCREG;
}
