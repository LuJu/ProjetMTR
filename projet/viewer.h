#ifndef VIEWER_H
#define VIEWER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <qgl.h>
#include <QtOpenGL>
#include <QGLWidget>
#include <QGLShaderProgram>
#include <QGLShader>
#include <QVector3D>
#include <QMatrix2x2>

//#include <QGLViewer/qglviewer.h>
//#include <QGLViewer/vec.h>

#include <string>
#include <iostream>
#include "core/core.h"



#include "3d/graphicobject3d.h"

#include "GL/glext.h"
#include "btBulletDynamicsCommon.h"

class Viewer : public QGLWidget
{
    Q_OBJECT
public:
    Viewer(const QGLFormat &format) : QGLWidget(format){__build();}
    Viewer() : QGLWidget(){__build();}
    ~Viewer();

public slots :
void framepersecond();
void animate();


protected :
    virtual void draw();
    virtual void init();

    virtual void mousePressEvent(QMouseEvent * mouseEvent);
    virtual void wheelEvent(QWheelEvent * wheelEvent);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual void keyReleaseEvent(QKeyEvent *keyEvent);
    virtual void closeEvent(QCloseEvent * event);

    virtual void addTo3DDisplayList(GraphicObject3D * object);
    virtual void display3DObjects();

    //! Draw the background
    void display2D();
    void displayFullTextured(int x, int y, int width, int height);

    void loadTexture(const QString &textureName);
    void loadTextures(QStringList &list);
    void startShaders();
    virtual void resizeGL(int width, int height);


    //!Function called at the end of each frame
    void frameEnd();

    void paintGL(){draw();}


    QGLShaderProgram * _program;
    QVector<GLuint> _textures;

    //! List of 3D objects on the scene. Only the objects on this array will be rendered
    QVector<GraphicObject3D * > _3D_display_list;

    UIState * _ui;
    InputManager * _input;

    //! The count of frames passed since the beginning of the last second
    int _frame;

    //! Timer used for FPS count
    QTimer * _timer_fps;
    int _background_position;
    bool _background_activated;

private :
    GLint _viewport[4];
    QGLShader * compileShader(const char * path, QGLShader::ShaderType type);
    void initializeGL(){init();}
    void __build(){/*init();*/}

    QPoint _last_position;
    void updateLastMousePosition(const QPoint& position){
        _last_position = position;
    }

};
#endif // VIEWER_H
