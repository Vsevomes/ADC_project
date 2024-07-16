#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QThread>
#include <QRegularExpression>
#include "protocol.h"
#include "function.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class ChartThread : public QObject {
    Q_OBJECT

public slots:
    void doWork() {
        struct termios tty;
        int serial_port = open("/dev/ttyUSB0", O_RDWR);
        if (serial_port < 0) {
            std::cerr << "Ошибка при открытии UART порта" << std::endl;
            return;
        }

        init_port_uart(serial_port, tty);

        while (!stop) {
            send_signal(serial_port, SEND_SIG);

            std::string freq, amplitude, low_volt, deviation;
            std::vector<int> sample;
            int adc_freq;
            int warn_flag;

            receive_array(serial_port, sample, adc_freq);

            find_freq(sample, adc_freq, freq, warn_flag);
            find_min_max_volt(sample, amplitude, low_volt);
            find_deviation(sample, deviation);

            std::cout << "Data received: freq=" << freq << ", amplitude=" << amplitude << ", low_volt=" << low_volt << ", deviation=" << deviation << std::endl;

            emit resultReady(sample, freq, amplitude, low_volt, deviation, warn_flag);

            QThread::msleep(1000);
        }

        ::close(serial_port);
        emit workFinished();
    }

    void stopWork() {
        stop = true;
    }

signals:
    void resultReady(const std::vector<int>& sample, const std::string& freq, const std::string& amplitude, const std::string& low_volt, const std::string& deviation, const int& warning_flag);
    void workFinished();

public:
    void resetStopFlag() {
        stop = false;
    }

private:
    bool stop = false;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_toggled(bool checked);

    void handleResults(const std::vector<int>& sample, const std::string& freq, const std::string& amplitude, const std::string& low_volt, const std::string& deviation, const int& warn_flag);

    void handleWorkFinished();

    void on_triggerSlider_sliderMoved(int position);

    void on_trigger_textChanged();

    void on_btn500Hz_clicked();

    void on_btn250Hz_clicked();

    void on_btn1000Hz_clicked();

    void on_btn2000Hz_clicked();

    void on_btn4000Hz_clicked();

private:
    Ui::MainWindow *ui;
    ChartThread *chartThread;
    QThread *thread;
    int warning_flag;
};
#endif // MAINWINDOW_H
