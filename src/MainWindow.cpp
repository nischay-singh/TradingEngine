#include "MainWindow.hpp"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QCandlestickSet>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mgr(OrderManager::getInstance()), tickCount(0), trader("GUI_USER", 100000.0)
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *layout = new QVBoxLayout(central);
    bidLabel = new QLabel("Best Bid: --", this);
    askLabel = new QLabel("Best Ask: --", this);
    midLabel = new QLabel("Mid Price: --", this);
    positionLabel = new QLabel("Position: --", this);
    realizedPnlLabel = new QLabel("Realized PnL: --", this);
    unrealizedPnlLabel = new QLabel("Unrealized PnL: --", this);

    auto *topRowLayout = new QHBoxLayout();
    auto *leftCol = new QVBoxLayout();
    auto *rightCol = new QVBoxLayout();

    leftCol->addWidget(bidLabel);
    leftCol->addWidget(askLabel);
    leftCol->addWidget(midLabel);

    rightCol->addWidget(positionLabel);
    rightCol->addWidget(realizedPnlLabel);
    rightCol->addWidget(unrealizedPnlLabel);

    topRowLayout->addLayout(leftCol);
    topRowLayout->addStretch();
    topRowLayout->addLayout(rightCol);

    layout->addLayout(topRowLayout);
    auto *controlsLayout = new QHBoxLayout();
    priceInput = new QLineEdit(this);
    priceInput->setPlaceholderText("Price");
    quantityInput = new QLineEdit(this);
    quantityInput->setPlaceholderText("Quantity");
    auto *buyLimitBtn = new QPushButton("Buy Limit", this);
    auto *buyMarketBtn = new QPushButton("Buy Market", this);
    auto *sellLimitBtn = new QPushButton("Sell Limit", this);
    auto *sellMarketBtn = new QPushButton("Sell Market", this);
    controlsLayout->addWidget(priceInput);
    controlsLayout->addWidget(quantityInput);
    controlsLayout->addWidget(buyLimitBtn);
    controlsLayout->addWidget(buyMarketBtn);
    controlsLayout->addWidget(sellLimitBtn);
    controlsLayout->addWidget(sellMarketBtn);
    layout->addLayout(controlsLayout);

    connect(buyLimitBtn, &QPushButton::clicked, this, &MainWindow::onBuyLimit);
    connect(buyMarketBtn, &QPushButton::clicked, this, &MainWindow::onBuyMarket);
    connect(sellLimitBtn, &QPushButton::clicked, this, &MainWindow::onSellLimit);
    connect(sellMarketBtn, &QPushButton::clicked, this, &MainWindow::onSellMarket);

    series = new QLineSeries();
    chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();

    if (auto axesX = chart->axes(Qt::Horizontal); !axesX.isEmpty())
    {
        auto *axis = qobject_cast<QValueAxis *>(axesX.first());
        axis->setRange(0, 50);
        axis->setLabelFormat("%d");
    }

    if (auto axesY = chart->axes(Qt::Vertical); !axesY.isEmpty())
    {
        auto *axis = qobject_cast<QValueAxis *>(axesY.first());
        axis->setRange(95, 105);
    }

    mgr.registerTrader(std::make_shared<Trader>(trader));

    chartView = new QChartView(chart);
    chartView->setMinimumHeight(200);
    layout->addWidget(chartView);

    orderBookTable = new QTableWidget(this);
    orderBookTable->setColumnCount(4);
    orderBookTable->setHorizontalHeaderLabels({"Bid Qty", "Bid Price", "Ask Price", "Ask Qty"});
    orderBookTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tradeHistoryTable = new QTableWidget(this);
    tradeHistoryTable->setColumnCount(5);
    tradeHistoryTable->setHorizontalHeaderLabels({"Side", "Type", "Qty", "Price", "Status"});

    auto *tablesLayout = new QHBoxLayout();
    tablesLayout->addWidget(orderBookTable);
    tablesLayout->addWidget(tradeHistoryTable);
    layout->addLayout(tablesLayout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onUpdate);
    timer->start(500);
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
    positionLabel->setText(QString("Position: %1").arg(trader.getCurrentPosition()));
    realizedPnlLabel->setText(QString("Realized PnL: %1").arg(trader.getRealizedPnL(), 0, 'f', 2));
    unrealizedPnlLabel->setText(QString("Unrealized PnL: %1").arg(trader.getUnrealizedPnL(midPrice), 0, 'f', 2));

    series->append(tickCount, midPrice);
    if (series->count() > 50)
    {
        series->removePoints(0, series->count() - 50);
    }

    if (auto axesX = chart->axes(Qt::Horizontal); !axesX.isEmpty())
    {
        auto *axis = qobject_cast<QValueAxis *>(axesX.first());
        axis->setRange(qMax(0, tickCount - 49), tickCount);
        axis->setLabelFormat("%d");
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

    updateOrderBookView();

    ++tickCount;
}

void MainWindow::updateOrderBookView()
{
    const auto &bidBook = mgr.getOrderBook().getBidLevels();
    const auto &askBook = mgr.getOrderBook().getAskLevels();

    int maxRows = std::max(bidBook.size(), askBook.size());
    orderBookTable->setRowCount(maxRows);

    for (int i = 0; i < maxRows; ++i)
    {
        if (i < bidBook.size())
        {
            const auto &bid = bidBook[i];
            orderBookTable->setItem(i, 0, new QTableWidgetItem(QString::number(bid.second)));
            orderBookTable->setItem(i, 1, new QTableWidgetItem(QString::number(bid.first, 'f', 2)));
        }
        else
        {
            orderBookTable->setItem(i, 0, new QTableWidgetItem(""));
            orderBookTable->setItem(i, 1, new QTableWidgetItem(""));
        }

        if (i < askBook.size())
        {
            const auto &ask = askBook[i];
            orderBookTable->setItem(i, 2, new QTableWidgetItem(QString::number(ask.first, 'f', 2)));
            orderBookTable->setItem(i, 3, new QTableWidgetItem(QString::number(ask.second)));
        }
        else
        {
            orderBookTable->setItem(i, 2, new QTableWidgetItem(""));
            orderBookTable->setItem(i, 3, new QTableWidgetItem(""));
        }
    }
}

void MainWindow::addTradeToHistory(const std::string &side, const std::string &type, const OrderExecution &execution)
{
    int row = tradeHistoryTable->rowCount();
    tradeHistoryTable->insertRow(row);

    QString sideStr = QString::fromStdString(side);
    QString typeStr = QString::fromStdString(type);

    tradeHistoryTable->setItem(row, 0, new QTableWidgetItem(sideStr));
    tradeHistoryTable->setItem(row, 1, new QTableWidgetItem(typeStr));
    tradeHistoryTable->setItem(row, 2, new QTableWidgetItem(QString::number(execution.filledQuantity)));
    tradeHistoryTable->setItem(row, 3, new QTableWidgetItem(QString::number(execution.avgPrice, 'f', 2)));
    tradeHistoryTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(execution.status)));
}

void MainWindow::onBuyLimit()
{
    double price = priceInput->text().toDouble();
    int qty = quantityInput->text().toInt();
    auto execution = trader.placeBuyLimitOrder(price, qty);

    if (execution && execution->isExecuted)
    {
        addTradeToHistory("Buy", "Limit", *execution);
        QMessageBox::information(this, "Trade Confirmation",
                                 QString("Buy Limit Executed: %1 @ %2").arg(execution->filledQuantity).arg(execution->avgPrice));
    }
}

void MainWindow::onBuyMarket()
{
    int qty = quantityInput->text().toInt();
    auto execution = trader.placeBuyMarketOrder(qty);

    if (execution && execution->isExecuted)
    {
        addTradeToHistory("Buy", "Market", *execution);
        QMessageBox::information(this, "Trade Confirmation",
                                 QString("Buy Market Executed: %1 @ %2").arg(execution->filledQuantity).arg(execution->avgPrice));
    }
}

void MainWindow::onSellLimit()
{
    double price = priceInput->text().toDouble();
    int qty = quantityInput->text().toInt();
    auto execution = trader.placeSellLimitOrder(price, qty);

    if (execution && execution->isExecuted)
    {
        addTradeToHistory("Sell", "Limit", *execution);
        QMessageBox::information(this, "Trade Confirmation",
                                 QString("Sell Limit Executed: %1 @ %2").arg(execution->filledQuantity).arg(execution->avgPrice));
    }
}

void MainWindow::onSellMarket()
{
    int qty = quantityInput->text().toInt();
    auto execution = trader.placeSellMarketOrder(qty);
    if (execution && execution->isExecuted)
    {
        addTradeToHistory("Sell", "Market", *execution);
        QMessageBox::information(this, "Trade Confirmation",
                                 QString("Sell Market Executed: %1 @ %2").arg(execution->filledQuantity).arg(execution->avgPrice));
    }
}