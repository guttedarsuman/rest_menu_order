#include <REGX51.H>

#define lcd_data P2	

sbit  lcd_rs   =  P2^0;  
sbit  lcd_en   =  P2^1;
/***Keypad intialization***/

sbit r0=P1^0;   
sbit r1=P1^1;
sbit r2=P1^2;
sbit r3=P1^3;

sbit c0=P1^4;   
sbit c1=P1^5;
sbit c2=P1^6;

char keypad_lut[4][3]={1,2,3,4,5,6,7,8,9,'*',0,'#'};

/*************************/
void DelayMs(unsigned int count); 
void lcd_init(void);
void lcdcmd(unsigned char value);
void lcddata(unsigned char value);
void msgdisplay(unsigned char b[]) ;

bit colscan()
{
	return(c0&c1&c2);
}

/***Key scan function declaration***/

unsigned char keyscan()
{
	unsigned char rowval,colval;
	c0=c1=c2=1;	
	r0=r1=r2=r3=0;
	while(colscan());
	r0=0;
	r1=r2=r3=1;
	if(!colscan())
	{
		rowval=0;
		goto colcheck;
	}
	r1=0;
	r0=r2=r3=1;
	if(!colscan())
	{
		rowval=1;
		goto colcheck;
	}
	r2=0;
	r0=r1=r3=1;
	if(!colscan())
	{
		rowval=2;
		goto colcheck;
	}
	r3=0;
	r0=r1=r2=1;
	if(!colscan())
	{
		rowval=3;
	}
	colcheck:
			if(c0==0)
			colval=0;
			else if(c1==0)
			colval=1;
			else if(c2==0)
			colval=2;
			while(!(c0&&c1&&c2));
			return keypad_lut[rowval][colval];
}

/***********************************/

/****Delay function declaration****/

void delay_ms(unsigned int n)
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<1275;j++);
	}
}

/**********************************/


  void delay(unsigned int v)
{
	unsigned int i,j;
	for(i=0;i<=v;i++)
	for(j=0;j<=1275;j++);	 
}
void lcd_init(void)
{
 lcdcmd(0x02);	
 lcdcmd(0x02);
 lcdcmd(0x28);  //intialise the lcd in 4 bit mode*/
 lcdcmd(0x28);  //intialise the lcd in 4 bit mode*/
 lcdcmd(0x0e);	//cursor blinking
 lcdcmd(0x06);	//move the cursor to right side
 lcdcmd(0x01);	//clear the lcd
}	
 void lcdcmd(unsigned char value)		   // LCD COMMAND
{
	lcd_data=value&(0xf0); //send msb 4 bits
	lcd_rs=0;	  //select command register
	lcd_en=1;	  //enable the lcd to execute command
	delay(3);
	lcd_en=0;  
	lcd_data=((value<<4)&(0xf0));	 //send lsb 4 bits
	lcd_rs=0;	 //select command register
	lcd_en=1;	 //enable the lcd to execute command
	delay(3);
	lcd_en=0;  
} 
void lcddata(unsigned char value)
{
	lcd_data=value&(0xf0); //send msb 4 bits
	lcd_rs=1;	  //select data register
	lcd_en=1;	  //enable the lcd to execute data
	delay(3);
	lcd_en=0;  
	lcd_data=((value<<4)&(0xf0));	 //send lsb 4 bits
	lcd_rs=1;	  //select data register
	lcd_en=1;	  //enable the lcd to execute data
	delay(3);
	lcd_en=0;  
	delay(3); 
}
void msgdisplay(unsigned char b[]) // send string to lcd
{
	unsigned char s,count=0;
	for(s=0;b[s]!='\0';s++)
	{
		count++;	 
		if(s==16)
		lcdcmd(0xc0);
		if(s==32)
		{
			lcdcmd(1);
			count=0;
		}
		lcddata(b[s]);
	}
}
//////////////////////////////////
unsigned char rx()
 {
  unsigned char rx; 
   while(RI == 0);
    rx=SBUF;
	RI=0;
	return rx;
 }

void tx(unsigned char *tx)
 {

   for(;*tx != '\0';tx++)
     {
	   SBUF=*tx;
	  while(TI == 0);
	   TI=0;  
	 }
 }

void tx1(unsigned char tx)
 {
 
	   SBUF=tx;
	  while(TI == 0);
	   TI=0;
	
 }
//////////////////////////////////
void main()
{
	
	unsigned char i;
	lcd_init();
	TMOD=0x20;
 	TH1=0xfd; //9600
 	SCON=0x50;
 	TR1=1;
	delay_ms(200);
	msgdisplay("RESTAURENT MENU ORDERING SYSTMS");
	delay_ms(300);
	lcdcmd(0x01);
	msgdisplay("MENU LIST");
	lcdcmd(0x01);
	msgdisplay("1-IDLY 2-DOSA");
	lcdcmd(0xc0);
	msgdisplay("3-TEA 4-COFFEE");
	delay_ms(1000);		 	
	while(1)
	{
		lcdcmd(0x01);
		msgdisplay("MENU LIST");
		lcdcmd(0x01);
		msgdisplay("1-IDLY 2-DOSA");
		lcdcmd(0xc0);
		msgdisplay("3-TEA 4-COFFEE");
		delay_ms(100);	
		lcdcmd(0x01);
		msgdisplay("GIVE ORDER");
		i=0x30+keyscan();
		if(i=='1')		
		{
		tx("*1");
		lcdcmd(0x01);
		msgdisplay("IDLY ORDERED-20");
		delay_ms(500);
		i=0;		
		}
		else if(i=='8')
		{
		tx("*8");
		lcdcmd(0x01);	
		msgdisplay("ORDERED ");
		delay_ms(500);	
		i=0;
		}
		else if(i=='9')
		{
		tx("*9");
		lcdcmd(0x01);	
		msgdisplay("ORDER CONCELED");
		delay_ms(500);	
		i=0;	
		}
		else if(i=='2')		
		{
		tx("*2");
		lcdcmd(0x01);	
		msgdisplay("DOSA ORDERED-20");
		delay_ms(500);	
		i=0;	
		}
		else if(i=='3')		
		{
		tx("*3");
		lcdcmd(0x01);	
		msgdisplay("TEA ORDERED-20");
		delay_ms(500);
		i=0;		
		}
		else if(i=='4')		
		{
		tx("*4");
		lcdcmd(0x01);	
		msgdisplay("COFFEE ORDERED-20");
		delay_ms(500);	
		i=0;	
		}
		else
		{
		lcdcmd(0x01);	
		msgdisplay("WRONG OPTION");
		delay_ms(1000);	
		i=0;
		}
}
}


