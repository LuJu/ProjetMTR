#include "globalconfig.h"

QHash<QString,bool> GlobalConfig::_bool;
QHash<QString,int> GlobalConfig::_int;
QHash<QString,QString> GlobalConfig::_string;
QSettings GlobalConfig::_settings("Windel", "ETS");

const std::string GlobalConfig::_assets_path="../assets/";

const char *GlobalConfig::find_asset_path(std::string asset){
    const char * path=(_assets_path+asset).c_str();
    return path;
}

const QString GlobalConfig::find_asset_path_qstring(QString asset){
    QString path(_assets_path.c_str());
    path.append(asset);
    return path;
}

const QString GlobalConfig::find_option_path(QString option){
    QString asset = get_string(QString(option));
    return find_asset_path_qstring(asset);
}

bool GlobalConfig::openGLSupport(const char * extension){
    const char *exts = (char*)glGetString(GL_EXTENSIONS);
    QString s(exts);
    qDebug()<<"Extensions supported : "<<s;
    if(strstr((const char*)exts, extension) == NULL) {
        qDebug()<<"extension "<<extension<<" non supportee";
        return false ;
    } else
        return true;
}

void GlobalConfig::switchState(QString data){
    _bool["bool/"+data]=!_bool["bool/"+data];
    qDebug()<<"value: "<<_bool[data];
}

void GlobalConfig::saveConfiguration(){
    QHashIterator<QString, bool> i(_bool);
    while (i.hasNext()) {
        i.next();
        _settings.setValue(i.key(),i.value()?"true":"false");
    }
    QHashIterator<QString, int> j(_int);
    while (j.hasNext()) {
        j.next();
        _settings.setValue(j.key(),QString::number(j.value()));
    }
    QHashIterator<QString, QString> k(_string);
    while (k.hasNext()) {
        k.next();
        _settings.setValue(k.key(),k.value());
    }
}

bool GlobalConfig::loadConfiguration(){

    FILE *fp;
    char path[1000];

    QString option;
    QString value;
    QStringList keys;
    int value_int;
    bool ok;
    _settings.beginGroup("bool");
    keys=_settings.allKeys();

    QString command = QString("ls");
    fp = popen(command.toStdString().c_str(), "r");
    while (fgets(path, 1000, fp) != NULL){
        QString s(path);
        qDebug()<<"s : "<<s;
    }
    for (int i =0; i < keys.size() ; i++){
        option=keys[i];
        value=_settings.value(keys[i]).toString();
        if (value =="true" || value=="false")
            _bool["bool/"+option]=((value=="true"));
    }
    _settings.endGroup();

    _settings.beginGroup("int");
    keys=_settings.allKeys();
    for (int i =0; i < keys.size() ; i++){
        option=keys[i];
        value_int=_settings.value(keys[i]).toInt(&ok);
        if(ok) _int["int/"+option]=value_int;
    }
    _settings.endGroup();

    _settings.beginGroup("string");
    keys=_settings.allKeys();
    for (int i =0; i < keys.size() ; i++){
        option=keys[i];
        value=_settings.value(keys[i]).toString();
        _string["string/"+option]=value;
    }
    _settings.endGroup();

    set_int("body_mass",100);
    set_string("input_location","values.csv");

    qDebug()<<"Previous configuration loaded";
    return true;
}

void GlobalConfig::parseArguments(int argc, char *argv[]){
    int value=0;
    bool ok=false;
    for (int i = 1; i < argc; ++i) {
        QString param(argv[i]);
        qDebug()<<"param "<<i<<": "<<param;
        if(i != 1){
            value= param.toInt(&ok);
            if (!ok)
                qWarning()<<"parameter "<<i<<" invalid";
            else qWarning()<<"parameter "<<value<<" valid";
        }
        switch (i){
        case 0:
            break ;
        case 1:
            set_string("input_location",param);
            break ;
        case 2:
            set_int("body_mass",value);
            break ;
        case 3:
            set_int("duration",value);
            break ;
        case 4:
            set_int("coefficient",value);
            break ;
        case 5:
            break ;
        }
    }
}
