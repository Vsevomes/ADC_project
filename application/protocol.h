#ifndef PROTOCOL_H
#define PROTOCOL_H
#define SEND_SIG 0xAA
#define RECEIVE_SIG 0x55

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

void init_port_uart(int& port, struct termios& tty);
void receive_array(int port, std::vector<int>& sample, int& adc_freq);
void send_data(int port, char sig);

#endif // PROTOCOL_H
