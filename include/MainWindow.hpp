#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QLineEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include "OrderManager.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onUpdate();
    void onBuyLimit();
    void onBuyMarket();
    void onSellLimit();
    void onSellMarket();

private:
    QLabel *bidLabel;
    QLabel *askLabel;
    QLabel *midLabel;
    QLineEdit *priceInput;
    QLineEdit *quantityInput;
    QTimer *timer;
    QChartView *chartView;
    QChart *chart;
    QLineSeries *series;
    QLabel *positionLabel;
    QLabel *realizedPnlLabel;
    QLabel *unrealizedPnlLabel;

    OrderManager &mgr;

    int tickCount;
    Trader trader;
};

#endif // MAINWINDOW_HPP