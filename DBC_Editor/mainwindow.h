#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTcpServer>
#include <QTcpSocket>
#include "DBCParse.h"
#include "chart_histoWindow.h"
//#include "qcustomplot.h"

//QT_CHARTS_USE_NAMESPACE
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_bt_msg_clicked();

    void on_bt_del_lastline_clicked();

    void on_bt_signal_clicked();

    void on_bt_save_clicked();

    void on_bt_addDBC_clicked();

    void on_bt_OpenDBC_clicked();

    void on_sg_select_combo_currentIndexChanged(int index);

    void on_pushButton_mode1_clicked();

    void on_pushButton_mode2_clicked();

    void on_pushButton_close_clicked();

    void on_bt_Clear_clicked();


    //Socket관련
    void decodeCan(QString _data);
    double dcodeMsgSg(int msg_id, const char* sg_name, const QString& _data, QLCDNumber* lcd);//, QLineSeries *series);//int colindex);
    void decodeDIDO(const QString& _data);
    void on_NewConnection();
    void on_DisConnection();
    void on_ReadyRead();

    void on_bt_chart_clicked();

    void on_DI_checkBox_clicked(bool checked);

    void on_DO_checkBox_clicked(bool checked);

    void on_Address_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QTcpServer *tcpServer;
    QTcpSocket *clientSocket;
    ChartDialog *chartDialog;
    int chart_on_flag;
    bool di_flag=0;
    bool do_flag=0;
    QTimer *timer;
    QTimer *timer1;
    void updateDO();
    void updateDI();
    QString hexAddress="00";
};


#endif // MAINWINDOW_H
