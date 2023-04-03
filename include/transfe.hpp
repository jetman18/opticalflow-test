#ifndef _TRANSFE_
#define _TRANSFE_

/* uart to cp*/
class transfe
{
private:
    /* data */
    void reverse( char *str, int len);
    int intToStr(int x,  char *str, int d);
    struct termios options;
    int sig;
    int index;
    int uart_;
public:
    transfe(uint32_t baud,char* port);
    ~transfe();
    void write_char(char *);
    void write_int(int data);
    void send_data(char rate_x,char rate_y,unsigned char quality);
    int  ISuartReady();
};
#endif
