#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QFile>

class CSVParser : public QList<QStringList>
{
public:
    explicit CSVParser();
    void parseFile(QString path, QString split);
    void parseFile(QString path);
};

#endif // CSVPARSER_H
