#ifndef PROTOCOL_H
#define PROTOCOL_H
#define SEND_SIG 0xAA
#define RECEIVE_SIG 0x55
//#define DIVIDER_SIG 0xBB
#define DIVIDER_250Hz 0xCA
#define DIVIDER_500Hz 0xCB
#define DIVIDER_1000Hz 0xCC
#define DIVIDER_2000Hz 0xCD
#define DIVIDER_4000Hz 0xCE


#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

void init_port_uart(int& port, struct termios& tty);
void receive_array(int port, std::vector<int>& sample, int& adc_freq);
void send_signal(int port, char sig);

#endif // PROTOCOL_H
