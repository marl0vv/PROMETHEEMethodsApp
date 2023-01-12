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
    QGraphicsScene * scene2 = new QGraphicsScene;
    // Кисти для заливки прямоугольников, отражающих значения phi
    QBrush darkGreenBrush(Qt::darkGreen);
    QBrush darkRedBrush(Qt::darkRed);
    // Перо для границ прямоугольников, отражающих значения phi
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    //Перо для рисования графиков альтернатив
    QPen yellowPen(Qt::yellow);
    yellowPen.setWidth(3);
    QBrush yellowBrush(Qt::yellow);
    //рисование прямоугольников - значений Phi для построения графиков
    QRectF greenLeft,redLeft,greenRight,redRight,redMid,greenMid;

    greenLeft.setCoords(m_phiPosLeftEdge,100,m_phiPosRightEdge,400);
    redLeft.setCoords(m_phiPosLeftEdge,400,m_phiPosRightEdge,700);
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
    zeroLabelLeft->setPos(65,710);
    scene->addItem(zeroLabelLeft);
    QGraphicsTextItem* zeroLabelRight = new QGraphicsTextItem();
    zeroLabelRight ->setDefaultTextColor(QColorConstants::Black);
    zeroLabelRight ->setFont(font);
    zeroLabelRight ->setHtml(str);
    zeroLabelRight ->setPos(710,65);
    scene->addItem(zeroLabelRight );

    str = "1.0"; //добавляем 1.0 возле левого и правого прямоугольников

    QGraphicsTextItem* oneLabelLeft = new QGraphicsTextItem();
    oneLabelLeft->setDefaultTextColor(QColorConstants::Black);
    oneLabelLeft->setFont(font);
    oneLabelLeft->setHtml(str);
    oneLabelLeft->setPos(65,65);
    scene->addItem(oneLabelLeft);
    QGraphicsTextItem* oneLabelRight = new QGraphicsTextItem();
    oneLabelRight->setDefaultTextColor(QColorConstants::Black);
    oneLabelRight->setFont(font);
    oneLabelRight->setHtml(str);
    oneLabelRight->setPos(710,710);
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
    double phiArray[m_drawingAlternativesAmount];
    // заполняем массивы значениями Phi альтернатив
    for (int i=0; i<m_drawingAlternativesAmount;i++)
    {
        phiPositiveArray[i]=m_drawingAlternatives[i].getPhiPositive();
        // вычитаем значение для корректного построения,
        // поскольку значения справа для Phi- идут от 0 до 1 сверху вниз,
        // и Phi не может быть больше 1, вычитаем Phi-
        // из единицы для получения противоположного значения
        phiNegativeArray[i]=1-m_drawingAlternatives[i].getPhiNegative();
        phiArray[i]=m_drawingAlternatives[i].getPhi();
    }

    double rectPositiveY;
    double rectNegativeY;
    QFont nametagFont=QFont("Arial", 8);

    for (int i=0; i<m_drawingAlternativesAmount;i++){
        // рисуем диагональ
        // 700-660*Phi т.к. y в нижней точке поля для рисования равен 800, прямоугольники высотой 600,
        // снизу и сверху от прямоугольников отступ равен 100. При Phi = 1 значение совпадает с верхней
        // частью прямоугольника, при Phi = 0 - с нижней.
        rectPositiveY = (m_windowHeight - m_verticalIndent) - m_phiRectangleHeight * phiPositiveArray[i];
        rectNegativeY = (m_windowHeight - m_verticalIndent) - m_phiRectangleHeight * phiNegativeArray[i];
        scene->addLine(m_phiPosRightEdge, rectPositiveY, m_phiNegLeftEdge, rectNegativeY, yellowPen);
        scene->addLine(m_phiPosLeftEdge, rectPositiveY, m_phiPosRightEdge, rectPositiveY, yellowPen); //рисуем _ слева
        scene->addLine(m_phiNegLeftEdge, rectNegativeY, m_phiNegRightEdge, rectNegativeY, yellowPen); //рисуем _ справа

        // рисуем названия альтернатив

        QString name = m_drawingAlternatives[i].getName();
        int nameLength = name.length();

        //прямоугольники для выделения названий
        QRectF *highlightRectLeft = new QRectF;
        QRectF *highlightRectRight = new QRectF;
        // отступы 5, 8 - символические, для того чтобы название
        // красиво умещалось в прямоугольнике по вертикали. Для рассчета ширины прямоугольника
        // умножаем длину названия на 7 (по 7 пунктов на каждый символ, чтобы название точно поместилось в прямоугольник)
        highlightRectLeft->setCoords(m_phiPosLeftEdge-nameLength*7,rectPositiveY-8,m_phiPosLeftEdge-5,rectPositiveY+8);
        scene->addRect(*highlightRectLeft,yellowPen,yellowBrush);
        highlightRectRight->setCoords(m_phiNegRightEdge+nameLength*7,rectNegativeY-8,m_phiNegRightEdge+5,rectNegativeY+8);
        scene->addRect(*highlightRectRight,yellowPen,yellowBrush);
        delete(highlightRectLeft);
        delete(highlightRectRight);
        // добавляем сами названия
        QGraphicsTextItem* alernativeNametagLeft = new QGraphicsTextItem();
        alernativeNametagLeft->setDefaultTextColor(QColorConstants::Black);
        alernativeNametagLeft->setFont(nametagFont);
        alernativeNametagLeft->setHtml(name);
        alernativeNametagLeft->setPos(m_phiPosLeftEdge-nameLength*7,rectPositiveY-7);
        scene->addItem(alernativeNametagLeft);
        QGraphicsTextItem* alernativeNametagRight = new QGraphicsTextItem();
        alernativeNametagRight->setDefaultTextColor(QColorConstants::Black);
        alernativeNametagRight->setFont(nametagFont);
        alernativeNametagRight->setHtml(name);
        alernativeNametagRight->setPos(m_phiNegRightEdge,rectNegativeY-7);
        scene->addItem(alernativeNametagRight);
    }

    ui->graphicsView->setScene(scene);

    //рисуем второй график
    scene2->addRect(greenMid,blackPen,darkGreenBrush);
    scene2->addRect(redMid,blackPen,darkRedBrush);
    str = "+1.0";
    QGraphicsTextItem* positiveOneLabel = new QGraphicsTextItem();
    positiveOneLabel->setDefaultTextColor(QColorConstants::Black);
    positiveOneLabel->setFont(font);
    positiveOneLabel->setHtml(str);
    positiveOneLabel->setPos(380,75);
    scene2->addItem(positiveOneLabel);
    str = "-1.0";
    QGraphicsTextItem* negativeOneLabel = new QGraphicsTextItem();
    negativeOneLabel->setDefaultTextColor(QColorConstants::Black);
    negativeOneLabel->setFont(font);
    negativeOneLabel->setHtml(str);
    negativeOneLabel->setPos(380,700);
    scene2->addItem(negativeOneLabel);
    str = "0.0";
    QGraphicsTextItem* zeroLabel = new QGraphicsTextItem();
    zeroLabel->setDefaultTextColor(QColorConstants::Black);
    zeroLabel->setFont(font);
    zeroLabel->setHtml(str);
    zeroLabel->setPos(365,385);
    scene2->addItem(zeroLabel);
    int graphY = 0; // переменная для вычисления "высоты", на которой будет строиться прямая
    for (int i=0; i<m_drawingAlternativesAmount;i++){
        if (phiArray[i] < 0){ //если Фи отрицательное
            phiArray[i] *= -1; // делаем его положительным
            //но строим в нижней (красной) половине отрезка
            //            600/2 = 300       + отступ (100)     +         300            * Фи, при Фи равном -1 рисуем в точке 700 (низ отрезка)
            graphY = m_phiRectangleHeight/2 + m_verticalIndent + m_phiRectangleHeight/2 * phiArray[i];
        } // иначе рисуем в верхней (зеленой) половине отрезка
     // середина отрезка 800/2 = 400   -       300              * Фи, при Фи равном 1 рисуем в точке с Y=100 (верх отрезка)
        else graphY = m_windowHeight/2 - m_phiRectangleHeight/2 * phiArray[i];
        scene2->addLine(300, graphY, 500, graphY, yellowPen);

        // рисуем названия альтернатив

        QString name = m_drawingAlternatives[i].getName();
        int nameLength = name.length();
        QString phi = QString::number(m_drawingAlternatives[i].getPhi());
        int phiLength = phi.length();

        //прямоугольники для выделения
        QRectF *highlightRectPhi = new QRectF;
        QRectF *highlightRectName = new QRectF;
        highlightRectPhi->setCoords(300-phiLength*7,graphY-8,300,graphY+8);
        scene2->addRect(*highlightRectPhi,yellowPen,yellowBrush);
        highlightRectName->setCoords(500+nameLength*7,graphY-8,500,graphY+8);
        scene2->addRect(*highlightRectName,yellowPen,yellowBrush);
        delete(highlightRectPhi);
        delete(highlightRectName);


        QGraphicsTextItem* phiNametag = new QGraphicsTextItem();
        phiNametag->setDefaultTextColor(QColorConstants::Black);
        phiNametag->setFont(nametagFont);
        phiNametag->setHtml(phi);
        phiNametag->setPos(300-phiLength*7,graphY-7);
        scene2->addItem(phiNametag);
        QGraphicsTextItem* alernativeNametag = new QGraphicsTextItem();
        alernativeNametag->setDefaultTextColor(QColorConstants::Black);
        alernativeNametag->setFont(nametagFont);
        alernativeNametag->setHtml(name);
        alernativeNametag->setPos(500,graphY-7);
        scene2->addItem(alernativeNametag);
    }

    ui->graphicsView_2->setScene(scene2);

}

Drawing::~Drawing()
{
    delete ui;
}

