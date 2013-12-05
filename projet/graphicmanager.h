#ifndef GRAPHICMANAGER_H
#define GRAPHICMANAGER_H

#include <QGLWidget>
#include "wgraph/wtree.h"
#include "wgraph/wnode.h"
#include "wgraph/walgorithms.h"
#include "graphicnode.h"
#include "graphicobject.h"
#include "graphicbuffer.h"
#include "core/uistate.h"
#include "graphicnode.h"
class GraphicManager
{
public:
    void paint(QGLWidget *target);
    void drawTree(QPaintDevice * target);
    GraphicManager();
    void set_ui(UIState * ui){_ui=ui;}
    void set_wtree(WTree * wtree){_wtree=wtree;}
    void set_NOI(WNode * NOI){_NOI=NOI;}
    QRect outsideRect(int x, int y, int r);
private:
    void recursiveDrawing(WNode * target, int level,QRect bounds, int levelleft, int index, int pk);
    UIState * _ui;
    WTree * _wtree;
    GraphicBuffer _buffer;
    int _width;
    int _height;
    WNode * _NOI;
//    QList<GraphicNode> _display_list;
    QList<GraphicObject> _display_list;
};

#endif // GRAPHICMANAGER_H
