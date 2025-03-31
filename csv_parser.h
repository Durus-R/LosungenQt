#ifndef CSV_PARSER_H
#define CSV_PARSER_H
#include <QFile>

class csv_parser
{
public:
    csv_parser(QFile file);
    QStringList get_date(const QDate& date) const;
private:
    QList<QStringList> data;
};

#endif // CSV_PARSER_H
