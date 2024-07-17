#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chartThread(new ChartThread)
    , thread(new QThread)
    , warning_flag(0)
{
    ui->setupUi(this);
    ui->pushButton->setCheckable(true);
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->setTitle("Analog Signal");
    chart->createDefaultAxes();
    ui->analog_signal->setChart(chart);
    ui->analog_signal->setRenderHint(QPainter::Antialiasing);

    ui->triggerSlider->setRange(0, 3300);

    ui->btn500Hz->click();

    qRegisterMetaType<std::vector<int>>("std::vector<int>");
    qRegisterMetaType<std::string>("std::string");

    chartThread->moveToThread(thread);
    connect(thread, &QThread::started, chartThread, &ChartThread::doWork);
    connect(chartThread, &ChartThread::resultReady, this, &MainWindow::handleResults);
    connect(chartThread, &ChartThread::workFinished, this, &MainWindow::handleWorkFinished);
}

MainWindow::~MainWindow()
{
    chartThread->stopWork();
    thread->quit();
    thread->wait();
    delete ui;
}

void MainWindow::on_pushButton_toggled(bool checked)
{
    if (checked) {
        ui->status->setText("Идет передача данных");
        if (!thread->isRunning()) {
            chartThread->resetStopFlag();
            thread->start();
        }
    } else {
        ui->status->setText("Передача данных остановлена");
        ui->warning->setText("");
        chartThread->stopWork();
    }
}

void MainWindow::handleResults(const std::vector<int>& sample, const std::string& freq, const std::string& amplitude, const std::string& low_volt, const std::string& deviation, const int& warn_flag)
{
    std::cout << "Handling results: freq=" << freq << ", amplitude=" << amplitude << ", low_volt=" << low_volt << ", deviation=" << deviation << std::endl;

    QtCharts::QSplineSeries *series = new QtCharts::QSplineSeries();
    QtCharts::QSplineSeries *trigger_series = new QtCharts::QSplineSeries();
    QtCharts::QChart *chart = new QtCharts::QChart();

    int trigger = ui->triggerSlider->value();
    bool flag = false;
    bool flag_first_dot = true;
    bool first_warning_flag = false;
    float step = 1 / std::stof(freq);
    float x_value = 0;
    warning_flag = warn_flag;

    if (warning_flag == 1) {
        ui->warning->setText("Частота дискретизации слишком низкая!");
        first_warning_flag = true;
    }
    else if (warning_flag == 2){
        ui->warning->setText("Частота дискретизации слишком высокая!");
        first_warning_flag = true;
    }

    for (int i = 0; i < sample.size() - 1;  ++i) {

        if ((float)trigger + 50 > (float)sample[i] / 4095 * 3500 && (float)trigger - 50 < (float)sample[i] / 4095 * 3500 && sample[i + 1] > sample[i] && flag_first_dot == true){
            flag = true;
            continue;
        }

        if (flag == true){
            if (flag_first_dot == true){
                trigger_series->append(x_value, float(trigger));
                series->append(x_value, float(trigger)); // уточнить опорное напряжение
                x_value += step;
                flag_first_dot = false;
            }
            else {
                trigger_series->append(x_value, float(trigger));
                series->append(x_value, (float)sample[i] / 4095 * 3500); // уточнить опорное напряжение
                x_value += step;
            }
        }
    }

    if (flag == false && first_warning_flag == false){
        ui->warning->setText("Триггер не распознан. Измените уровень триггера.");
    }
    else if (flag == true && first_warning_flag == false) {
        ui->warning->setText("");
    }

    QPen pen(Qt::blue);
    trigger_series->setPen(pen);

    chart->addSeries(series);

    pen.setColor(Qt::darkYellow);
    pen.setStyle(Qt::DashLine);
    trigger_series->setPen(pen);

    chart->addSeries(trigger_series);

    chart->legend()->hide();
    chart->setTitle("Analog Signal");
    chart->createDefaultAxes();
    ui->analog_signal->setChart(chart);

    ui->reg_val->setText(QString::fromStdString(freq + " Гц"));
    ui->high_voltage->setText(QString::fromStdString(amplitude + " мВ"));
    ui->low_voltage->setText(QString::fromStdString(low_volt + " мВ"));
    ui->deviation->setText(QString::fromStdString(deviation + " мВ"));
}

void MainWindow::handleWorkFinished()
{
    thread->quit();
    thread->wait();
}


void MainWindow::on_triggerSlider_sliderMoved(int position)
{
    QString value;
    ui->trigger->setText(value.setNum(position));
}


void MainWindow::on_trigger_textChanged()
{
    QString value = ui->trigger->toPlainText();
    QString filteredText;
    QRegularExpression regex("[^0-9]");

    for (const QChar &ch : value) {
        if (!regex.match(ch).hasMatch()) {
            filteredText.append(ch);
        }
    }

    if (value != filteredText) {
        ui->trigger->blockSignals(true);
        ui->trigger->setPlainText(filteredText);
        ui->trigger->moveCursor(QTextCursor::End);
        ui->trigger->blockSignals(false);
    }
    else {
        ui->triggerSlider->setSliderPosition(value.toInt());
    }
}

void MainWindow::on_btn250Hz_clicked()
{
    ui->pushButton->setChecked(false);
    ui->warning->setText("");
    warning_flag = 0;
    struct termios tty;
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Ошибка при открытии UART порта" << std::endl;
        return;
    }

    init_port_uart(serial_port, tty);

    send_signal(serial_port, DIVIDER_250Hz);
    std::cout << "send sig 250 Hz" << std::endl;

    ::close(serial_port);
}

void MainWindow::on_btn500Hz_clicked()
{
    ui->pushButton->setChecked(false);
    ui->warning->setText("");
    warning_flag = 0;
    struct termios tty;
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Ошибка при открытии UART порта" << std::endl;
        return;
    }

    init_port_uart(serial_port, tty);

    send_signal(serial_port, DIVIDER_500Hz);
    std::cout << "send sig 500 Hz" << std::endl;

    ::close(serial_port);
}

void MainWindow::on_btn1000Hz_clicked()
{
    ui->pushButton->setChecked(false);
    ui->warning->setText("");
    warning_flag = 0;
    struct termios tty;
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Ошибка при открытии UART порта" << std::endl;
        return;
    }

    init_port_uart(serial_port, tty);

    send_signal(serial_port, DIVIDER_1000Hz);
    std::cout << "send sig 1000 Hz" << std::endl;

    ::close(serial_port);
}

void MainWindow::on_btn2000Hz_clicked()
{
    ui->pushButton->setChecked(false);
    ui->warning->setText("");
    warning_flag = 0;
    struct termios tty;
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Ошибка при открытии UART порта" << std::endl;
        return;
    }

    init_port_uart(serial_port, tty);

    send_signal(serial_port, DIVIDER_2000Hz);
    std::cout << "send sig 2000 Hz" << std::endl;

    ::close(serial_port);
}

void MainWindow::on_btn4000Hz_clicked()
{
    ui->pushButton->setChecked(false);
    ui->warning->setText("");
    warning_flag = 0;
    struct termios tty;
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Ошибка при открытии UART порта" << std::endl;
        return;
    }

    init_port_uart(serial_port, tty);

    send_signal(serial_port, DIVIDER_4000Hz);
    std::cout << "send sig 4000 Hz" << std::endl;

    ::close(serial_port);
}

