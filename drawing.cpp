#include "drawing.h"
#include "ui_drawing.h"
#include <QGraphicsTextItem>
#include <QStandardItem>
#include <mainwindow.h>



Drawing::Drawing(QWidget *parent,std::vector<Actions> &actions,int &count) :
    QWidget(parent),
    ui(new Ui::Drawing)
{
    m_drawingAlternatives = actions;
    m_drawingAlternativesAmount = count;
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    QGraphicsScene * scene = new QGraphicsScene;
    // Кисти для заливки прямоугольников, отражающих значения phi
    QBrush darkGreenBrush(Qt::darkGreen);
    QBrush darkRedBrush(Qt::darkRed);
    // Перо для границ прямоугольников, отражающих значения phi
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    //Перо для рисования графиков альтернатив
    QPen yellowPen(Qt::yellow);
    yellowPen.setWidth(3);
    //рисование прямоугольников - значений Phi для построения графиков
    QRectF greenLeft,redLeft,greenRight,redRight,redMid,greenMid;

    greenLeft.setCoords(100,100,120,400);
    redLeft.setCoords(100,400,120,700);
    greenRight.setCoords(700,100,680,400);
    redRight.setCoords(700,400,680,700);

    greenMid.setCoords(395,100,405,400);
    redMid.setCoords(395,400,405,700);

    scene->addRect(greenLeft,blackPen,darkGreenBrush);
    scene->addRect(redLeft,blackPen,darkRedBrush);
    scene->addRect(greenRight,blackPen,darkGreenBrush);
    scene->addRect(redRight,blackPen,darkRedBrush);

    scene->addRect(greenMid,blackPen,darkGreenBrush);
    scene->addRect(redMid,blackPen,darkRedBrush);

    QFont font=QFont("Arial", 10, QFont::Bold);

    QString str = "0.0"; //добавляем 0.0 возле левого и правого прямоугольников

    QGraphicsTextItem* zeroLabelLeft = new QGraphicsTextItem();
    zeroLabelLeft->setDefaultTextColor(QColorConstants::Black);
    zeroLabelLeft->setFont(font);
    zeroLabelLeft->setHtml(str);
    zeroLabelLeft->setPos(65,690);
    scene->addItem(zeroLabelLeft);
    QGraphicsTextItem* zeroLabelRight = new QGraphicsTextItem();
    zeroLabelRight ->setDefaultTextColor(QColorConstants::Black);
    zeroLabelRight ->setFont(font);
    zeroLabelRight ->setHtml(str);
    zeroLabelRight ->setPos(710,85);
    scene->addItem(zeroLabelRight );

    str = "1.0"; //добавляем 1.0 возле левого и правого прямоугольников

    QGraphicsTextItem* oneLabelLeft = new QGraphicsTextItem();
    oneLabelLeft->setDefaultTextColor(QColorConstants::Black);
    oneLabelLeft->setFont(font);
    oneLabelLeft->setHtml(str);
    oneLabelLeft->setPos(65,85);
    scene->addItem(oneLabelLeft);
    QGraphicsTextItem* oneLabelRight = new QGraphicsTextItem();
    oneLabelRight->setDefaultTextColor(QColorConstants::Black);
    oneLabelRight->setFont(font);
    oneLabelRight->setHtml(str);
    oneLabelRight->setPos(710,690);
    scene->addItem(oneLabelRight);

    str = "Phi+"; //добавляем Phi+ возле левого прямоугольника
    QGraphicsTextItem* phiPositiveLabel = new QGraphicsTextItem();
    phiPositiveLabel->setDefaultTextColor(QColorConstants::Black);
    phiPositiveLabel->setFont(font);
    phiPositiveLabel->setHtml(str);
    phiPositiveLabel->setPos(55,390);
    scene->addItem(phiPositiveLabel);

    str = "Phi-"; //добавляем Phi- возле правого прямоугольника
    QGraphicsTextItem* phiNegativeLabel = new QGraphicsTextItem();
    phiNegativeLabel->setDefaultTextColor(QColorConstants::Black);
    phiNegativeLabel->setFont(font);
    phiNegativeLabel->setHtml(str);
    phiNegativeLabel->setPos(710,390);
    scene->addItem(phiNegativeLabel);


    double phiPositiveArray[m_drawingAlternativesAmount];
    double phiNegativeArray[m_drawingAlternativesAmount];
    // заполняем массивы значениями Phi альтернатив
    for (int i=0; i<m_drawingAlternativesAmount;i++)
    {
        phiPositiveArray[i]=m_drawingAlternatives[i].getPhiPositive();
        // вычитаем значение для корректного построения,
        // поскольку значения справа для Phi- идут от 0 до 1 сверху вниз,
        // и Phi не может быть больше 1, вычитаем Phi-
        // из единицы для получения противоположного значения
        phiNegativeArray[i]=1-m_drawingAlternatives[i].getPhiNegative();
    }
    for (int i=0; i<m_drawingAlternativesAmount;i++){
        // рисуем диагональ
        // 700-660*Phi т.к. y в нижней точке поля для рисования равен 800, прямоугольники высотой 600,
        // снизу и сверху от прямоугольников отступ равен 100. При Phi = 1 значение совпадает с верхней
        // частью прямоугольника, при Phi = 0 - с нижней.
        scene->addLine(120,700-600*phiPositiveArray[i],680,700-600*phiNegativeArray[i],yellowPen);
        scene->addLine(100,700-600*phiPositiveArray[i],120,700-600*phiPositiveArray[i],yellowPen); //рисуем _ слева
        scene->addLine(680,700-600*phiNegativeArray[i],700,700-600*phiNegativeArray[i],yellowPen); //рисуем _ справа
    }

    ui->graphicsView->setScene(scene);


}

Drawing::~Drawing()
{
    delete ui;
}

