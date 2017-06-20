#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  timer = new QTimer(this);
  initChart();
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateAction()));
  connect(ui->pushButtonGet,
          SIGNAL(clicked(bool)),
          this,
          SLOT(getData()));
  connect(ui->connectButton,
          SIGNAL(clicked(bool)),
          this,
          SLOT(connectAction()));
  connect(ui->disconnectButton,
          SIGNAL(clicked(bool)),
          this,
          SLOT(disconnectAction()));
  connect(ui->startButton,
          SIGNAL(clicked(bool)),
          this,
          SLOT(startAction()));
  connect(ui->stopButton,
          SIGNAL(clicked(bool)),
          this,
          SLOT(stopAction()));
  connect(ui->updateButton,
          SIGNAL(clicked(bool)),
          this,
          SLOT(updateAction()));
}

void MainWindow::initChart(){
    series = new QtCharts::QLineSeries();
    chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chartView = new QtCharts::QChartView(chart);
    chartView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    chartView->setRenderHint(QPainter::Antialiasing);
    axisX = new QtCharts::QValueAxis;
    axisX->setTickCount(30);
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
    axisY->setTickCount(11);
    axisY->setRange(0, 10);
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Value");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    ui->chartLayout->addWidget(chartView,0,0);
}

void MainWindow::getData(){
  QString str;
  QStringList list;
  QDateTime datetime, time;
  qDebug() << "to get data...";
  if(socket->state() == QAbstractSocket::ConnectedState){
    if(socket->isOpen()){
      qDebug() << "reading...";
      socket->write("get 127.0.0.1\r\n");
      socket->waitForBytesWritten();
      socket->waitForReadyRead();
      qDebug() << socket->bytesAvailable();
      while(socket->bytesAvailable()){
        str = socket->readLine().replace("\n","").replace("\r","");
        list = str.split(" ");
        if(list.size() == 2){
          time = datetime.fromString(list.at(0),Qt::ISODate);
          str = list.at(1);
          qDebug() << time.toSecsSinceEpoch() << ": " << str;
        }
      }
    }
  }
}

void MainWindow::connectAction()
{
  QString str;
  QStringList list;
  socket->connectToHost(ui->ipInput->text(),1234);
  if(socket->waitForConnected(3000)){
    qDebug() << "Connected";
    if(socket->isOpen()){
        socket->write("list\r\n");
        socket->waitForBytesWritten();
        socket->waitForReadyRead();
        list = QStringList();
        while(socket->bytesAvailable()){
          str = socket->readLine().replace("\n","").replace("\r","");
          list.append(str);
        }
        list.removeDuplicates();
        ui->ipDisplay->clear();
        ui->ipDisplay->addItems(list);
    }
  }
  else{
    qDebug() << "Disconnected";
  }
}

void MainWindow::disconnectAction()
{
    socket->disconnectFromHost();
    if(socket->waitForConnected(3000)){
      qDebug() << "Connected";
    }
    else{
      ui->ipDisplay->clear();
      qDebug() << "Disconnected";
    }
}

void MainWindow::startAction()
{
    int timing = ui->timingInput->value();
    if(socket->state()== QAbstractSocket::ConnectedState){
        if(timing>0) timer->start(timing*1000);
    }
}

void MainWindow::stopAction()
{
    timer->stop();
}

void MainWindow::updateAction()
{
    QString get, str;
    QStringList list;
    QDateTime datetime;
    QList<QStringList> vector, vector30;
    QList<QStringList>::iterator i;
    QList<QStringList>::reverse_iterator r;
    int value, minRange, maxRange, count;
    if(ui->ipDisplay->currentItem()){
        get = "get " + ui->ipDisplay->currentItem()->text() + "\r\n";
        QByteArray ba = get.toLatin1();
        const char *getChar = ba.data();
        if(socket->state() == QAbstractSocket::ConnectedState){
          if(socket->isOpen()){
            socket->write(getChar);
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            while(socket->bytesAvailable()){
              str = socket->readLine().replace("\n","").replace("\r","");
              list = str.split(" ");
              vector.append(list);
            }
            if (vector.length()>30) {
                count = 0;
                for (r = vector.rbegin(); r != vector.rend(); ++r) {
                    if(count<30) vector30.prepend((*r));
                    count++;
                }
            } else {
                vector30 = vector;
            }
            series->clear();
            for (i = vector30.begin(); i != vector30.end(); ++i) {
                if((*i).size() == 2){
                  datetime = datetime.fromString((*i).at(0),Qt::ISODate);
                  value = (*i).at(1).toInt();
                  series->append(datetime.toSecsSinceEpoch(), value);
                  if(i == vector30.begin()) minRange = datetime.toSecsSinceEpoch();
                }
            }
            maxRange = datetime.toSecsSinceEpoch();
            axisX->setRange(minRange, maxRange);
          }
        }
    }
}

MainWindow::~MainWindow()
{
  delete socket;
  delete ui;
}
