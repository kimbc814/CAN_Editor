#include "chart_histoWindow.h"


ChartDialog::ChartDialog(QWidget *parent)
    : QDialog(parent) {
    cnt_TPS=0;
    cnt_RPM=0;
    cnt_VS=0;
    cnt_TEMP_ENG=0;
    cnt_SAS_Angle=0;
    tm_cnt=0;
    QVBoxLayout *layout_y = new QVBoxLayout(this);
    QHBoxLayout *layout_x = new QHBoxLayout(this);
    layout_y->addLayout(layout_x);
    QHBoxLayout *layout_x1 = new QHBoxLayout(this);
    layout_y->addLayout(layout_x1);

    QVector<QString> plot_name={"쓰로틀 퍼센트","RPM","속도","엔진 온도","스티어 각도"};
    for(int i=0;i<5;i++){
        plots[i]= new QCustomPlot();
        plots[i]->addGraph();
        //plot->graph(0)->setName(plot_name[i]);
        plots[i]->graph(0)->setName(plot_name[i]);
        plots[i]->legend->setVisible(true);
        plots[i]->setFixedSize(400,300);
        plots[i]->graph(0)->setLineStyle(QCPGraph::lsLine);
        layout_x->addWidget(plots[i]);

        raw_data[i]=new QVector<double>();
        plots_b[i]= new QCustomPlot();
        plots_b[i]->setFixedSize(400,300);
        bins[i]= new QVector<double>();
        bar_data[i]= new QVector<double>();
        histogram[i] = new QCPBars(plots_b[i]->xAxis, plots_b[i]->yAxis);
        histogram[i]->setData(*bins[i], *bar_data[i]);
        plots_b[i]->rescaleAxes();
        layout_x1->addWidget(plots_b[i]);

    }
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ChartDialog::updateChart);
    timer->start(500);

}
ChartDialog::~ChartDialog(){

}
void ChartDialog::appendTPS(double _scaledValue){
    if(_scaledValue != 7777 && plots[0]!=nullptr && raw_data[0]!=nullptr){
        //qDebug()<<"TPS : "<<_scaledValue;
        plots[0]->graph(0)->addData(cnt_TPS++,_scaledValue);
        raw_data[0]->append(_scaledValue);
    }
}
void ChartDialog::appendRPM(double _scaledValue){
    if(_scaledValue != 7777 && plots[1]!=nullptr && raw_data[1]!=nullptr){
        //qDebug()<<"\t\tRPM : "<<_scaledValue;
        plots[1]->graph(0)->addData(cnt_RPM++,_scaledValue);
        raw_data[1]->append(_scaledValue);
    }
}
void ChartDialog::appendVS(double _scaledValue){
    if(_scaledValue != 7777 && plots[2]!=nullptr && raw_data[2]!=nullptr){
        //qDebug()<<"\t\t\t\tVS : "<<_scaledValue;
        plots[2]->graph(0)->addData(cnt_VS++,_scaledValue);
        raw_data[2]->append(_scaledValue);
    }
}
void ChartDialog::appendTEMPENG(double _scaledValue){
    if(_scaledValue != 7777 && plots[3]!=nullptr && raw_data[3]!=nullptr){
        //qDebug()<<"\t\t\t\t\t\tTEMP_ENG : "<<_scaledValue;
        plots[3]->graph(0)->addData(cnt_TEMP_ENG++,_scaledValue);
        raw_data[3]->append(_scaledValue);
    }
}
void ChartDialog::appendSASANGLE(double _scaledValue){
    if(_scaledValue != 7777 && plots[4]!=nullptr && raw_data[4]!=nullptr){
        //qDebug()<<"\t\t\t\t\t\t\t\tSAS_Angle : "<<_scaledValue;
        plots[4]->graph(0)->addData(cnt_SAS_Angle++,_scaledValue);
        raw_data[4]->append(_scaledValue);
    }
}

void ChartDialog::updateChart(){
    tm_cnt++;
    qDebug()<<tm_cnt;
    for(QCustomPlot* plot_:plots){
        plot_->rescaleAxes();
        plot_->replot();
    }
    if(tm_cnt%2==0){
        for(int i=0;i<5;i++){
            replotBar(plots_b[i],histogram[i],raw_data[i],bins[i],bar_data[i]);
        }
    }
}
void ChartDialog::replotBar(QCustomPlot* plot_b,QCPBars* histogram_,QVector<double>*raw_data,QVector<double>*category, QVector<double>*data_cnt){
    category->clear();
    category->shrink_to_fit();
    data_cnt->clear();
    data_cnt->shrink_to_fit();
    double maxValue = *std::max_element(raw_data->begin(), raw_data->end());
    double minValue = *std::min_element(raw_data->begin(), raw_data->end());

    int binSize;
    if (maxValue <= 100 || minValue >= -100) {
        binSize = 10;
    } else if (maxValue >= 1000 || minValue <= -1000) {
        binSize = 100;
    } else {
        binSize = 50;
    }
    int numBins_=0;
    int numBins=0;
    if(minValue<0){
        numBins_= (abs(minValue) + binSize - 1) / binSize +1;
    }
    numBins = (maxValue + binSize - 1) / binSize +1;

    QVector<int> bins(numBins_+numBins, 0);
    for (double value : *raw_data) {
        if(value<0){
            int binIndex = numBins_+ (value / binSize);
            if (binIndex >= 0 && binIndex < numBins_) {
                bins[binIndex]++;
            }
        }
        int binIndex = numBins_ + (value / binSize);
        if (binIndex >= numBins_ && binIndex < numBins+numBins_) {
            bins[binIndex]++;
        }
    }
    for (int i = 0; i < numBins_; i++) {
        int binStart = -(numBins_-i) * binSize;
        category->append(binStart);
        data_cnt->append(bins[i]);
    }
    for (int i = numBins_; i < numBins +numBins_; i++) {
        int binStart = (i-numBins_) * binSize;
        category->append(binStart);
        data_cnt->append(bins[i]);
    }
    histogram_->setData(*category,*data_cnt);
    plot_b->rescaleAxes();
    plot_b->replot();
}
