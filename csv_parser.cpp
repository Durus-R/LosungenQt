#include "csv_parser.h"
#include <QFile>
#include <QDate>
#include <qdebug.h>

csv_parser::csv_parser(QFile file) {
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << file.errorString();
    }

    data.clear();
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        data += str.split("\t");
    }
}

QStringList csv_parser::get_date(const QDate& date) const  {
    QStringList result;
    for (int i = 0; i < data.size(); i++) {
        if (data[i][0] == date.toString("dd.MM.yyyy")) {
            result = data[i];
            break;
        }
    }
    return result;
}
