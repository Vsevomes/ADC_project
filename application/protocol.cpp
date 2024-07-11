#include "protocol.h"

void init_port_uart(int& port, struct termios& tty){
    if (tcgetattr(port, &tty) != 0) {
        std::cerr << "Ошибка при чтении настроек порта" << std::endl;
        return;
    }

    tty.c_cflag &= ~PARENB; // Отключение бита четности
    tty.c_cflag &= ~CSTOPB; // Использование одного стоп-бита
    tty.c_cflag &= ~CSIZE;  // Очистка размера символов
    tty.c_cflag |= CS8;     // Установка 8-битных символов
    tty.c_cflag &= ~CRTSCTS; // Отключение аппаратного управления потоком
    //tty.c_cflag |= CREAD | CLOCAL; // Включение чтения и игнорирование управляющих сигналов

    tty.c_lflag &= ~ICANON; // Отключение канонического режима
    tty.c_lflag &= ~ECHO;   // Отключение эха

    tty.c_cc[VTIME] = 100;    // Время ожидания
    tty.c_cc[VMIN] = 0;      // Минимальное количество символов для чтения

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    if (tcsetattr(port, TCSANOW, &tty) != 0) {
        std::cerr << "Ошибка при установке настроек порта" << std::endl;
        return;
    }
}

void send_data(int port, char sig){
    write(port, &sig, 1);
}

void receive_array(int port, std::vector<int>& sample, int& adc_freq){
    int bytes_read;
    int flag = 0;
    int read_flag = 0;
    std::string adc_f;
    std::string size;
    std::string numb;
    int iter = 0;
    char buffer[256];

    while (true) {
        bytes_read = read(port, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Добавление завершающего нуля

            // for (int i = 0; i < sizeof(buffer); ++i){
            //     std::cout << bytes_read << std::endl;
            //     std::cout << buffer[i];
            // }
            // std::cout << std::endl;

            if (buffer[0] == RECEIVE_SIG){
                bytes_read = read(port, buffer, sizeof(buffer) - 1);
                bytes_read = read(port, buffer, sizeof(buffer) - 1);
                read_flag = 1;
            }

            if (read_flag == 1){
                if (flag == 0){
                    if (buffer[0] == '\n'){
                        flag = 1;
                        adc_freq = std::atoi(adc_f.c_str());
                    }
                    else
                        adc_f += buffer[0];
                }
                else if (flag == 1){
                    if (buffer[0] == '\n'){
                        flag = 2;
                        sample.resize(std::atoi(size.c_str()));
                    }
                    else
                        size += buffer[0];

                }
                else if (flag == 2){
                    if (buffer[0] == '\n')
                        flag = 3;
                    else if (buffer[0] == ' '){
                        sample[iter] = std::atoi(numb.c_str());
                        numb.clear();
                        iter++;
                    }
                    else
                        numb += buffer[0];
                }
                else if (flag == 3)
                    break;
            }

        } else if (bytes_read == -1) {
            std::cerr << "Ошибка при чтении данных" << std::endl;
            break;
        }
    }
}
