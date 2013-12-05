#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <iostream>
#include <QtOpenGL>
#include <QSettings>
#include <QString>

//! Static class describing the global settings of the program
/*!
    */
class GlobalConfig
{
public:
    static QSettings _settings;
    static const std::string _assets_path;
    // returns the path of the assets of the program
    static const char * find_asset_path(std::string asset);
    static const QString find_asset_path_qstring(QString asset);
    static const QString find_option_path(QString option);
    static const QString get_date_time(){return QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");}
    static bool openGLSupport(const char * extension);
    static void saveConfiguration();
    static bool loadConfiguration();

    static bool is_enabled(QString data){return _bool["bool/"+data];}
    static void switchState(QString data);

    static void enable(QString data){  _bool["bool/"+data]=true;}
    static void disable(QString data){ _bool["bool/"+data]=false;}

    static int get_int(QString data){ return _int["int/"+data] ;}
    static void set_int(QString data, int value){_int["int/"+data]=value;}

    static QString get_string(QString data){ return _string["string/"+data] ;}
    static void set_string(QString data, QString value){ _string["string/"+data]=value;}
    static void parseArguments(int argc, char *argv[]);
private:
    virtual void abstract()=0;
    static QHash<QString,bool> _bool;
    static QHash<QString,int> _int;
    static QHash<QString,QString> _string;
};

#endif // GLOBALCONFIG_H
