#include "CanvasScene.h"
#include "DiagramClass.h"
#include "Arrow.h"
#include <QDebug>
CanvasScene::CanvasScene(){
  this->mode = Select;
}

CanvasScene::~CanvasScene(){
    
}

void CanvasScene::setMode(Mode mode)
{
    this->mode = mode;
    qDebug("%d",mode);
}

void CanvasScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    if(this->mode==ClassDiagram){
      DiagramClass *item = new DiagramClass();
      item->setType(ClassDiagram);
      addItem(item);
      item->setPos(mouseEvent->scenePos()+QPointF(100,100));
    }
    if(this->mode==UseCaseDiagram){
      DiagramClass *item = new DiagramClass();
      item->setType(UseCaseDiagram);
      addItem(item);
      item->setPos(mouseEvent->scenePos()+QPointF(100,100));
    }
    if(this->mode==AssoLine){
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                    mouseEvent->scenePos()));
        line->setPen(QPen(Qt::blue, 2));
        addItem(line);
    }
    if(this->mode==GenLine){
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                    mouseEvent->scenePos()));
        line->setPen(QPen(Qt::blue, 2));
        addItem(line);
    }
    if(this->mode==ComLine){
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                    mouseEvent->scenePos()));
        line->setPen(QPen(Qt::blue, 2));
        addItem(line);
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CanvasScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if((this->mode==AssoLine||this->mode==ComLine||this->mode==GenLine)&& line != 0){
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else
      QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CanvasScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && (this->mode==AssoLine||this->mode==ComLine||this->mode==GenLine))
    {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        QLineF link = line->line();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramClass::Type &&
            endItems.first()->type() == DiagramClass::Type &&
            startItems.first() != endItems.first())
        {
            //qDebug()<<link.p1()<<link.p2();
            DiagramClass *startItem = qgraphicsitem_cast<DiagramClass *>(startItems.first());
            DiagramClass *endItem = qgraphicsitem_cast<DiagramClass *>(endItems.first());
            //這裡需要加上點擊位置
            QPointF point1 = link.p1();
            QPointF point2 = link.p2();
            Arrow *arrow = new Arrow(startItem, endItem, &point1, &point2);
            if(this->mode==AssoLine){
              arrow->setType(AssoLine);
            }
            if(this->mode==GenLine){
              arrow->setType(GenLine);
            }
            if(this->mode==ComLine){
              arrow->setType(ComLine);
            }
            arrow->setColor(Qt::black);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            //arrow->setZValue(-1000.0);
            addItem(arrow);
            //arrow->updatePosition();
        }
    }
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
/* vim: set ft=cpp.qt: */