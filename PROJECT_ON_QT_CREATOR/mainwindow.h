#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QGraphicsScene>
#include <Box2D/Box2D.h>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include "random.h"



#define rand01() ( fabs(sin(rand())) )

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Circle : public QGraphicsEllipseItem
{
   public:
         Circle(b2World* world, qreal radius, QPointF initPos,bool type_of_body ,double gamma,double k);
        ~Circle();
    private:
        b2Body* body;
    public:
        virtual void advance(int phase);

};

class CircleForTr: public QGraphicsEllipseItem
{
   public:
         CircleForTr( qreal radius, QPointF initPos);
};


class GroundRect: public QGraphicsRectItem{
    public:
        GroundRect(b2World *world ,QSizeF size,QPointF initPos ,qreal angle);
        ~GroundRect();
        b2Body* body;
};

class RectForHist: public QGraphicsRectItem{
    public:
        RectForHist(QSizeF size,QPointF initPos ,qreal angle);
};


class Scene: public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(qreal x, qreal y ,qreal width,qreal height,b2World* world);
    QGraphicsView* qV;
    b2World* world;
    double lz;
    double zentr;
    double r;
    double gamma;
    double k;
    double R;
    int B_per_sek;

    double x_of_generator;
    double y_of_generator;

    double y_for_ball_out;



public slots:
    void create_body();
    void advance();
    // QGraphicsScene interface
public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    // QGraphicsScene interface
public:
    void wheelEvent(QGraphicsSceneWheelEvent *event);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
    Scene* scene;
    QTimer* frameTimer;
    b2World* world;
    QTimer* frameTimer2;    

};
#endif // MAINWINDOW_H
