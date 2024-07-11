#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include "./ui_mainwindow.h"
#include "protocol.h"
#include "function.h"

#include <QObject>

namespace Ui{
class MainWindow;
}

class ProcessThread : public QObject
{
    Q_OBJECT
public:
    explicit ProcessThread(Ui::MainWindow* ui, QObject *parent = nullptr);

public slots:
    void do_analog_sig();

signals:
    void chartReady(QtCharts::QChart* chart);

private:
    Ui::MainWindow* ui;
};

#endif // PROCESSTHREAD_H
