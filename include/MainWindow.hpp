#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include "OrderManager.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onUpdate();

private:
    QLabel *bidLabel;
    QLabel *askLabel;
    QLabel *midLabel;
    QTimer *timer;
    OrderManager &mgr;

    QChartView *chartView;
    QChart *chart;
    QLineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
    int tickCount;
};

#endif // MAINWINDOW_HPP