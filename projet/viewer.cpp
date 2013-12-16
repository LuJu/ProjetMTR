#include "viewer.h"

using namespace std;
//using namespace qglviewer;



Viewer::~Viewer(){
    delete _timer_fps;
}

void Viewer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    QMatrix4x4 V;
    _program->setUniformValue("_color",QVector3D(1,1,1));
    float scale = (((float)_ui->get_zoom())/100)+0.5;
    V.translate( 0,-5.5,1);
    V.scale(scale,scale,scale);
//    V.rotate(_ui->get_rotate().x(),1,0,0);
//    V.rotate(_ui->get_rotate().y(),0,1,0);
//    V.rotate(_ui->get_rotate().z(),0,0,1);
    V = V*_ui->_rotation;

    _ui->set_view(V);
    _program->setUniformValue("V",V);
}

void Viewer::resizeGL(int width, int height){
    QMatrix4x4 P;
    P.ortho(-1000,1000,-1000,1000,-1000,1000);
//    P.perspective(45.0f,2.0f/3.0f,0.1f,100.0f);
    _ui->set_projection(P);
    _program->setUniformValue("P",P);
    int side = qMin(width, height);
    glViewport(0,0, width, height);
//    glViewport(0, 0, width, height);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//#ifdef QT_OPENGL_ES_1
//    glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
//#else
//    glOrtho(-0.5, +0.5, -0.5, +0.5, -0.5, +0.5);
//#endif
//    glMatrixMode(GL_MODELVIEW);
}


void Viewer::startShaders(){
    if(GlobalConfig::is_enabled("shaders")) {
        _program = new QGLShaderProgram(this);

        QGLShader *vshader = compileShader("shaders/vshader.glsl",QGLShader::Vertex);
        QGLShader *fshader = compileShader("shaders/fshader.glsl",QGLShader::Fragment);
//        QGLShader *gshader = compileShader("shaders/gshader.glsl",QGLShader::Geometry);

        _program->addShader(vshader);
        _program->addShader(fshader);
//        _program->addShader(gshader);

        if(_program->link()) qDebug()<<"Link success";
        if(_program->bind()) qDebug()<<"Bind success";
    }
}

QGLShader * Viewer::compileShader(const char * path, QGLShader::ShaderType type){
    QGLShader * shader;
    QFile shaderf(GlobalConfig::find_asset_path(path));
    qDebug()<<GlobalConfig::find_asset_path(path);
    qDebug()<<shaderf.exists();
    if (shaderf.open(QIODevice::ReadOnly)){
        QByteArray qvs = shaderf.readAll();
        shaderf.close();
        shader = new QGLShader(type, this);
        shader->compileSourceCode(qvs.data());
        shaderf.close();
        return shader;
    } else {
        qCritical()<<"Error opening shader";
        return NULL;
    }
}

void Viewer::display3DObjects(){
    for(int i = 0; i< _3D_display_list.size(); i++){
        QMatrix4x4 M,V,P;
        V = _ui->get_view();
        P = _ui->get_projection();
        M = _3D_display_list[i]->get_matrix();
        _program->setUniformValue("M",_3D_display_list[i]->get_matrix());
        QMatrix4x4 pvm = P*V*M;
        _program->setUniformValue("pvm",pvm);
        _3D_display_list[i]->render();
    }
}


void Viewer::displayFullTextured(int x, int y, int width, int height){
    glBegin(GL_QUADS); {
//        glTexCoord2f(0,0);
//        glVertex2d(startposition+_background_position,            height);
//        glTexCoord2f(1,0);
//        glVertex2d(startposition+_background_position+tex_size[0],height);
//        glTexCoord2f(1,1);
//        glVertex2d(startposition+_background_position+tex_size[0],0          );
//        glTexCoord2f(0,1);
//        glVertex2d(startposition+_background_position,            0          );
    } glEnd();
}
void Viewer::display2D(){
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
//    startScreenCoordinatesSystem();
//    camera()->getViewport(_viewport);
    float width = _viewport[2];
    float height= _viewport[1];
    if(_background_activated) {
        glPushMatrix();{

            GLint tex_size[2];
            glBindTexture(GL_TEXTURE_2D, _textures[4]);
            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&tex_size[0]);
            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&tex_size[1]);
//            Mesh3d me
//            glBegin(GL_QUADS); {
//                int startposition = 0;
//                int i=0;
//                do {
//                    startposition=(i-1)*tex_size[0];
//                    glTexCoord2f(0,0);
//                    glVertex2d(startposition+_background_position,            height);
//                    glTexCoord2f(1,0);
//                    glVertex2d(startposition+_background_position+tex_size[0],height);
//                    glTexCoord2f(1,1);
//                    glVertex2d(startposition+_background_position+tex_size[0],0          );
//                    glTexCoord2f(0,1);
//                    glVertex2d(startposition+_background_position,            0          );
//                    i++;
//                } while(startposition<width);
//            } glEnd();
//            _background_position+=1;
            if(_background_position>tex_size[0]){
              _background_position=0;
            }
        } glPopMatrix();
    }
//    stopScreenCoordinatesSystem();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void Viewer::init()
{
    _input = InputManager::instance();
    _ui = UIState::instance();

    int major,minor;
    glGetIntegerv(GL_MAJOR_VERSION,&major);
    glGetIntegerv(GL_MINOR_VERSION,&minor);
    glClearColor(0.2,0.2,0.2,1);
    qDebug()<<"OPENGLVersion :"<<major<<"."<<minor;
    qDebug()<<"GLSL Version  :"<<QString(*glGetString(GL_SHADING_LANGUAGE_VERSION));
    qDebug()<<"FLAGS : "<<(int)QGLFormat::OpenGLVersionFlags();

    if(GlobalConfig::is_enabled("shaders"))
        startShaders();
    _timer_fps= new QTimer();
    _timer_fps->setInterval(1000);
    _timer_fps->connect(_timer_fps, SIGNAL(timeout()),this, SLOT(framepersecond()));
    _timer_fps->start();
    _frame=0;
    _background_activated=true;

    QTimer *timer_start = new QTimer(this);
    connect(timer_start, SIGNAL(timeout()), this, SLOT(animate()));
    timer_start->start(Timing::fps_delta_time);

    QMatrix4x4 P;
    P.ortho(-1,1,-1,1,-1,1);
    _ui->set_projection(P);
    _program->setUniformValue("P",P);
}

void Viewer::animate(){
    updateGL();
}

void Viewer::frameEnd(){
    Debugger::promptOpenGLError();
    _frame++;
    _ui->updateState();
}

void Viewer::framepersecond()
{
    if(GlobalConfig::is_enabled("output_fps"))
        qDebug()<<"FPS : "<<_frame;
     _frame=0;
}


void Viewer::loadTexture(const QString &textureName)
{
    QImage qim_Texture;
    QImage qim_TempTexture;
    qim_TempTexture.load(textureName);
    qim_Texture = QGLWidget::convertToGLFormat( qim_TempTexture );
    _textures.append(0);
    glGenTextures( 1, &_textures[_textures.size()-1] );
    glBindTexture( GL_TEXTURE_2D, _textures[_textures.size()-1] );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, qim_Texture.width(), qim_Texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qim_Texture.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void Viewer::loadTextures(QStringList &list){
    for (int i = 0; i < list.size(); ++i) {
        string asset=list.at(i).toStdString();
        loadTexture(GlobalConfig::find_asset_path(asset));
    }
}

void Viewer::addTo3DDisplayList(GraphicObject3D * object){
    _3D_display_list.append(object);
}

void Viewer::mousePressEvent(QMouseEvent * event)
{
    QPoint position = event->pos();
    updateLastMousePosition(event->pos());
}

void Viewer::mouseMoveEvent(QMouseEvent * event){
    QPoint diff = event->pos()-_last_position;
    updateLastMousePosition(event->pos());
    _input->mouseEvent(InputManager::Moved,diff);
}

void Viewer::wheelEvent(QWheelEvent * event)
{
    _input->wheelEvent(event->orientation(),event->delta(),event->pos());
}

void Viewer::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() ==  Qt::Key_Escape)
        close();
    _input->keyboardEvent(keyEvent->key(),InputManager::Key_Pressed);
}

void Viewer::keyReleaseEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() ==  Qt::Key_Escape)
        close();
    _input->keyboardEvent(keyEvent->key(),InputManager::Key_Released);
}

void Viewer::closeEvent(QCloseEvent * event){
    _ui->saveState();
    GlobalConfig::saveConfiguration();
//    QGLViewer::closeEvent(event);
}
