#include "drawing.h"
#include "ui_drawing.h"
#include <QGraphicsTextItem>
#include <QStandardItem>





drawing::drawing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::drawing)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    QGraphicsScene * scene = new QGraphicsScene;
    QBrush dgreenbrush(Qt::darkGreen);
    QBrush dredbrush(Qt::darkRed);
    QPen blackpen(Qt::black);
    blackpen.setWidth(2);
    QRectF greenLeft,redLeft,greenRight,redRight,redMid,greenMid;

    greenLeft.setCoords(100,100,120,400);
    redLeft.setCoords(100,400,120,700);
    greenRight.setCoords(700,100,680,400);
    redRight.setCoords(700,400,680,700);

    greenMid.setCoords(395,100,405,400);
    redMid.setCoords(395,400,405,700);

    scene->addRect(greenLeft,blackpen,dgreenbrush);
    scene->addRect(redLeft,blackpen,dredbrush);
    scene->addRect(greenRight,blackpen,dgreenbrush);
    scene->addRect(redRight,blackpen,dredbrush);

    scene->addRect(greenMid,blackpen,dgreenbrush);
    scene->addRect(redMid,blackpen,dredbrush);

    QFont font=QFont("Arial", 10, QFont::Bold);
    QString str = "0.0";

    QGraphicsTextItem* item01 = new QGraphicsTextItem();
    item01->setDefaultTextColor(QColorConstants::Black);
    item01->setFont(font);
    item01->setHtml(str);
    item01->setPos(65,690);
    scene->addItem(item01);

    QGraphicsTextItem* item02 = new QGraphicsTextItem();
    item02->setDefaultTextColor(QColorConstants::Black);
    item02->setFont(font);
    item02->setHtml(str);
    item02->setPos(710,85);
    scene->addItem(item02);

    str = "1.0";

    QGraphicsTextItem* item11 = new QGraphicsTextItem();
    item11->setDefaultTextColor(QColorConstants::Black);
    item11->setFont(font);
    item11->setHtml(str);
    item11->setPos(65,85);
    scene->addItem(item11);
    QGraphicsTextItem* item12 = new QGraphicsTextItem();
    item12->setDefaultTextColor(QColorConstants::Black);
    item12->setFont(font);
    item12->setHtml(str);
    item12->setPos(710,690);
    scene->addItem(item12);

    str = "Phi+";
    QGraphicsTextItem* PhiPlus = new QGraphicsTextItem();
    PhiPlus->setDefaultTextColor(QColorConstants::Black);
    PhiPlus->setFont(font);
    PhiPlus->setHtml(str);
    PhiPlus->setPos(55,390);
    scene->addItem(PhiPlus);

    str = "Phi-";
    QGraphicsTextItem* PhiMinus = new QGraphicsTextItem();
    PhiMinus->setDefaultTextColor(QColorConstants::Black);
    PhiMinus->setFont(font);
    PhiMinus->setHtml(str);
    PhiMinus->setPos(710,390);
    scene->addItem(PhiMinus);

    ui->graphicsView->setScene(scene);
}

drawing::~drawing()
{
    delete ui;
}


