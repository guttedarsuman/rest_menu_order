
#include <REGX51.H>

#define lcd_data P2	

sbit  lcd_rs   =  P2^0;  
sbit  lcd_en   =  P2^1;



void DelayMs(unsigned int count); 
void lcd_init(void);
void lcdcmd(unsigned char value);
void lcddata(unsigned char value);
void msgdisplay(unsigned char b[]) ;



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
	
	unsigned char i,ch;
	lcd_init();
	TMOD=0x20;
 	TH1=0xfd; //9600
 	SCON=0x50;
 	TR1=1;
	delay_ms(200);
	msgdisplay("RESTAURENT MENU ORDERING SYSTMS");
	delay_ms(1000);
/*	lcdcmd(0x01);
	msgdisplay("MENU LIST");
	lcdcmd(0x01);
	msgdisplay("1-IDLY 2-DOSA");
	lcdcmd(0x80);
	msgdisplay("3-TEA 4-COFFEE");
	delay_ms(3000);	*/	 	
	while(1)
	{
		do
		{
		ch=rx();
		}while(ch!='*');  
		 i=rx();
		if(i=='1')		
		{
		lcdcmd(0x01);
		msgdisplay("IDLY ORDERED");
// 		lcdcmd(0xc0);
// 		msgdisplay("PLACED T.No-1");
// 		delay_ms(300);
		i=0;		
		}
		else if(i=='2')		
		{
		lcdcmd(0x01);
		msgdisplay("DOSA ORDERED");
// 		lcdcmd(0xc0);
// 		msgdisplay("PLACED T.No-2");
// 		delay_ms(300);
		i=0;		
		}
		else if(i=='3')		
		{
		lcdcmd(0x01);
		msgdisplay("TEA ORDERED");
// 		lcdcmd(0xc0);
// 		msgdisplay("PLACED T.No-3");
// 		delay_ms(300);
		i=0;			
		}
		else if(i=='4')		
		{
		lcdcmd(0x01);
		msgdisplay("COFFEE ORDERED");
// 		lcdcmd(0xc0);
// 		msgdisplay("PLACED T.No-4");
// 		delay_ms(300);
		i=0;	   
		}
		else if(i=='8')		
		{
		
		//msgdisplay("COFFEE ORDERE-20");
		lcdcmd(0xc0);
		msgdisplay("PLACED T.No-1");
		delay_ms(300);
		i=0;	   
		}
		else if(i=='9')		
		{
		//msgdisplay("COFFEE ORDERE-20");
		lcdcmd(0xc0);
		msgdisplay("ORDER CONCELED");
		delay_ms(300);
		i=0;	   
		}
	
}
}


