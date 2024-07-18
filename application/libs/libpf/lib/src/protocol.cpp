#include "protocol.h"
#include <QThread>

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

    tty.c_cc[VTIME] = 0;    // Время ожидания
    tty.c_cc[VMIN] = 0;      // Минимальное количество символов для чтения

    cfsetispeed(&tty, B230400);
    cfsetospeed(&tty, B230400);

    if (tcsetattr(port, TCSANOW, &tty) != 0) {
        std::cerr << "Ошибка при установке настроек порта" << std::endl;
        return;
    }
}

void send_signal(int port, char sig){
    write(port, &sig, 1);
}

void receive_array(int port, std::vector<int>& sample, int& adc_freq){
    int bytes_read;
    int flag = 0;
    int read_flag = 0;
    int break_flag = 0;
    std::string adc_f;
    std::string size;
    std::string numb;
    int iter = 0;
    char buffer[2048];

    while (true) {
        bytes_read = read(port, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Добавление завершающего нуля

            std::cout << "Bytes read: " << bytes_read << std::endl;
            std::cout << "Buffer content: " << buffer << std::endl;

            if (buffer[0] == RECEIVE_SIG){
                read_flag = 1;
            }

            if (read_flag == 1){
                for (size_t i = 2; i < bytes_read + 1; ++i){

                    if (flag == 0){
                        if (buffer[i] == '\n'){
                            flag = 1;
                            adc_freq = std::atoi(adc_f.c_str());
                        }
                        else
                            adc_f += buffer[i];
                    }
                    else if (flag == 1){
                        if (buffer[i] == '\n'){
                            flag = 2;
                            sample.resize(std::atoi(size.c_str()));
                        }
                        else
                            size += buffer[i];

                    }
                    else if (flag == 2){
                        if (buffer[i] == '\n')
                            flag = 3;
                        else if (buffer[i] == ' '){
                            sample[iter] = std::atoi(numb.c_str());
                            numb.clear();
                            iter++;
                        }
                        else
                            numb += buffer[i];
                    }
                    else if (flag == 3)
                        break_flag = 1;
                }
            }

            if (break_flag == 1){
                break;
            }

        } else if (bytes_read == -1) {
            std::cerr << "Ошибка при чтении данных" << std::endl;
            break;
        }

        QThread::msleep(180);
    }
}
