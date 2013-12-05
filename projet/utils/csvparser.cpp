#include "csvparser.h"

CSVParser::CSVParser()
{
}

void CSVParser::parseFile(QString path,QString split){
    QFile file(path);
    QString data;
    QStringList datas;
    if(file.open(QIODevice::ReadOnly)){
        data=file.readAll();
        datas=data.split("\n",QString::SkipEmptyParts);
        for (int i = 0; i < datas.size(); ++i) {
            append(datas[i].split(split,QString::SkipEmptyParts));
        }
        file.close();
    }
}

void CSVParser::parseFile(QString path){
    parseFile(path,",");
}
