#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void getData();
  void connectAction();
  void disconnectAction();
  void startAction();
  void stopAction();
  void updateAction();
  void initChart();
private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  QTimer *timer;
  QtCharts::QLineSeries *series;
  QtCharts::QChart *chart;
  QtCharts::QChartView *chartView;
  QtCharts::QValueAxis *axisX;
};

#endif // MAINWINDOW_H
