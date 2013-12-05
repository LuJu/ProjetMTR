#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <iostream>
#include <Qt>
#include <QDateTime>
#include <QString>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <GL/gl.h>
#include "globalconfig.h"

//! Abstract class allowing the redirection of messages
/*!
        The Debugger class redirects the debug and warning messages dispatched to a file. Call the function QtInstallMsgHandler to specify the redirection
        \n Status  5 : implemented and functionnal
    */
class Debugger
{
public:
    //! Redirects the message dispatched (debug, warning...)
    /*!
        \n Status  5 : implemented and functionnal
    */
    static void customMessageHandler(QtMsgType type, const char *msg);

    //! Promps the errors that can happen with opengl functions
    /*!
        \n Status  5 : implemented and functionnal
    */
    static void promptOpenGLError();

private:
    static QString _logfile;
    //! Function to make the class not instanciable
    /*!
    */
    virtual void abstract()=0;


};

#endif // DEBUGGER_H
