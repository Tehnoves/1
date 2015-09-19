/*
*	05.11.14
*	работаем с 
*	V1.0 05.11.14
*  	V2.0 07.11.14    TIC5231 + АЦП
*   V3.0 измеритель тока
*   V4.0 индикация на матрице
*   V5.0 TIC120 
*   файл 887.c
*   RC1  -  LOAD
*   RC2  -  DIN
*   RC3  -  DCLK    SCK   SCL
*   RC4             SDI   SDA  
*   RC5             SDO   
*   RB0 RB1 RB2 RB3    RD0 - RD7   LCD
*   AT45DB041B
*  1   2    3    4   5   6  7     8
*  SI  SCK  RES  CS  WP  +  GND   SO  
*
*
*
*  23K256 
*  1   2    3    4   5   6    7     8 
*  CS  SO        GND SI  SCK  HOLD  +
*  V6.0  проба I2C для TIC   --------------------->
*
*/






#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h> // include standard header file
//#include <pic16f887.inc>
//#include <delays.h>
#define temp     -(4000) 

#define CS RC2

#define LOAD RC1
#define DIN RC2
#define DCLK RC0   //RC3

#define perviy RB0
#define vtoroy RB1
#define tretiy RB2
#define chvrtv RB3


bit flag;
char txt[7];
unsigned char sek,i,i2,decimal,bvminus,ii ,counter,counter;
unsigned int rez,rez_ind,text[4];
unsigned char SEGMENT[] = {0xf9, 0x50, 0xec, 0xf4, 0x55, 0xb5, 0xbd, 0xd0, 0xfd, 0xf5,  0x02};
//                            0     1     2     3     4     5     6    7     8      9     .


void null() {
	  DCLK = 1;    //GPIO |= (1<<dclk);
	  asm("nop");//asm nop
	  DCLK = 0;   //GPIO &= ~(1<< dclk);
	  asm("nop");//asm nop
	  DIN = 0;   //GPIO &= ~(1<< din);
	  asm("nop");//asm nop
  }
  
void edin () {
	  DCLK = 1;   //GPIO |= (1<<dclk);
	  asm("nop");//asm nop
	  DCLK = 0;  // GPIO &= ~(1<< dclk);
	  asm("nop");//asm nop
	  DIN = 1;   //GPIO |= (1<< din);
	  asm("nop");//asm nop
}
void minus() {
      null();
      null();
      null();
      null();
      null();
      edin();
      null();
      }
 void zero () {
      edin();
      edin();
      edin();
      edin();
      edin();
      null();
      edin();
 }

 void one() {
		null();// edin();
		null();
		null();// edin();
		null();
		edin();//null();
		null();
		edin();//null();

 }

 void two() {
      edin(); //
      null();
      edin();
      edin();
      null();
      edin(); //
      edin();  //
 }

 void tree() {
      edin();
      null();
      null();
      edin();
      edin();
      edin();
	  edin();  //  null();
 }
 void four() {  //2675
      null();  //
      edin();  //
      null();
      null();
      edin();
      edin();
      edin();
 }
 void five() {
      edin();
      edin();
      null();
      edin();
      edin();
      edin();
      null();
 }
 void six() {   //
      edin();
      edin();
      edin();
      edin();
      edin();
      edin();
      null();
 }
 void seven() {   //175   2346
      edin();
      null();
      null();
      null();
      edin();
      null();
      edin();
 }
 void eight() {
      edin();
      edin();
      edin();
      edin();
      edin();
      edin();
      edin();
 }
 void nine() {   //
      edin();
      edin();
	  null();//edin();//edin();
      edin();
      edin();
      edin();//edin();
      edin();
 }
 void pusto() {
      null();
      null();
      null();
      null();
      null();
      null();
      null();
 }

 void cas(unsigned char qwe) {

      switch (qwe)
           {
          case  0x30: zero();break;
          case  0x31: one();break;
          case  0x32: two();break;
          case  0x33: tree();break;
          case  0x34: four();break;
          case  0x35: five();break;
          case  0x36: six(); break;
          case  0x37: seven(); break;
          case  0x38: eight();break;
          case  0x39: nine();break;
          case  0x20: pusto();break;
          case  '-': minus();break;
           }
 }
 void indik (void)
 {
 
 


	null();
	null();
	null();

	cas(txt[0]);	
	null();//edin();  	 
	cas(txt[1]);
		
	cas(txt[2]);	
	edin();	
	cas(txt[3]);
	cas(txt[4]);	  	 	
	edin();	 
		 
							


    LOAD = 1; // GPIO |= (1<<load);
    asm("nop");//asm nop
    asm("nop");//asm nop
    asm("nop");//asm nop
    asm("nop");//asm nop
    LOAD = 0;  //GPIO &= ~(1<< load);

 }
	void interrupt Interrupt()
		{

if (PIR1bits.ADIF)  // there is only one interrupt vector so you should check to verify what caused the interrupt
				{
				
					rez =ADRESH << 8;
 					rez += ADRESL;
					PIR1bits.ADIF = 0;
					ADCON0bits.GO =1;
					flag = 1;
				}


if (INTCONbits.RBIF)
							{
								
								i2++;
							///	if (i2 & 1)
								///	RD1 = 0;	
							///	else
								///	RD1 = 1;
								INTCONbits.RBIF= 0;
								}	
	if (INTCONbits.T0IF)							
			{
				
			INTCONbits.T0IF= 0;	
			}
								
	if (PIR1bits.TMR1IF)  // there is only one interrupt vector so you should check to verify what caused the interrupt
				{
				TMR1 = 0xff00;
				PORTD = 0xff;
					PORTB =  1 << counter ;
										i=text[3-counter] ;
					PORTD = ~(SEGMENT[i]);
					counter++;
					if (counter >= 4)
						counter = 0;
				
					PIR1bits.TMR1IF=0; 
				}
if (PIR1bits.TMR2IF)  // there is only one interrupt vector so you should check to verify what caused the interrupt
				{
		
					TMR2=(0);
					sek++;
//unsigned char SEGMENT[] = {0xf9, 0x50, 0xec, 0xf4, 0x55, 0xb5, 0xbd, 0xd0, 0xfd, 0xf5,  0x02};
//                            0     1     2     3     4     5     6    7     8      9    					
					

					
					if ( sek ==30)
						{
							sek = 0;
					///		RD0 = ~RD0;
						}
					//TMR1H =(0xff & (temp >> 8));  
 																				//	PORTBbits.RB2=~PORTBbits.RB2;
            		PIR1bits.TMR2IF=0;        												// ...and then clear the interrupt flag before exiting
				}
		}



	int main(int argc, char** argv)
{

	
	OSCCONbits.IRCF =0xf;  //  0100 q000
/*	
text[0] = 1;
text[1] = 2;
text[2] = 3;
text[3] = 4;
while (1)
{
	i =  1 << counter ;
					
					ii = SEGMENT[text[counter]];
counter++;
if (counter >= 4)
	counter = 0;
}*/
	ANSELH =0;
	ANSEL =0x1;
	PORTA  = 0;
	TRISA  = 1; 	
						//	LATA   = 0;
	ADCON0bits.ADCS = 0x3;	
	ADCON0bits.CHS = 0x0;
	ADCON1bits.ADFM = 1;
	ADCON1bits.VCFG1 =0;
	ADCON1bits.VCFG0 =0;
	ADCON0bits.ADON =1;
						/////	CMCON  = 0x07;
						// 1-input   0 - output
	TRISB  = 0;
	PORTB  = 0;
						//	LATB   = 0;
	TRISC  = 0;
	PORTC  = 0;
	PORTD  = 0;
						//	LATD   = 0;
	TRISD  = 0;
						//	PORTE  = 0;
						//	LATE   = 0;
						//	TRISE  = 0;
	sek = 0;	
						//	RA0 = 1;
						//	LATA0  =0;
	CM1CON0bits.C1ON = 0;	
	CM2CON0bits.C2ON = 0;
	T1CONbits.TMR1ON = 0x1;
	T2CONbits.TOUTPS = 0xf;
	T2CONbits.T2CKPS = 0x2;
	T2CONbits.TMR2ON = 0x1;
	TMR2=(0);

	IOCBbits.IOCB = 1;
//	OPTION_REGbits.T0CS=0;
									//	TMR2H =(0xff & (temp >> 8));  T0IE
	INTCONbits.PEIE = 1;          	// Enable peripheral interrupt
	INTCONbits.RBIE = 1;
	INTCONbits.T0IE = 1;
	PIE1bits.TMR1IE =1;
	PIE1bits.TMR2IE =1;
	PIE1bits.ADIE =1;
	PIE2bits.EEIE =1;


	INTCONbits.GIE  = 1;           	// enable global interrupt
	ADCON0bits.GO =1;
						//CS = 0;
						//CS = 1;
//	RD0 = 0;
//	RD0 = 1;
						 //	RD1 = 0;
						//	RD1 = 1;
						//	RD2 = 0;
						//	RD2 = 1;
						//	RD3 = 0;
						//	RD3 = 1;
						//	RD4 = 0;
						//	RD4 = 1;
						//	RD5 = 0;
						//	RD5 = 1;
						//	RD6 = 0;
						//	RD7 = 0;
						//	RD7 = 1;	RD6 = 1;

	
						//eeprom_write(0x01,0x055);
						//i = eeprom_read(0x01);
        txt[0] =' ';
        txt[1] =' ';
        txt[2] ='0';
		txt[3] ='0';
        txt[4] ='0';
	 								 // txt[5] ='6';	
      	indik();
		flag = 0;
//	perviy = 1;
    //	perviy = 0;
	//	vtoroy = 1;
	//		vtoroy = 0;

	//	RB2 = 1;
	//RB2 = 0;
		//RB3 =1;
		//	RB3 =0;

		//	PORTB = 0xff;
			PORTD = 0xff;
/*
			PORTD = ~SEGMENT[0];
		PORTD = ~SEGMENT[1];
		PORTD = ~SEGMENT[2];
	PORTD = ~SEGMENT[3];
PORTD = ~SEGMENT[4];
PORTD = ~SEGMENT[5];
PORTD = ~SEGMENT[6];
PORTD = ~SEGMENT[7];
PORTD = ~SEGMENT[8];
PORTD = ~SEGMENT[9];
PORTD = ~SEGMENT[10];
	*/		/*
							RD0 = 0;
							RD0 = 1;
							RD1 = 0;
							RD1 = 1;
							RD2 = 0;
							RD2 = 1;
							RD3 = 0;
							RD3 = 1;
							RD4 = 0;
							RD4 = 1;
							RD5 = 0;
							RD5 = 1;
							RD6 = 0;
							RD6 = 1;
							RD7 = 0;
							RD7 = 1;	
		PORTD = 0;*/
text[0] = 1;
text[1] = 2;
text[2] = 3;
text[3] = 4;
counter = 1;
	while (1)
	{
	if (flag)
			{
				INTCONbits.GIE  = 0; 
				flag = 0;
				rez_ind = (int) (100.0*75.0*rez/1024);
				
											//	rez_ind = 1234;
				txt[4] = (rez_ind % 10 + '0');
				txt[3] = (rez_ind % 100/10+ '0') ;
				txt[2] = (rez_ind % 1000/100+ '0') ;
				txt[1] = (rez_ind % 10000/1000+ '0') ;
				if (txt[1] == '0')
					txt[1] = ' ';
				indik();
				INTCONbits.GIE  = 1; 
			}
	}
	
}