#include "mesh.h"

using namespace std;

Mesh::Mesh():
    _type(triangle)
{
}

void Mesh::render(){
    int size;
    const Vertex * vertices =_vertices.constData();
    if(_vertices.size()>0){

        size = sizeof(vertices[0]);
        const Point3df * vertices_array = &(vertices[0]._point);
        const Point3df * normal_array = &(vertices[0]._normal);
        const float * color_array = vertices[0]._color;
        const unsigned short * polygons = _polygons.constData();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        {
            glNormalPointer(GL_FLOAT,size,normal_array);
            glColorPointer(4,GL_FLOAT,size,color_array);
            glVertexPointer(3,GL_FLOAT,size,vertices_array);

            switch(_type){
            case triangle :
                glDrawElements(GL_TRIANGLES,_polygons.size(),GL_UNSIGNED_SHORT,polygons);
                break;
            case lines:
                glDrawElements(GL_LINES,_polygons.size(),GL_UNSIGNED_SHORT,polygons);
                break;
            case line_strip:
                glDrawElements(GL_LINE_STRIP,_polygons.size(),GL_UNSIGNED_SHORT,polygons);
                break;
            }

        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);


//        glPushMatrix(); {
//            glBindTexture( GL_TEXTURE_2D, _texture );
//            glBegin(GL_TRIANGLES); {
//                for(int i=0; i< _polygons.size(); i++){
//                    for(int j =0; j<3; j++){
//                        if(GlobalConfig::is_enabled("texture")){
//                            switch (j){
//                            case 0:
//                                glTexCoord2f(0.0f,0.0f);
//                                break;
//                            case 1:
//                                glTexCoord2f(0.0f,0.1);
//                                break;
//                            case 2:
//                                glTexCoord2f(0.1,0.0);
//                                break;
//                            }
//                        }
//                    }
//                }
//            } glEnd();
//        } glPopMatrix();
    } else {
//        qWarning()<<"attempting to render empty mesh";
    }

}

void Mesh::loadFromOBJ(QString filepath){
    std::ifstream file(filepath.toStdString().c_str(),ios::in);
    std::string line;
    QString str;
    QStringList list;
    QVector<Point3df> buffer;
    QVector<Point3df> _temp_vertices;
    QVector<Point3df> _temp_normals;

    QVector<Point3dus> _temp_polygons;
    QVector<Point3dus> _temp_texture_polygons;
    QVector<Point3dus> _temp_normal_polygons;
    bool allocated = false;
    if (file.is_open()) {
        while (std::getline(file,line)) {
            str=QString(line.c_str());
            if(str[0]=='v')
            {
                list = str.split(" ",QString::SkipEmptyParts);
                if(str[1]==' ')
                {
                    if (list.size()==4) {
                        _temp_vertices.append(Point3df(list[1].toFloat(),list[2].toFloat(),list[3].toFloat()));
                    } else qWarning()<<"Invalid line"<<str;
                } else if(str[1]=='n') {
                    if (list.size()==4) {
                        _temp_normals.append(Point3df(list[1].toFloat(),list[2].toFloat(),list[3].toFloat()));
                    } else qWarning()<<"Invalid line"<<str;
                }
            } else if (str[0]=='f'){
                if(allocated==false){
                    allocated=true;
                    buffer=QVector<Point3df>(_temp_normals.size()+1000);
                }

                list = str.split(QRegExp("[//| ]"),QString::SkipEmptyParts);
                Point3df normal;
                Point3df verte ;
                if(line[1]==' '){
                    switch (list.size()) {
                    case 4: // Only polygons
                        _temp_polygons.append(Point3dus(list[1].toInt(),list[2].toInt(),list[4].toInt()));
//                        _temp_normal_polygons.append(Point3dus(-1,-1,-1));
//                        _temp_texture_polygons.append(Point3dus(-1,-1,-1));
                        break;
                    case 7: // Only polygons and normals
                        _temp_polygons.append(Point3dus(list[1].toInt(),list[3].toInt(),list[5].toInt()));
//                        _temp_texture_polygons.append(Point3dus(-1,-1,-1));
                        _temp_normal_polygons.append(Point3dus(list[2].toInt(),list[4].toInt(),list[6].toInt()));
                        break;
                    case 10:// Polygons and normals and textures
                        _temp_polygons.append(Point3dus(list[1].toInt(),list[4].toInt(),list[7].toInt()));
                        _temp_texture_polygons.append(Point3dus(list[2].toInt(),list[5].toInt(),list[8].toInt()));
                        _temp_normal_polygons.append(Point3dus(list[3].toInt(),list[6].toInt(),list[9].toInt()));
                        break;
                    default:
                        qWarning()<<"Invalid line"<<str;
                        break;
                    }
                } else qWarning()<<"Invalid line"<<str;
            } else if (str[0]=='#'){
//                qDebug()<<"Comment ignored"<<str;
            } else if (str.startsWith("mtllib")) {
                // load material
            } else {
//                qDebug()<<"Ignored line : "<<str;
            }
        }
        file.close();
        fillVertices(_temp_vertices,_temp_normals,_temp_polygons,_temp_texture_polygons,_temp_normal_polygons);
    }
}

void Mesh::fillVertices(const QVector<Point3df>& _temp_vertices,
                  const QVector<Point3df>& _temp_normals,
                  const QVector<Point3dus>& _temp_polygons,
                  const QVector<Point3dus>& _temp_texture_polygons,
                  const QVector<Point3dus>& _temp_normal_polygons){
    for(int i = 0;i<_temp_polygons.size();i++){
        int index[3];
        Vertex v[3];
        Point3dus polygon = _temp_polygons.at(i);
        Point3dus normal_polygon = _temp_normal_polygons.at(i);

        for(int j = 0 ; j< 3; j++){
            v[j]._point=_temp_vertices[polygon[j]-1];
            v[j]._normal=_temp_normals[normal_polygon[j]-1];
            v[j]._color[0]=v[j]._point[0];
            v[j]._color[1]=v[j]._point[1];
            v[j]._color[2]=v[j]._point[2];
            v[j]._color[3]=1;
            index[j] = _vertices.indexOf(v[j]);

            if(index[j]==-1){
                _vertices.append(v[j]);
                index[j] = _vertices.size()-1;
            } else {
                index[j]=index[j];
            }
        }
        _polygons.append(index[0]);
        _polygons.append(index[1]);
        _polygons.append(index[2]);
    }
}


void Mesh::loadFromFile(QString filepath, int filetype){
    switch (filetype){
    case obj:
        loadFromOBJ(filepath);
        break;
    default:
        break;
    }
}
