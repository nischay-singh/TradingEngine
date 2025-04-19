#include "MainWindow.hpp"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mgr(OrderManager::getInstance()), tickCount(0)
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *layout = new QVBoxLayout(central);
    bidLabel = new QLabel("Best Bid: --", this);
    askLabel = new QLabel("Best Ask: --", this);
    midLabel = new QLabel("Mid Price: --", this);
    layout->addWidget(bidLabel);
    layout->addWidget(askLabel);
    layout->addWidget(midLabel);

    series = new QLineSeries();
    chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();

    if (auto axesX = chart->axes(Qt::Horizontal); !axesX.isEmpty())
    {
        auto *axis = qobject_cast<QValueAxis *>(axesX.first());
        axis->setRange(0, 50);
    }

    if (auto axesY = chart->axes(Qt::Vertical); !axesY.isEmpty())
    {
        auto *axis = qobject_cast<QValueAxis *>(axesY.first());
        axis->setRange(95, 105);
    }

    chartView = new QChartView(chart);
    chartView->setMinimumHeight(200);
    layout->addWidget(chartView);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onUpdate);
    timer->start(1000);
}

MainWindow::~MainWindow() = default;

void MainWindow::onUpdate()
{
    mgr.updatePrice();
    auto data = mgr.getMarketData();
    double bestBid = data["best_bid"];
    double bestAsk = data["best_ask"];
    double midPrice = data["mid_price"];

    bidLabel->setText(QString("Best Bid: %1").arg(bestBid, 0, 'f', 2));
    askLabel->setText(QString("Best Ask: %1").arg(bestAsk, 0, 'f', 2));
    midLabel->setText(QString("Mid Price: %1").arg(midPrice, 0, 'f', 2));

    series->append(tickCount, midPrice);
    if (series->count() > 50)
    {
        series->removePoints(0, series->count() - 50);
    }

    if (auto axesX = chart->axes(Qt::Horizontal); !axesX.isEmpty())
    {
        auto *axis = qobject_cast<QValueAxis *>(axesX.first());
        axis->setRange(qMax(0, tickCount - 49), tickCount);
    }

    auto pts = series->points();
    double minY = pts.first().y(), maxY = minY;
    for (auto &pt : pts)
    {
        minY = qMin(minY, pt.y());
        maxY = qMax(maxY, pt.y());
    }
    if (auto axesY = chart->axes(Qt::Vertical); !axesY.isEmpty())
    {
        auto *axis = qobject_cast<QValueAxis *>(axesY.first());
        axis->setRange(minY, maxY);
    }

    ++tickCount;
}