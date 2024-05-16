#ifndef CHART_HISTOWINDOW_H
#define CHART_HISTOWINDOW_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QVector>
#include <QPointF>
#include "qcustomplot.h"
namespace QtCharts { 
class QCharts;
}
using namespace QtCharts;

class ChartDialog : public QDialog {
    Q_OBJECT

    QTimer *timer;
public:
    ChartDialog(QWidget *parent = nullptr);
    ~ChartDialog();
    void updateChart();
    void appendTPS(double _scaledValue);
    void appendRPM(double _scaledValue);
    void appendVS(double _scaledValue);
    void appendTEMPENG(double _scaledValue);
    void appendSASANGLE(double _scaledValue);
    void replotBar(QCustomPlot* plot_b,QCPBars* histogram_,QVector<double>*raw_data,QVector<double>*category, QVector<double>*data_cnt);
private:
    QCustomPlot* plots[5]={nullptr};

    int cnt_TPS;
    int cnt_RPM;
    int cnt_VS;
    int cnt_TEMP_ENG;
    int cnt_SAS_Angle;
    int tm_cnt;
    QCustomPlot* plots_b[5]={nullptr};
    QCPBars *histogram[5]={nullptr};
    QVector<double>* bins[5]={nullptr};
    QVector<double>* bar_data[5]={nullptr};
    QVector<double>* raw_data[5]={nullptr};
};



#endif // CHART_HISTOWINDOW_H
