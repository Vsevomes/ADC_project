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
            send_data(serial_port, SEND_SIG);

            std::string freq, amplitude, low_volt, deviation;
            std::vector<int> sample;
            int adc_freq;

            receive_array(serial_port, sample, adc_freq);

            find_freq(sample, adc_freq, freq);
            find_min_max_volt(sample, amplitude, low_volt);
            find_deviation(sample, deviation);

            emit resultReady(sample, freq, amplitude, low_volt, deviation);

        }

        ::close(serial_port);
        emit workFinished();
    }

    void stopWork() {
        stop = true;
    }

signals:
    void resultReady(const std::vector<int>& sample, const std::string& freq, const std::string& amplitude, const std::string& low_volt, const std::string& deviation);
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
    void handleResults(const std::vector<int>& sample, const std::string& freq, const std::string& amplitude, const std::string& low_volt, const std::string& deviation);
    void handleWorkFinished();

    void on_triggerSlider_sliderMoved(int position);

    void on_trigger_textChanged();

private:
    Ui::MainWindow *ui;
    ChartThread *chartThread;
    QThread *thread;
};
#endif // MAINWINDOW_H
