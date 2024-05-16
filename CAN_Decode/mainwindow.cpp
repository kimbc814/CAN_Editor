#include "mainwindow.h"
#include "./ui_mainwindow.h"

//#include "lib/CAN_Access.h"
//tab1
QString g_msg_dbc="";
QString g_msg_debug="";
int g_debug_cnt=0;
int g_msg_cnt=0;
int g_sg_cnt[100]={0,};


//tab2
QString g_msg_debug1="";
QString g_msg_debug2="";
int g_debug1_cnt=0;
int g_debug2_cnt=0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),tcpServer(nullptr),
    clientSocket(nullptr)

{
    ui->setupUi(this);
    g_sg_cnt[0]=1;

    tcpServer = new QTcpServer(this);
    if(tcpServer->listen(QHostAddress::Any, 12345)) {
        connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::on_NewConnection);
    }

    ui->tabWidget->setStyleSheet("background-color: #F5F5F5;");
    ui->CAN_ID->setStyleSheet("background-color: #FFFFFF;");
    ui->msg_name->setStyleSheet("background-color: #FFFFFF;");
    ui->msg_length->setStyleSheet("background-color: #FFFFFF;");
    ui->msg_cm->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_name->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_start->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_length->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_endian->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_type->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_factor->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_offset->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_min->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_max->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_endian->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_unit->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_cm->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_unit->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_cm->setStyleSheet("background-color: #FFFFFF;");
    ui->DBC_temp->setStyleSheet("background-color: #FFFFFF;");
    ui->debug->setStyleSheet("background-color: #FFFFFF;");
    ui->txt_debug_readtab->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_table->setStyleSheet("background-color: #FFFFFF;");
    ui->msg_table->setStyleSheet("background-color: #FFFFFF;");
    ui->txt_DBCfilepath->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_select_combo->setStyleSheet("background-color: #FFFFFF;");
    ui->txt_debug_readtab_2->setStyleSheet("background-color: #FFFFFF;");
    ui->sg_select_combo_2->setStyleSheet("background-color: #FFFFFF;");
    ui->live_table->setStyleSheet("background-color: #FFFFFF;");

    ui->txt_DBCfilepath->setText("");
    ui->txt_debug_readtab->setText("");
}

MainWindow::~MainWindow()
{
    tcpServer->close();
    delete ui;
}


void MainWindow::on_bt_msg_clicked()
{

    QString CAN_id = ui->CAN_ID->toPlainText();
    QString msg_name = ui->msg_name->toPlainText();
    QString msg_len = QString::number(ui->msg_length->value());
    QString msg_cm = ui->msg_cm->toPlainText();

    g_debug_cnt++;
    if(CAN_id.isEmpty()||msg_name.isEmpty()||msg_len.isEmpty()|| g_sg_cnt[g_msg_cnt] ==0){
        g_msg_debug = g_msg_debug + QString::number(g_debug_cnt) +"  Failed to add message\n";

    } else{
        g_msg_cnt++;

        QString msg_text ="";
        if(!g_msg_dbc.isEmpty())g_msg_dbc=g_msg_dbc+"\n";
        msg_text = "BO_ "+
                   CAN_id + " " +
                   msg_name + ": " +
                   msg_len + " " +
                   msg_cm;
        g_msg_dbc = g_msg_dbc + msg_text;
        g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  Message added successfully\n";
    }
    ui->DBC_temp->setText(g_msg_dbc);
    ui->debug->setText(g_msg_debug);
    ui->debug->verticalScrollBar()->setValue(ui->debug->verticalScrollBar()->maximum());
    ui->DBC_temp->verticalScrollBar()->setValue(ui->DBC_temp->verticalScrollBar()->maximum());
}

void MainWindow::on_bt_signal_clicked()
{
    QString sg_name= ui->sg_name->toPlainText();
    QString sg_start = QString::number(ui->sg_start->value());
    QString sg_len = QString::number(ui->sg_length->value());
    QString sg_endian= ui->sg_endian->currentText();
    QString sg_type= ui->sg_type->currentText();
    QString sg_factor= ui->sg_factor->toPlainText();
    QString sg_offset= ui->sg_offset->toPlainText();
    QString sg_min= ui->sg_min->toPlainText();
    QString sg_max= ui->sg_max->toPlainText();
    QString sg_unit= ui->sg_unit->toPlainText();
    QString sg_cm= ui->sg_cm->toPlainText();

    g_debug_cnt++;
    if(sg_name.isEmpty()||sg_factor.isEmpty()||sg_offset.isEmpty()||sg_min.isEmpty() ||sg_max.isEmpty() ||sg_unit.isEmpty() || g_msg_cnt == 0){
        g_msg_debug = g_msg_debug + QString::number(g_debug_cnt) +"  Failed to add signal\n";

    } else{
        if(sg_endian == "Little"){
            sg_endian ="@1";
        } else {
            sg_endian ="@0";
        }
        if(sg_type == "Signed"){
            sg_type ="-";
        } else {
            sg_type ="+";
        }
        g_sg_cnt[g_msg_cnt]++;
        QString sg_text = "\n SG_ "+
                          sg_name + " : " +
                          sg_start + "|" +
                          sg_len +
                          sg_endian +
                          sg_type + " (" +
                          sg_factor + ","+
                          sg_offset + ") ["+
                          sg_min + "|"+
                          sg_max + "] \"" +
                          sg_cm + "\"";

        g_msg_dbc = g_msg_dbc + sg_text;

        g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  signal added Successfully\n";
    }
    ui->debug->setText(g_msg_debug);
    ui->DBC_temp->setText(g_msg_dbc);
    ui->debug->verticalScrollBar()->setValue(ui->debug->verticalScrollBar()->maximum());
    ui->DBC_temp->verticalScrollBar()->setValue(ui->DBC_temp->verticalScrollBar()->maximum());

}

void MainWindow::on_bt_del_lastline_clicked()
{
    g_debug_cnt++;
    if (!g_msg_dbc.isEmpty()) {
        int lastline_index = g_msg_dbc.lastIndexOf("\n");
        if (lastline_index != -1) {
            g_msg_dbc = g_msg_dbc.left(lastline_index);
        } else {
            g_msg_dbc.clear();
        }
        if(g_sg_cnt[g_msg_cnt] ==0){
            g_msg_cnt--;
        } else {
            g_sg_cnt[g_msg_cnt]--;
        }
        g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  Last line deleted successfully\n";
    } else {
        g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  No lines to delete\n";
    }
    ui->DBC_temp->setText(g_msg_dbc);
    ui->debug->setText(g_msg_debug);
    ui->debug->verticalScrollBar()->setValue(ui->debug->verticalScrollBar()->maximum());
    ui->DBC_temp->verticalScrollBar()->setValue(ui->DBC_temp->verticalScrollBar()->maximum());
}

void MainWindow::on_bt_save_clicked()
{
    g_debug_cnt++;
    if(g_sg_cnt[g_msg_cnt]!=0 && g_msg_cnt != 0 ){
        QString init_file_path=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString file_path = QFileDialog::getSaveFileName(this, tr("Save DBC File"), init_file_path, tr("DBC Files (*.dbc);;All Files (*)"));
        if (!file_path.isEmpty()) {
            QFile file(file_path);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << g_msg_dbc;
                file.close();
                g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  DBC file saved successfully\n";
            } else {
                g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  Failed to save DBC file\n";
            }
        } else {
            g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  No file selected for saving\n";
        }
    } else {
        g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  Syntax is not correct\n";
    }
    ui->debug->setText(g_msg_debug);
    ui->debug->verticalScrollBar()->setValue(ui->debug->verticalScrollBar()->maximum());
}


void MainWindow::on_bt_addDBC_clicked()
{
    g_debug_cnt++;
    if(g_sg_cnt[g_msg_cnt]!=0 && g_msg_cnt != 0 ){
        QString init_file_path=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString file_path = QFileDialog::getOpenFileName(this, tr("Open DBC File"), init_file_path, tr("DBC Files (*.dbc);;All Files (*)"));
        if (!file_path.isEmpty()) {
            QFile file(file_path);
            if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)) {
                QTextStream out(&file);
                out.seek(file.size());
                g_msg_dbc="\n"+g_msg_dbc;
                out << g_msg_dbc;
                file.close();
                g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  DBC file updated successfully\n";
            } else {
                g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  Failed to update DBC file\n";
            }
        } else {
            g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  No file selected for saving\n";
        }
    }else {
        g_msg_debug = g_msg_debug +QString::number(g_debug_cnt) +"  Syntax is not correct\n";
    }
    ui->debug->setText(g_msg_debug);
    ui->debug->verticalScrollBar()->setValue(ui->debug->verticalScrollBar()->maximum());
}




QVector<Message> messages ;

void MainWindow::on_bt_OpenDBC_clicked()
{
    g_debug1_cnt++;
    // 파일 탐색기창 제목
    QString str_caption = "Open File";
    QString str_dir     = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString str_filters = "DBC files (*.dbc);; All Files (*.*)";
    // 파일 저장할 폴더 경로 가져오기
    QString filePathDBC = QFileDialog::getOpenFileName(this, str_caption, str_dir, str_filters);
    QFile fileDBC(filePathDBC);
    if(!fileDBC.exists()) {    // 파일이 존재하지 않는다면
        g_msg_debug1 = g_msg_debug1 +QString::number(g_debug1_cnt) +"  Failed to Open DBC file\n";
        ui->txt_debug_readtab->setText(g_msg_debug1);
        ui->txt_debug_readtab->verticalScrollBar()->setValue(ui->txt_debug_readtab->verticalScrollBar()->maximum());
        return;
    }
    messages = DBCParse::parseDBCToMessages(filePathDBC);
    if(messages.isEmpty()){
        g_msg_debug1 = g_msg_debug1 +QString::number(g_debug1_cnt) +"  Failed to Read DBC file\n";
        ui->txt_debug_readtab->setText(g_msg_debug1);
        ui->txt_debug_readtab->verticalScrollBar()->setValue(ui->txt_debug_readtab->verticalScrollBar()->maximum());
        return;
    }
    std::sort(messages.begin(), messages.end(), [](const Message &m1, const Message &m2) {
        return m1.id < m2.id;
    });
    g_msg_debug1 = g_msg_debug1 +QString::number(g_debug1_cnt) +"  DBC Read Successfully\n";
    ui->txt_DBCfilepath->setText(filePathDBC);
    ui->txt_debug_readtab->setText(g_msg_debug1);
    ui->txt_debug_readtab->verticalScrollBar()->setValue(ui->txt_debug_readtab->verticalScrollBar()->maximum());
    ui->sg_select_combo->clear();

    ui->msg_table->setColumnCount(3);
    ui->msg_table->setHorizontalHeaderLabels(QStringList() << "Name" << "ID" << "Data Length");

    for(int i = 0; i < messages.size(); ++i) {

        QList<QStandardItem*> rowItems;
        ui->sg_select_combo->addItem(QString::number(messages[i].id));
        ui->sg_select_combo_2->addItem(QString::number(messages[i].id));

        int rowCount = ui->msg_table->rowCount();
        ui->msg_table->insertRow(rowCount);

        // 데이터 삽입
        ui->msg_table->setItem(rowCount, 0, new QTableWidgetItem(messages[i].name));
        ui->msg_table->setItem(rowCount, 1, new QTableWidgetItem(QString::number(messages[i].id)));
        ui->msg_table->setItem(rowCount, 2, new QTableWidgetItem(QString::number(messages[i].dl)));

    }

    ui->msg_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->msg_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

/*
void MainWindow::on_bt_OpenDBC_clicked()
{
    g_debug1_cnt++;
    // 파일 탐색기창 제목
    QString str_caption = "Open File";
    QString str_dir     = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString str_filters = "DBC files (*.dbc);; All Files (*.*)";
    // 파일 저장할 폴더 경로 가져오기
    QString filePathDBC = QFileDialog::getOpenFileName(this, str_caption, str_dir, str_filters);
    QFile fileDBC(filePathDBC);
    if(!fileDBC.exists()) {    // 파일이 존재하지 않는다면
        g_msg_debug1 = g_msg_debug1 +QString::number(g_debug1_cnt) +"  Failed to Open DBC file\n";
    } else {
        if(parseDbcFile(filePathDBC)){
            g_msg_debug1 = g_msg_debug1 +QString::number(g_debug1_cnt) +"  Failed to Read DBC file\n";
        } else{
            g_msg_debug1 = g_msg_debug1 +QString::number(g_debug1_cnt) +"  DBC Read Successfully\n";
            ui->txt_DBCfilepath->setText(filePathDBC);
        }
    }
    ui->txt_debug_readtab->setText(g_msg_debug1);
    ui->txt_debug_readtab->verticalScrollBar()->setValue(ui->txt_debug_readtab->verticalScrollBar()->maximum());
}
void MainWindow::on_bt_OpenData_clicked()
{
    g_debug1_cnt++;
    // 파일 탐색기창 제목
    QString str_caption = "Open File";
    QString str_dir     = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString str_filters = "DBC files (*.trc);; All Files (*.*)";
    // 파일 저장할 폴더 경로 가져오기
    QString filePathData = QFileDialog::getOpenFileName(this, str_caption, str_dir, str_filters);
    QFile fileDBC(filePathData);
    if(!fileDBC.exists()) {    // 파일이 존재하지 않는다면
        g_msg_debug1 = g_msg_debug1 +QString::number(g_debug1_cnt) +"  Failed to Open Data file\n";
    } else {
        if(parseTrcFile(filePathData)){
            g_msg_debug1 = g_msg_debug1 +QString::number(g_debug1_cnt) +"  Failed to Read Data file\n";
        } else{
            g_msg_debug1 = g_msg_debug1 +QString::number(g_debug1_cnt) +"  Data Read Successfully\n";
            ui->comboBox_msgid->clear();
            ui->comboBox_sgname->clear();
            decodeCanData();
            ui->txt_Datafilepath->setText(filePathData);
        }
    }
    for(int i=0;i<l_interpreted_data.size();i++){
        ui->comboBox_msgid->addItem(QString("Item"), QVariant::fromValue(l_interpreted_data[i].id));
        for (int j = 0; j < l_interpreted_data[i].sg_data.size(); ++j) {
            ui->comboBox_sgname->addItem(QString("Item"), QVariant::fromValue(l_interpreted_data[i].sg_data[j].name));
        }
    }





    ui->txt_debug_readtab->setText(g_msg_debug1);
    ui->txt_debug_readtab->verticalScrollBar()->setValue(ui->txt_debug_readtab->verticalScrollBar()->maximum());
}
void MainWindow::on_comboBox_sgname_currentTextChanged(const QString &arg1)
{
    QString now_msg_id = ui->comboBox_msgid->currentText();
    QString now_sg_name = ui->comboBox_sgname->currentText();
    for (int id_index = 0; id_index < l_interpreted_data.size(); ++id_index) {
        if(now_msg_id == QString::number(l_interpreted_data[id_index].id)){
            for (int name_index = 0; name_index < l_interpreted_data[id_index].sg_data.size(); ++name_index) {
                if(now_sg_name == l_interpreted_data[id_index].sg_data[name_index].name){
                    QLineSeries *series = new QLineSeries();
                    series->setName(now_sg_name);
                    for(int data_num=0;data_num<l_interpreted_data[id_index].sg_data[name_index].data.size();data_num++){
                        series->append(data_num,l_interpreted_data[id_index].sg_data[name_index].data[data_num]);
                    }
                    QChart *chart = new QChart();
                    //chart->legend()->hide();
                    chart->addSeries(series);
                    chart->setTitle(now_msg_id + " " + now_sg_name);
                    chart->createDefaultAxes();
                    QChartView *chartView = new QChartView(chart);
                    chartView->setRenderHint(QPainter::Antialiasing);
                    chartView->setGeometry(10, 320, 591, 401);
                    setCentralWidget(chartView);

                }
            }
        }

    }
}
*/


void MainWindow::on_sg_select_combo_currentIndexChanged(int index)
{
    if (index >= 0 && index < messages.size()) {
        const QVector<Signal> &signalList = messages[index].signalList;

        // 시그널 테이블 모델 초기화
        ui->sg_table->setColumnCount(9);
        ui->sg_table->setRowCount(0);
        ui->sg_table->setHorizontalHeaderLabels(QStringList() << "Name" << "Start Bit" << "Length" << "Endian" << "Scale" << "Offset" << "Min" << "Max" << "Unit");

        // 시그널 테이블에 데이터 추가
        for (int i = 0; i < signalList.size(); ++i) {
            const Signal &signal = signalList[i];
            // 행 추가
            int rowCount = ui->sg_table->rowCount();
            ui->sg_table->insertRow(rowCount);
            ui->sg_table->setItem(rowCount, 0, new QTableWidgetItem(signal.name));
            ui->sg_table->setItem(rowCount, 1, new QTableWidgetItem(QString::number(signal.startBit)));
            ui->sg_table->setItem(rowCount, 2, new QTableWidgetItem(QString::number(signal.bitLength)));
            ui->sg_table->setItem(rowCount, 3, new QTableWidgetItem(signal.endian));
            ui->sg_table->setItem(rowCount, 4, new QTableWidgetItem(QString::number(signal.scale)));
            ui->sg_table->setItem(rowCount, 5, new QTableWidgetItem(QString::number(signal.offset)));
            ui->sg_table->setItem(rowCount, 6, new QTableWidgetItem(QString::number(signal.min)));
            ui->sg_table->setItem(rowCount, 7, new QTableWidgetItem(QString::number(signal.max)));
            ui->sg_table->setItem(rowCount, 8, new QTableWidgetItem(signal.unit));
        }

        // 시그널 테이블 모델 설정

        ui->sg_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->sg_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}
int live_end_flag=0;
void MainWindow::on_pushButton_mode1_clicked()
{
    g_debug2_cnt++;
    if(live_end_flag ==1 && messages.size() > 0){
        g_msg_debug2 = g_msg_debug2 +QString::number(g_debug2_cnt) +"  Started Recieve CAN Data >> Mode : 1\n";
        ui->txt_debug_readtab_2->setText(g_msg_debug2);
        ui->txt_debug_readtab_2->verticalScrollBar()->setValue(ui->txt_debug_readtab_2->verticalScrollBar()->maximum());
        ui->live_table->setRowCount(0);
        ui->live_table->setColumnCount(messages[ui->sg_select_combo_2->currentIndex()].signalList.size());

        QStringList headerLabels;
        for (const Signal &signal : messages[ui->sg_select_combo_2->currentIndex()].signalList) {
            headerLabels <<signal.name;
        }
        ui->live_table->setHorizontalHeaderLabels(headerLabels);
        ui->live_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        if(ui->live_table->rowCount()==0){
            ui->live_table->insertRow(0);
        }
        ui->live_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        live_end_flag=2;
    }else{
        g_msg_debug2 = g_msg_debug2 +QString::number(g_debug2_cnt) +"  Failed to start >> Mode : 1\n";
        ui->txt_debug_readtab_2->setText(g_msg_debug2);
        ui->txt_debug_readtab_2->verticalScrollBar()->setValue(ui->txt_debug_readtab_2->verticalScrollBar()->maximum());
    }
}

void MainWindow::on_pushButton_mode2_clicked()
{
    g_debug2_cnt++;
    if(live_end_flag ==1 && messages.size() > 0){
        g_msg_debug2 = g_msg_debug2 +QString::number(g_debug2_cnt) +"  Started Recieve CAN Data >> Mode : 2\n";
        ui->txt_debug_readtab_2->setText(g_msg_debug2);
        ui->txt_debug_readtab_2->verticalScrollBar()->setValue(ui->txt_debug_readtab_2->verticalScrollBar()->maximum());
        ui->live_table->setRowCount(0);
        ui->live_table->setColumnCount(5);
        QStringList headerLabels;
        //for (const Signal &signal : messages[ui->sg_select_combo_2->currentIndex()].signalList) {
        //headerLabels <<signal.name;
        //headerLabels << "쓰로틀 퍼센트"<<"RPM"<<"속도"<<"엔진 온도"<<"스티어 각도";
        //headerLabels << "Throttling"<<"RPM"<<"KM/H"<<"ENG_TEMP"<<"Steering Angle";
        //headerLabels << "Throttle Position"<<"RPM"<<"Vehicle Speed"<<"Engine Temperature"<<"Steering Angle";
        headerLabels << "TPS"<<"RPM"<<"VS"<<"TEMP_ENG"<<"SAS_Angle";
        //}
        ui->live_table->setHorizontalHeaderLabels(headerLabels);
        ui->live_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        if(ui->live_table->rowCount()==0){
            ui->live_table->insertRow(0);
        }
        ui->live_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        live_end_flag=3;
    }
    else{
        g_msg_debug2 = g_msg_debug2 +QString::number(g_debug2_cnt) +"  Failed to start  >> Mode : 2\n";
        ui->txt_debug_readtab_2->setText(g_msg_debug2);
        ui->txt_debug_readtab_2->verticalScrollBar()->setValue(ui->txt_debug_readtab_2->verticalScrollBar()->maximum());
    }
}


void MainWindow::on_pushButton_close_clicked()
{
    if(live_end_flag >1){
        ui->live_table->clear();
        ui->live_table->setRowCount(0);
        ui->live_table->setColumnCount(0);
        live_end_flag=1;
        g_debug2_cnt++;
        g_msg_debug2 = g_msg_debug2 +QString::number(g_debug2_cnt) +"  Device Close Successfully\n";
        ui->txt_debug_readtab_2->setText(g_msg_debug2);
        ui->txt_debug_readtab_2->verticalScrollBar()->setValue(ui->txt_debug_readtab_2->verticalScrollBar()->maximum());
        return;
    }
    g_debug2_cnt++;
    g_msg_debug2 = g_msg_debug2 +QString::number(g_debug2_cnt) +"  Failed to Close Device\n";
    ui->txt_debug_readtab_2->setText(g_msg_debug2);
    ui->txt_debug_readtab_2->verticalScrollBar()->setValue(ui->txt_debug_readtab_2->verticalScrollBar()->maximum());
}


void MainWindow::on_bt_Clear_clicked()
{
    messages.clear();
    ui->txt_DBCfilepath->clear();
    ui->live_table->clear();
    ui->live_table->setColumnCount(0);
    ui->live_table->setRowCount(0);
    ui->sg_select_combo->clear();
    ui->sg_select_combo_2->clear();
    ui->msg_table->clear();
    ui->msg_table->setColumnCount(0);
    ui->msg_table->setRowCount(0);
    ui->sg_table->clear();
    ui->sg_table->setColumnCount(0);
    ui->sg_table->setRowCount(0);
    g_debug1_cnt++;
    g_msg_debug1 = g_msg_debug1 +QString::number(g_debug1_cnt) +"  Clear All\n";
    ui->txt_debug_readtab->setText(g_msg_debug1);
    ui->txt_debug_readtab->verticalScrollBar()->setValue(ui->txt_debug_readtab->verticalScrollBar()->maximum());
}


void MainWindow::on_NewConnection()
{
    clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::on_ReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    g_debug2_cnt++;
    g_msg_debug2 = g_msg_debug2 +QString::number(g_debug2_cnt) +"  Device Connect Successfully\n";
    ui->txt_debug_readtab_2->setText(g_msg_debug2);
    ui->txt_debug_readtab_2->verticalScrollBar()->setValue(ui->txt_debug_readtab_2->verticalScrollBar()->maximum());
    live_end_flag=1;
}
#define MASK64(nbits) ((0xffffffffffffffff)>> (64-nbits))
void MainWindow::dcodeMsgSg(int msg_id, const char* sg_name, const QString& _data,int colindex){
    QStringList parts = _data.split(' ',Qt::SkipEmptyParts);
    int ID= parts[0].toInt(nullptr,16);
    if(ID != msg_id){
        return;
    }
    int length=parts[1].toInt();
    uint8_t data[8]= {0,};
    for (int i = 0; i < length; ++i) {

        data[i] = parts[2 + i].toInt(nullptr, 16);
    }
    int msg_address=-1;
    for(int i=0;i<messages.size();i++){
        if(messages[i].id==msg_id)msg_address=i;
    }
    if(msg_address<0)return;
    int sg_address=-1;
    for(int i=0;i<messages[msg_address].signalList.size();i++){
        if(messages[msg_address].signalList[i].name==sg_name)sg_address=i;
    }
    if(sg_address<0)return;
    const Signal &signal = messages[msg_address].signalList[sg_address];
    uint8_t start_byte =signal.startBit / 8;
    uint8_t startbit_in_byte=signal.startBit % 8;
    uint8_t end_byte = 0;
    int8_t count = 0;
    uint64_t target = data[start_byte] >> startbit_in_byte;
    uint8_t current_target_length = (8-startbit_in_byte);
    if(signal.endian == "Big") // Motorola (big endian)
    {
        end_byte = (start_byte * 8 + 8 - startbit_in_byte - signal.bitLength) / 8;

        for(count = start_byte-1; count >= end_byte; count --)
        {
            target |= data[count] << current_target_length;
            current_target_length += 8;
        }
    }
    else // Intel (little endian)
    {
        end_byte = (signal.startBit+signal.bitLength) / 8;

        for(count = start_byte+1; count < end_byte; count ++)
        {
            target |= data[count] << current_target_length;
            current_target_length += 8;
        }
    }

    target &= MASK64(signal.bitLength);
    double scaledValue = ( (int64_t) target ) * signal.scale + signal.offset;
    if(strcmp(sg_name, "TPS") == 0 && scaledValue <=0.5f)scaledValue=0;
    else if(strcmp(sg_name, "SAS_Angle") == 0 && scaledValue >=600)scaledValue-=6553.5f;
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(scaledValue));
    ui->live_table->setItem(0, colindex, item);
}
void MainWindow::decodeCan(QString _data){

        QStringList parts = _data.split(' ',Qt::SkipEmptyParts);
        int ID= parts[0].toInt(nullptr,16);
        if(ID != messages[ui->sg_select_combo_2->currentIndex()].id){
            return;
        }
        int length=parts[1].toInt();
        uint8_t data[8]= {0,};

        for (int i = 0; i < length; ++i) {
            data[i] = parts[2 + i].toInt(nullptr, 16);
        }

        QVector<QVariant> rowData;
        for (const Signal &signal : messages[ui->sg_select_combo_2->currentIndex()].signalList) {
            uint8_t start_byte =signal.startBit / 8;
            uint8_t startbit_in_byte=signal.startBit % 8;
            uint8_t end_byte = 0;
            int8_t count = 0;
            uint64_t target = data[start_byte] >> startbit_in_byte;
            uint8_t current_target_length = (8-startbit_in_byte);
            if(signal.endian == "Big") // Motorola (big endian)
            {
                end_byte = (start_byte * 8 + 8 - startbit_in_byte - signal.bitLength) / 8;

                for(count = start_byte-1; count >= end_byte; count --)
                {
                    target |= data[count] << current_target_length;
                    current_target_length += 8;
                }
            }
            else // Intel (little endian)
            {
                end_byte = (signal.startBit+signal.bitLength) / 8;

                for(count = start_byte+1; count < end_byte; count ++)
                {
                    target |= data[count] << current_target_length;
                    current_target_length += 8;
                }
            }

            target &= MASK64(signal.bitLength);
            double scaledValue = ( (int64_t) target ) * signal.scale + signal.offset;
            rowData.append(scaledValue);
            for (int colIndex = 0; colIndex < rowData.size(); ++colIndex) {
                QTableWidgetItem *item = new QTableWidgetItem(rowData.at(colIndex).toString());
                ui->live_table->setItem(0, colIndex, item);
            }
        }
}
void MainWindow::on_ReadyRead()
{
    QByteArray data = clientSocket->readAll();
    if(live_end_flag ==2){
        //qDebug() << "Received:" << QString(data);
        decodeCan(data);
    }
    else if(live_end_flag == 3){
        //qDebug() << "Received:" << QString(data);
        dcodeMsgSg(809,"TPS",data,0);
        dcodeMsgSg(790,"N",data,1);
        dcodeMsgSg(790,"VS",data,2);
        dcodeMsgSg(809,"TEMP_ENG",data,3);
        dcodeMsgSg(688,"SAS_Angle",data,4);
    }
}

