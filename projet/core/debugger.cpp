#include "debugger.h"

QString Debugger::_logfile=QString("");

void Debugger::customMessageHandler(QtMsgType type, const char *msg)
{
    _logfile="logs/logfile_"+GlobalConfig::get_date_time()+".txt";
    bool output_stderr=true;
    QString txt;
    QString time = QTime::currentTime().toString();
    switch (type) {
    case QtDebugMsg:
        txt = QString("%2 Debug   : %1").arg(msg).arg(time);
        if(!GlobalConfig::is_enabled("debug_output_debug")) output_stderr=false;
    break;
    case QtWarningMsg:
        txt = QString("%2 Warning : %1").arg(msg).arg(time);
        if(!GlobalConfig::is_enabled("debug_output_warning")) output_stderr=false;
    break;
    case QtCriticalMsg:
        txt = QString("%2 Critical: %1").arg(msg).arg(time);
        if(!GlobalConfig::is_enabled("debug_output_critical")) output_stderr=false;
    break;
    case QtFatalMsg:
        txt = QString("%2 Fatal   : %1").arg(msg).arg(time);
        abort();
    }

    QFile outFile(_logfile);
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream ts(&outFile);
        ts << txt << endl;
    } else {
        int error = outFile.error();
    }
    if(output_stderr)
        fprintf(stderr, "%s\n", msg);
}


void Debugger::promptOpenGLError(){
    GLenum error=glGetError();
    QString message;
    switch (error){
        case GL_NO_ERROR:
        break;
    case GL_INVALID_ENUM:
        message="Invalid enum";
        break;
    case GL_INVALID_VALUE:
        message="Invalid value";
        break;
    case GL_INVALID_OPERATION:
        message="Invalid operation";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        message="Invalid framebuffer operation";
        break;
    case GL_OUT_OF_MEMORY:
        message="out of memory";
        break;
    case GL_STACK_UNDERFLOW:
        message="stack underflow";
        break;
    case GL_STACK_OVERFLOW:
        message="stack overflow";
        break;
    default:
        message="unknown error";
        break;
    }
    if(message!="")
        qDebug()<<"Opengl Error : "<<message;
}

