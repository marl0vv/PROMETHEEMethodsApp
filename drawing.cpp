#include "Drawing.h"
#include "ui_drawing.h"
#include <QGraphicsTextItem>
#include <QStandardItem>
#include <mainwindow.h>



Drawing::Drawing(QWidget *parent,std::vector<Actions> a,int n) :
    QWidget(parent),
    ui(new Ui::Drawing)
{
    drawingAlternatives = a;
    drawingAlternativesAmount = n;
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    QGraphicsScene * scene = new QGraphicsScene;
    QBrush dgreenbrush(Qt::darkGreen);
    QBrush dredbrush(Qt::darkRed);
    QPen blackpen(Qt::black);
    blackpen.setWidth(2);
    QPen yellowpen(Qt::yellow);
    yellowpen.setWidth(3);
    //рисование прямоугольников - значений Phi для построения графиков
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

    QString str = "0.0"; //добавляем 0.0 возле левого и правого прямоугольников

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

    str = "1.0"; //добавляем 1.0 возле левого и правого прямоугольников

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

    str = "Phi+"; //добавляем Phi+ возле левого прямоугольника
    QGraphicsTextItem* PhiPlus = new QGraphicsTextItem();
    PhiPlus->setDefaultTextColor(QColorConstants::Black);
    PhiPlus->setFont(font);
    PhiPlus->setHtml(str);
    PhiPlus->setPos(55,390);
    scene->addItem(PhiPlus);

    str = "Phi-"; //добавляем Phi- возле правого прямоугольника
    QGraphicsTextItem* PhiMinus = new QGraphicsTextItem();
    PhiMinus->setDefaultTextColor(QColorConstants::Black);
    PhiMinus->setFont(font);
    PhiMinus->setHtml(str);
    PhiMinus->setPos(710,390);
    scene->addItem(PhiMinus);


    double php[drawingAlternativesAmount];
    double phn[drawingAlternativesAmount];
    for (int i=0; i<drawingAlternativesAmount;i++){
        php[i]=drawingAlternatives[i].getPhiPositive();
        phn[i]=1-drawingAlternatives[i].getPhiNegative();
    }
    for (int i=0; i<drawingAlternativesAmount;i++){
        scene->addLine(120,700-600*php[i],680,700-600*phn[i],yellowpen); //рисуем /
        scene->addLine(100,700-600*php[i],120,700-600*php[i],yellowpen); //рисуем _ слева
        scene->addLine(680,700-600*phn[i],700,700-600*phn[i],yellowpen); //рисуем _ справа
    }

    ui->graphicsView->setScene(scene);


}

Drawing::~Drawing()
{
    delete ui;
}

