#ifndef DBCPARSE_H
#define DBCPARSE_H

#include <QString>
#include <QVector>

struct Signal {

    QString name;
    int startBit;
    int bitLength;
    QString endian;
    double scale;
    double offset;
    double min;
    double max;
    QString unit;
};

struct Message {
    QString name;
    int id;
    int dl;
    QVector<Signal> signalList;
};

class DBCParse {
public:
    static QVector<Message> parseDBCToMessages(const QString &filename);
};

#endif // DBCPARSE_H
