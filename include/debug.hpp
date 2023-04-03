#ifndef _DEBUG_
#define _DEBUG_

/* uart to cp*/
class debug
{
private:
    /* data */
    void reverse( char *str, int len);
    int intToStr(int x,  char *str, int d);
    struct termios options;
    int sig;
    int index;
public:
    debug(uint32_t baud,char* port);
    ~debug();
    void write_char(char *);
    void write_int(int data);
    void debug::send_data(char rate_x,char rate_y,unsigned char quality);
};
#endif
