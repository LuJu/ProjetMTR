#include "graphicmanager.h"

GraphicManager::GraphicManager()
{
    _NOI=NULL;
    _wtree=NULL;
    _ui=NULL;
    _width=_height=0;
}

void GraphicManager::drawTree(QPaintDevice * target){
    if(_NOI==NULL) _NOI = _wtree->get_root();
    int action=_ui->recordAction();
    QPoint p;
    switch (action){
    case UIState::forward :
        p = _ui->get_mouse_position();
        for (int i=0;i<_display_list.size();i++){
            GraphicObject  node = _display_list.at(i);
            if (node.contains(p)) {
//                _NOI=_wtree->get_node_by_id(node.get_id());
            }
        }
        break;
    case UIState::backward:
        if (!_NOI->is_root()){
            _NOI = _NOI->get_parent();
        }
        break;
    }
    _ui->set_level(_NOI->get_level());



    _buffer.drawRect(0,0,200,_height);
    recursiveDrawing(_NOI,1,QRect(200,10,_width-200,_height-20),3,0,0);


}

//void Drawer::draw3d(QPaintDevice * target){

//}

void GraphicManager::paint(QGLWidget * target){
    _width=target->width();
    _height=target->height();
    _buffer.begin(target);
    {
//        drawTree(target);
//        _buffer.drawStandardCircle(0,0,300,"lol");

    }
    _buffer.end();
}


void GraphicManager::recursiveDrawing(WNode * target, int level,QRect bounds, int levelleft, int index, int pk){
    QString *data = (QString*)target->get_data();
    qint16 diameter = qMin(bounds.width(),bounds.height());
    QPoint topleft(bounds.x(),bounds.y());
    QPoint newpos(bounds.x()+bounds.width()/2,bounds.y()+bounds.height()/2);
    _display_list.empty();
    int brothers=0;
    int totalindex=0;
    int ring=1;
    int k=0;
    qreal r=0.0;
    if(level>1){
        k = target->get_parent()->getK(totalindex);
        while (index >= (totalindex+k)){
            totalindex+=k;
            k = target->get_parent()->getK(totalindex);
            ring++;
        }
        brothers=target->get_parent()->get_number_of_children();
        QPointF r1r2=WAlgorithms::getSize(index-totalindex,brothers-totalindex,k);
        QPointF shift=WAlgorithms::getRelativePosition(index-totalindex,brothers-totalindex,k,r1r2.x()/ring,r1r2.y()/ring);
        shift=shift;
        r=(diameter/(ring*3/2))*r1r2.x();
        newpos=QPoint(topleft.x()+bounds.width()*shift.x(),topleft.y()+bounds.height()*shift.y());
    }
    QString message=*data;

    switch (level){
    case 1:
        r=diameter/2;

        break;
    case 3:
        r=diameter/10;
        break;
    }

    QRect newbounds(newpos.x()-r,newpos.y()-r,r*2,r*2);
    if (level <3) _buffer.drawStandardCircle(newbounds,message);
    else _buffer.drawStandardCircle(newbounds,message,1);
    if(level==2){
        QPoint topleft(newpos.x(),newpos.y());
        QPoint bottomright(newpos.x()+r,newpos.y()+r);

        _display_list.append(GraphicObject(topleft.x(),topleft.y(),QRect(topleft,bottomright)));
    }
//    if(level==2)_display_list.append(GraphicNode(newpos.x(),newpos.y(),r,level,target->get_id()));
    if (levelleft != 1){
        QList<WNode*>::const_iterator i;
        int ind=0;
        for(i = target->get_begin();i!=target->get_end();i++){
            recursiveDrawing(*i,level+1,newbounds,levelleft-1,ind,k);
            ind++;
        }
    }

}
