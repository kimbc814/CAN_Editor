#include "DBCParse.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

QVector<Message> DBCParse::parseDBCToMessages(const QString &filename) {
    QVector<Message> messages;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open DBC file";
        return messages;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.startsWith("BO_")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) {
                Message msg;
                msg.id = parts[1].toInt();
                msg.name = parts[2].split(":").first();
                msg.dl= parts[3].toInt();
                messages.append(msg);
            }
        } else if (line.startsWith("SG_")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 7) {
                Signal signal;
                signal.name = parts[1];
                signal.startBit = parts[3].split("|").first().toInt();
                signal.bitLength = parts[3].split("@").first().split("|").last().toInt();
                signal.endian = (parts[3].split("@").last().contains("1")?"Little":"Big");
                signal.scale = parts[4].split(",").first().split("(").last().toDouble();
                signal.offset = parts[4].split(",").last().split(")").first().toDouble();
                signal.min = parts[5].split("|").first().split("[").last().toDouble();
                signal.max = parts[5].split("|").last().split("]").first().toDouble();
                signal.unit = parts[6].mid(1,parts[6].size()-2);
                messages.last().signalList.append(signal);
            }
        }
    }

    file.close();
    return messages;
}





/*
QList<CanMessage> dbc_messages = {};
QList<CanData> l_data = {};
QList<InterpretedData> l_interpreted_data = {};

int parseDbcFile(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open DBC file" << filename;
        return -1; // 파일 열기 실패
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.startsWith("BO_ ")) {
            QStringList messageParts = line.split(" ");
            if (messageParts.size() < 5) continue; // 메시지 형식이 맞지 않으면 건너뛰기

            CanMessage message;
            message.id = messageParts[1].toInt();
            message.name = messageParts[2].trimmed();

            // 신호(SG_) 파싱 시작
            while (!in.atEnd()) {
                line = in.readLine().trimmed();
                if (!line.startsWith("SG_ ")) break; // 다음 메시지에 도달하면 중단

                QStringList signalParts = line.split(" ");
                if (signalParts.size() < 10) continue; // 신호 형식이 맞지 않으면 건너뛰기

                CanSignal signal;
                signal.name = signalParts[1];
                QStringList startBitLength = signalParts[3].split("|");
                signal.startBit = startBitLength[0].toInt();
                signal.bitLength = startBitLength[1].split("@")[0].toInt();
                signal.edian = startBitLength[1].split("@")[1].left(1); // Endian 정보 추출 ('0' 또는 '1')
                QStringList scaleOffset = signalParts[4].remove('(').remove(')').split(",");
                signal.scaleFactor = scaleOffset[0].toDouble();
                signal.offset = scaleOffset[1].toDouble();
                QStringList minMax = signalParts[5].remove('[').remove(']').split("|");
                signal.min = minMax[0].toDouble();
                signal.max = minMax[1].toDouble();
                signal.unit = signalParts[6].remove('\"'); // 단위 정보, 양쪽 따옴표 제거

                message.l_signals.append(signal);
            }

            dbc_messages.append(message);
        }
    }

    file.close();
    return 1; // 성공적으로 파일을 파싱함
}

int parseDbcFile(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open DBC file" << filename;
        return 0;
    }

    QTextStream in(&file);
    QString line = in.readLine().trimmed();
    QStringList parts = line.split(" ");
    while (!in.atEnd()) {
        CanMessage message;
        if (parts.size() > 3 && parts[0] == "BO_") {
            message.id = parts[1].toInt();
            message.name = parts[2];
            line = in.readLine().trimmed();
            parts = line.split(" ");
        }
        while(!in.atEnd()){
            if (parts.size() > 5 && parts[0] == "SG_") {
                CanSignal signal;
                signal.name = parts[1];
                signal.startBit = parts[3].split("|").first().toInt();
                signal.bitLength = parts[3].split("@").last().split("|").last().toInt();
                signal.edian = parts[3].split("@").last();
                signal.scaleFactor = parts[4].split(",").first().split("(").last().toDouble();
                signal.offset = parts[4].split(",").last().split(")").first().toDouble();
                signal.min = parts[5].split("|").first().split("[").last().toDouble();
                signal.max = parts[5].split("|").last().split("]").first().toDouble();
                signal.unit = parts[6].split(""").last().split(""").first();
                message.l_signals.append(signal);
                line = in.readLine().trimmed();
                parts = line.split(" ");
            }else break;
        }
        dbc_messages.append(message);
    }

    file.close();
    return 1;
}

int parseTrcFile(const QString &filename){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open DBC file" << filename;
        return 0;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(" ");

        CanData data;
        data.id = parts[3].toInt(nullptr,16);
        data.cdl = parts[4].toInt();
        for(int i=0;i<data.cdl;i++){
            for(int j=0;j<8;j++){
                data.data[i][j] = parts[5+i].toInt(nullptr,16)>>(7-j) & 0x01;
                //1234 5678
                //0000 0011 3
            }
        }
        l_data.append(data);
    }
    file.close();
    return 1;
}

void decodeCanData(){
    QList<CanData>::iterator it_l_data = l_data.begin();
    while (it_l_data != l_data.end()) {
        const CanData &data = *it_l_data;
        QList<CanMessage>::iterator it_dbc_messages = dbc_messages.begin();
        while (it_dbc_messages != dbc_messages.end()) {
            const CanMessage &message = *it_dbc_messages;
            int check_id =0;
            if(data.id == message.id){
                while(1){
                    if(check_id == l_interpreted_data.size()){
                        InterpretedData temp_interp_data;
                        temp_interp_data.id=data.id;
                        l_interpreted_data.append(temp_interp_data);
                        break;
                    }
                    if(l_interpreted_data.at(check_id).id == data.id){
                        break;
                    }
                    check_id++;
                }
                InterpretedData &interp_data = l_interpreted_data[check_id];
                for(int i=0;i<message.l_signals.size();i++){
                    const CanSignal &signal = message.l_signals.at(i);
                    QString tempdata;
                    QString tempdata_bin;
                    if(signal.edian.at(0) == '1'){
                        for (int j = 0; j < data.cdl; ++j) {
                            for(int k=0;k<8;k++){
                                tempdata+= QString::number(data.data[j][k]);
                            }
                        }
                    } else{
                        for (int j = data.cdl; j >0 ; --j) {
                            for(int k=0;k<8;k++){
                                tempdata+= QString::number(data.data[j][k]);
                            }
                        }
                    }
                    //문자열 쭉 깔렸으니 스타트부터 갯수개 추출후 십진수 바꿔서 연산때리고 값 도출하는 코드 작성
                    for(int j = signal.startBit; j <signal.startBit + signal.bitLength ; j++){
                        tempdata_bin = tempdata_bin +tempdata[j];
                    }
                    double decimalValue = signal.offset + (tempdata_bin.toInt(nullptr, 2)*signal.scaleFactor);
                    int check_name=0;
                    while(1){
                        if(check_name == interp_data.sg_data.size()){
                            sg_data temp_sg_data;
                            temp_sg_data.name=signal.name;
                            interp_data.sg_data.append(temp_sg_data);
                            break;
                        }
                        if(signal.name == interp_data.sg_data.at(check_name).name){
                            break;
                        }
                        check_name++;
                    }
                    interp_data.sg_data[check_name].data.append(decimalValue);
                }

            }
        }
    }
}
*/
















