#ifndef CHART_HISTOWINDOW_H
#define CHART_HISTOWINDOW_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QVector>
#include <QPointF>
#include <QCheckBox>
#include <QLineEdit>
#include "qcustomplot.h"
namespace QtCharts {  // 올바른 네임스페이스 사용 예
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
    void replotBar(int binSize, QCustomPlot* plot_b,QCPBars* histogram_,QVector<double>*raw_data,QVector<double>*category, QVector<double>*data_cnt);
private:
    QCustomPlot* plots[5]={nullptr};

    int cnt_TPS;
    int cnt_RPM;
    int cnt_VS;
    int cnt_TEMP_ENG;
    int cnt_SAS_Angle;
    int tm_cnt;

    QVector<QString> plot_name={"가속페달 ","RPM","속도","엔진 온도","스티어 각도"};
    QVector<QString> plot_axis={"%","RPM","Km/h","°C","deg","ms","cnt"};

    QVBoxLayout *layout_y={nullptr};
    QHBoxLayout *layout_x={nullptr};
    QHBoxLayout *layout_x1={nullptr};
    QHBoxLayout *layout_x2={nullptr};
    QHBoxLayout *layout_x3={nullptr};

    QCustomPlot* plots_b[5]={nullptr};
    QCPBars *histogram[5]={nullptr};
    QVector<double>* bins[5]={nullptr};
    QVector<double>* bar_data[5]={nullptr};
    QVector<double>* raw_data[5]={nullptr};


    void onCheckBoxToggled(bool checked);
    void onCb_histogramToggled(bool checked);
    QVBoxLayout* layout_state[5]={nullptr};
    QLineEdit* lineEdit_range_y[5]={nullptr};
    QLineEdit* lineEdit_range_x[5]={nullptr};
    QLineEdit* lineEdit_warning[5]={nullptr};
    QLineEdit* lineEdit_histogram[5]={nullptr};
    QPointF range_min[5];
    QPointF range_max[5];
    QPointF range_warning[5];
    QCPItemRect* rect_warning[5];

    int flag_warn[5]={0,};
    int flag_histogram=0;
    int histogram_binsize[5]={0,};
};



#endif // CHART_HISTOWINDOW_H
