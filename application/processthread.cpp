#include "processthread.h"

ProcessThread::ProcessThread(Ui::MainWindow* ui, QObject *parent)
    : QObject(parent)
    , ui(ui)
{

}

void ProcessThread::do_analog_sig(){
    struct termios tty;
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Ошибка при открытии UART порта" << std::endl;
        return;
    }

    init_port_uart(serial_port, tty);

    send_data(serial_port, SEND_SIG);

    std::string freq, amplitude;
    std::vector<int> sample;

    receive_array(serial_port, sample);

    ::close(serial_port);

    calculate(sample, freq, amplitude);

    QtCharts::QSplineSeries *series = new QtCharts::QSplineSeries();
    QtCharts::QChart *chart = new QtCharts::QChart();

    float step = 1 / std::stof(freq);
    float x_value = 0;

    for (auto num : sample){
        x_value += step;
        series->append(x_value, (float)num / 4095 * 3500); // уточнить опорное напряжение
    }

    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Analog Signal");
    chart->createDefaultAxes();

    emit chartReady(chart);

    ui->analog_signal->setChart(chart);

    for (auto num : sample){
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << freq << " " << amplitude << std::endl;
    ui->reg_val->setText(QString::fromStdString(freq + " Гц"));
    ui->voltage->setText(QString::fromStdString(amplitude + " мВ"));
}
