#include "viewer.h"

using namespace std;
using namespace qglviewer;



Viewer::~Viewer(){
    delete _timer_fps;
}

void Viewer::draw()
{
    drawStandard();
}

void Viewer::box(float x, float y, float z)
{
    glPushMatrix();
    glScalef(x,y,z);
    glBegin (GL_QUADS);
    //face 1
    glNormal3i(-1, 1,-1);
    glVertex3i(-1, 1,-1); glVertex3i( 1, 1,-1);
    glVertex3i( 1,-1,-1); glVertex3i(-1,-1,-1);
    //face 2
    glNormal3i(-1,-1,-1);
    glVertex3i(-1,-1,-1); glVertex3i( 1,-1,-1);
    glVertex3i( 1,-1, 1); glVertex3i(-1,-1, 1);
    // face 3
    glNormal3i( 1,-1, 1);
    glVertex3i( 1,-1, 1); glVertex3i( 1,-1,-1);
    glVertex3i( 1, 1,-1); glVertex3i( 1, 1, 1);
    //face 4
    glNormal3i( 1, 1,-1);
    glVertex3i( 1, 1,-1); glVertex3i(-1, 1,-1);
    glVertex3i(-1, 1, 1); glVertex3i( 1, 1, 1);
    //face 5
    glNormal3i(-1, 1, 1);
    glVertex3i(-1, 1, 1); glVertex3i(-1, 1,-1);
    glVertex3i(-1,-1,-1); glVertex3i(-1,-1, 1);
    //face 6
    glNormal3i( 1,-1, 1);
    glVertex3i( 1,-1, 1); glVertex3i( 1, 1, 1);
    glVertex3i(-1, 1, 1); glVertex3i(-1,-1, 1);
    glEnd();
    glPopMatrix();
}

void Viewer::computePaths(){
    QVector<MovementPath*>::iterator it = _paths.begin();
    MovementPath * path;
    while(it!=_paths.end()){
        path=*it;
        if(path->is_active()){
            path->nextPosition();
            it++;
        }
        else {
            if (path->get_type()==MovementPath::erase){
                delete(path);
                it=_paths.erase(it);
            }
            else {
                it++;
            }
        }
    }
}

void Viewer::drawStandard(){
    QGLViewer::draw();
    display2D();


    glEnable(GL_LIGHTING);

    GLfloat camerapos[4]={((float)_background_position-400)/250,1,0,1.0};
    GLfloat view[4]={0,0,0,1.0};
    if(GlobalConfig::is_enabled("display_light")){
        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex3fv(camerapos);
        glEnd();
    }
    glLightfv(GL_LIGHT0, GL_POSITION, camerapos);

    // Orientate light along view direction
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, view);

    computePaths();
    for(int i = 0; i< _3D_display_list.size(); i++)
        _3D_display_list[i]->render();

    //    computeCollisions();
    graphicInterface();
    frameEnd();


}

void Viewer::display2DObjects(){
//    float width = _viewport[2];
//    float height= _viewport[1];
    GraphicObject * _object;

    glPushMatrix(); {
        for(int i=0; i<_2D_display_list.size(); i++){
            _object = _2D_display_list[i];
        }
    } glPopMatrix();

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
    startScreenCoordinatesSystem();
    camera()->getViewport(_viewport);
    float width = _viewport[2];
    float height= _viewport[1];
    glPushMatrix();{

        GLint tex_size[2];
        glBindTexture(GL_TEXTURE_2D, _textures[4]);
        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&tex_size[0]);
        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&tex_size[1]);
        glBegin(GL_QUADS); {
            int startposition = 0;
            int i=0;
            do {
                startposition=(i-1)*tex_size[0];
                glTexCoord2f(0,0);
                glVertex2d(startposition+_background_position,            height);
                glTexCoord2f(1,0);
                glVertex2d(startposition+_background_position+tex_size[0],height);
                glTexCoord2f(1,1);
                glVertex2d(startposition+_background_position+tex_size[0],0          );
                glTexCoord2f(0,1);
                glVertex2d(startposition+_background_position,            0          );
                i++;
            } while(startposition<width);
        } glEnd();
        _background_position+=1;
        if(_background_position>tex_size[0]){
          _background_position=0;
        }
    } glPopMatrix();
    stopScreenCoordinatesSystem();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void Viewer::graphicInterface(){
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    startScreenCoordinatesSystem();
    float width = _viewport[2];
    float height= _viewport[1];
    float buttonwidth = 0;
    float buttonheight = 50;
    float offset=0;
    int buttons = _buttons.size();

    buttonwidth = 200 ;
    if(buttonwidth * buttons > width)
        buttonwidth = width / buttons ;
    offset= width/2 - (buttonwidth*buttons/2);
    buttonheight = buttonwidth / 4;

    for(int i=0;i<_buttons.size();i++){
        InterfaceButton * button = _buttons[i];
        button->set_position( QPoint(offset+i*buttonwidth, height-buttonheight));
        button->set_size(QSize(buttonwidth,                buttonheight));
    }
    displayButtons();
    stopScreenCoordinatesSystem();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}


void Viewer::displayButtons(){
    glPushMatrix(); {
        for(int i=0;i<_buttons.size();i++){
            InterfaceButton * button = _buttons[i];
            glBindTexture(GL_TEXTURE_2D, _textures[6+i]);
            glColor3f(1,1,1);

            glBegin(GL_QUADS); {
                glTexCoord2f(0,1);
                glVertex3d(button->get_position().x(),
                           button->get_position().y(),
                           0);
                glTexCoord2f(1,1);
                glVertex3d(button->get_position().x() + button->get_size().width(),
                           button->get_position().y(),
                           0);
                glTexCoord2f(1,0);
                glVertex3d(button->get_position().x() + button->get_size().width(),
                           button->get_position().y() + button->get_size().height(),
                           0);
                glTexCoord2f(0,0);
                glVertex3d(button->get_position().x(),
                           button->get_position().y() + button->get_size().height(),
                           0);
            } glEnd();
        }
    } glPopMatrix();
}

void Viewer::init()
{
    _input = InputManager::instance();
    _ui = UIState::instance();

    _timer_fps= new QTimer();
    _timer_fps->setInterval(1000);
    _timer_fps->connect(_timer_fps, SIGNAL(timeout()),this, SLOT(framepersecond()));
    _timer_fps->start();
    _frame=0;
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


void Viewer::standard(){

}



void Viewer::addTo3DDisplayList(GraphicObject3D * object){
    _3D_display_list.append(object);
}


void Viewer::addTo2DDisplayList(GraphicObject * object){
    _2D_display_list.append(object);
}

void Viewer::mousePressEvent(QMouseEvent * event)
{
    QPoint position = event->pos();
    if(GlobalConfig::is_enabled("test")){
        QGLViewer::mousePressEvent(event);
    } else {
        InterfaceButton * button;
        for(int i =0; i < _buttons.size() ; i++){
            button = _buttons[i];
            button->testClicked(position);
        }
    }

}

void Viewer::mouseMoveEvent(QMouseEvent * event){
    _input->mouseEvent(InputManager::Moved,event->pos());
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
    QGLViewer::closeEvent(event);
}
