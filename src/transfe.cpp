#include "transfe.hpp"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
//#define SERIAL_DEVICE "/dev/ttyS1"
transfe::transfe(uint32_t baud,char *port)
{  
    int fp =-1;
    uart_ = 1;
    fp = open(port, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (fp == -1)
	{   
        uart_ = 0;
		printf("Error - Unable to open UART.\n");
	}
   	tcgetattr(fp, &options);
	options.c_cflag = baud | CS8 | CLOCAL | CREAD;	
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(fp, TCIFLUSH);
	tcsetattr(fp, TCSANOW, &options);

}
transfe::~transfe()
{
}

int transfe::ISuartReady()
{
    return uart_;
}
void transfe::reverse( char *str, int len)
{
    int i = sig, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
int transfe::intToStr(int x,  char *str, int d)
{
    while (x) {
        str[index++] = (x % 10) + '0';
        x = x / 10;
    }

    while (index < d)
        str[index++] = '0';
    reverse(str,index);
    return index;
}

void transfe::write_char(char *str)
{
    if (fp = -1)return;
    uint16_t len=0;
    while(str[len++]);
	write(fp,str,len);		
}

void transfe:: write_int(int x)
{   
    index=0;
    sig = 0;
    char str_[11];
    memset(str_,0,11);
    if(x<0){
        x*=-1;
        str_[0] = '-';
        index++;
        sig = 1;
    }
    int len = intToStr(x,str_,0);
    write(fp,str_,len);	
}
void transfe::send_data(char rate_x,char rate_y,unsigned char quality)
{
   char str[7];
   str[0] = 0xb5;
   str[1] = 0x62;
   str[2] = rate_x;
   str[3] = rate_y;
   str[4] = quality;
   int16_t sum = rate_x + rate_y + quality;
   str[5] = *(char*)&sum[0];
   str[6] = *(char*)&sum[1];
   write(fp,str,7);	
}

