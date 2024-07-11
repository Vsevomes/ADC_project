#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setCheckable(true);
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->setTitle("Analog Signal");
    chart->createDefaultAxes();
    ui->analog_signal->setChart(chart);
    ui->analog_signal->setRenderHint(QPainter::Antialiasing);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_toggled(bool checked)
{
    if (checked){
        ui->status->setText("Идет передача данных");

        struct termios tty;
        int serial_port = open("/dev/ttyUSB0", O_RDWR);
        if (serial_port < 0) {
            std::cerr << "Ошибка при открытии UART порта" << std::endl;
            return;
        }

        init_port_uart(serial_port, tty);

        send_data(serial_port, SEND_SIG);

        std::string freq, amplitude, low_volt, deviation;
        std::vector<int> sample;
        int adc_freq;

        receive_array(serial_port, sample, adc_freq);

        ::close(serial_port);

        find_freq(sample, adc_freq, freq);
        find_min_max_volt(sample, amplitude, low_volt);
        find_deviation(sample, deviation);

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
        ui->analog_signal->setChart(chart);

        for (auto num : sample){
            std::cout << num << " ";
        }
        std::cout << std::endl;
        std::cout << freq << " " << amplitude << " " << low_volt << std::endl << std::endl;
        ui->reg_val->setText(QString::fromStdString(freq + " Гц"));
        ui->high_voltage->setText(QString::fromStdString(amplitude + " мВ"));
        ui->low_voltage->setText(QString::fromStdString(low_volt + " мВ"));
        ui->deviation->setText(QString::fromStdString(deviation + " мВ"));
    }

    else {
        ui->status->setText("Передача данных остановлена");
    }

}

