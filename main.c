#include <REGX52.H>
#include <intrins.h>
#define u16 unsigned int
#define u32 unsigned long
#define uchar unsigned char

const char hexTable[] = "012346789abcdef";
char rbuf[5];

void Delay_ms(unsigned int ms)
{
  unsigned int i, j;
  for(i = 0; i < ms; i++)
    for(j = 0; j < 113; j++);  // 123 for 12M, 113.36 for 11.0592M
}

void hiccup()
{
  _nop_();
}

/*
void UART_Init(int baudrate)
{
	SCON = 0x50;  // Asynchronous mode, 8-bit data and 1-stop bit
	TMOD = 0x20;  // Timer1 in Mode2.
	TH1 = 256 - (11059200UL)/(long)(32*12*baudrate); // Load timer value for baudrate generation
	TR1 = 1;      // Turn ON the timer for Baud rate generation
}
*/

void UART_Init(void)
{
	SCON = 0x50;  // Asynchronous mode, 8-bit data and 1-stop bit
	TMOD = 0x20;  // Timer1 in Mode2.
	TH1 = 0xfd;   // baudrate = 9600
	TR1 = 1;      // Turn ON the timer for Baud rate generation
}

void UART_TxChar(char ch)
{
	SBUF = ch;      // Load the data to be transmitted
	while(TI==0);   // Wait till the data is trasmitted
	TI = 0;         // Clear the Tx flag for next cycle.
}

char UART_RxChar(void)
{
	while(RI==0);     // Wait till the data is received
	RI=0;             // Clear Receive Interrupt Flag for next cycle
	return(SBUF);     // return the received char
}

/* void printHex(uchar val)
{
	UART_TxChar(hexTable[val >> 4]);
	UART_TxChar(hexTable[val & 0x0f]);
} */

/* void printLongHex(u32 val)
{
	printHex(val >> 24);
	printHex(val >> 16);
	printHex(val >> 8);
	printHex(val);
	UART_TxChar('\n');
} */

void printLong(u32 val)
{
	uchar buffer[12];
	uchar i = 0;
	
	if (val == 0)
	{
		UART_TxChar('0');
		return;
	}
	
	while (val > 0) {
			buffer[i++] = '0' + (val % 10);
			val /= 10;
	}
	while (i-- > 0) {
			UART_TxChar(buffer[i]);
	}
}

#define COUNTER P2
sbit CNTCLR = P0^3;
sbit CNTLAT = P0^4;
sbit CNTOE0 = P0^5;
sbit CNTOE1 = P0^6;
sbit CNTOE2 = P0^7;

sbit DACCS_ = P1^1;
sbit DACSCK = P1^3;
sbit DACSDI = P1^2;
sbit DACLDAC_ = P1^0;

sbit STEPPUL_ = P3^2;
sbit STEPDIR_ = P3^3;
sbit STEPENA_ = P3^4;

void set_DAC(uchar channel, u16 value)
{
	uchar i = 0;
	DACCS_ = 0;
	hiccup();
	
	value &= 0x0fff;
	value |= ((u16)channel) << 15;
	value |= 0x3000;
	for(i = 16; i-- > 0; )
  {
		// hiccup();
		DACSDI = (value >> i) & 1;
		hiccup();
		DACSCK = 1;
		hiccup();
		DACSCK = 0;
		hiccup();
	}
	DACCS_ = 1;
	hiccup();
	
	DACLDAC_ = 0;
	hiccup();
	DACLDAC_ = 1;
	hiccup();
}

u32 get_count()
{
	u32 result = 0;
	CNTLAT = 1;
	hiccup();
	CNTCLR = 1;
	hiccup();
	
	CNTCLR = 0;
	Delay_ms(10);
	CNTLAT = 0;

	CNTOE0 = 0;
	hiccup();
	result |= (u32)COUNTER;
	CNTOE0 = 1;
	hiccup();
	
	CNTOE1 = 0;
	hiccup();
	result |= ((u32)COUNTER) << 8;
	CNTOE1 = 1;
	hiccup();
	
	CNTOE2 = 0;
	hiccup();
	result |= ((u32)COUNTER) << 16;
	CNTOE2 = 1;
	
	return result;
}

void main()
{
	uchar i, a[]={"UART Initialization!"};
	u16 val = 0;
	u32 result = 0;
	
	CNTLAT = 1;
	CNTCLR = 1;
	CNTOE0 = 1;
	CNTOE1 = 1;
	CNTOE2 = 1;
	
	DACCS_ = 1;
	DACSCK = 0;
	DACSDI = 0;
	DACLDAC_ = 1;
	
	STEPPUL_ = 1;
	STEPDIR_ = 1;
	STEPENA_ = 1;

	UART_Init();  //Initialize the UART module
	for(i = 0; a[i] != 0; i++)
	{
		UART_TxChar(a[i]);  // Transmit predefined string
	}

  while(1)
	{
		/* for(i = 0; i < 10; i++)
		{
			rbuf[i] = UART_RxChar();
			if (rbuf[i] = '\n') break;
		} */
		
		
		
		for(val = 0; val < 256; val++)
		{
			set_DAC(0, val);
			Delay_ms(1);
			result = get_count();
			printLong(val);
			UART_TxChar(' ');
			printLong(result);
			UART_TxChar('\n');
		}
		UART_TxChar('#');
		UART_TxChar('\n');
	}
}