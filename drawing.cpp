#include "drawing.h"
#include "ui_drawing.h"
#include <QGraphicsTextItem>
#include <QStandardItem>
#include <mainwindow.h>
#include <QStack>


int DFS(int **mas, bool* vis, int cur,int prev, QStack<int> Q,const int n, QGraphicsScene *scene4, double **point, QPen blackPen){
    if (vis[cur]==true) return -1;
    QStack<int> q;
    vis[cur]=true;
    if((mas[prev][cur]==0)&&(mas[cur][prev]==1)) {
        QPointF p1;
        QPointF p2;
        p1.setX(point[cur][0]+10);
        p1.setY(point[cur][1]+10);
        p2.setX(point[prev][0]+10);
        p2.setY(point[prev][1]+10);
        QLineF line;
        line.setPoints(p1,p2);
        scene4->addLine(line,blackPen);
        int x_half = (point[cur][0] + point[prev][0]+20)/2;
        int y_half = (point[cur][1] + point[prev][1]+20)/2;
        QLineF arrow;
        arrow.setP1(QPointF(x_half,y_half));
        arrow.setAngle(line.angle()+150);
        arrow.setLength(40);
        scene4->addLine(arrow, blackPen);
        arrow.setAngle(line.angle()-150);
        scene4->addLine(arrow, blackPen);

    }
    if((mas[prev][cur]==1)&&(mas[cur][prev]==0)) {
        QPointF p1;
        QPointF p2;
        p1.setX(point[cur][0]+10);
        p1.setY(point[cur][1]+10);
        p2.setX(point[prev][0]+10);
        p2.setY(point[prev][1]+10);
        QLineF line;
        line.setPoints(p1,p2);
        scene4->addLine(line,blackPen);
        int x_half = (point[cur][0] + point[prev][0]+20)/2;
        int y_half = (point[cur][1] + point[prev][1]+20)/2;
        QLineF arrow;
        arrow.setP1(QPointF(x_half,y_half));
        arrow.setAngle(line.angle()+30);
        arrow.setLength(40);
        scene4->addLine(arrow, blackPen);
        arrow.setAngle(line.angle()-30);
        scene4->addLine(arrow, blackPen);

    }
    for(int i=0; i<n;i++){
        if(vis[i]==false && mas[cur][i]==1 && mas[i][cur]==0) {
            q.push(i);
        }
    }
    while (!q.empty())
    {
        int Prev = cur;
        int Cur = q.top();
        q.pop();
        DFS(mas,vis,Cur,Prev,q,n,scene4,point,blackPen);

    }
    return 0;
}


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
    QGraphicsScene * scene3 = new QGraphicsScene;
    QGraphicsScene * scene4 = new QGraphicsScene;
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
    QPen cyanPen(Qt::cyan);
    cyanPen.setWidth(3);
    QBrush cyanBrush(Qt::cyan);
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

    QFont font=QFont("Roboto", 10, QFont::Bold);

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

    //заменил обычные сишные массивы на векторы, потому что программа ломалась в компиляторе MSVC
    //надеюсь, что я ничего не сломал
    std::vector<double> phiPositiveArray(m_drawingAlternativesAmount);
    std::vector<double> phiNegativeArray(m_drawingAlternativesAmount);
    std::vector<double> phiArray(m_drawingAlternativesAmount);
   // double phiPositiveArray[m_drawingAlternativesAmount];
   // double phiNegativeArray[m_drawingAlternativesAmount];
   // double phiArray[m_drawingAlternativesAmount];
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
    QFont nametagFont=QFont("Roboto", 8);

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

    //PROMETHEE Diamond
    for (int i=0; i<m_drawingAlternativesAmount;i++)
    {
        phiPositiveArray[i]=m_drawingAlternatives[i].getPhiPositive();
        // вычитаем значение для корректного построения,
        // поскольку значения справа для Phi- идут от 0 до 1 сверху вниз,
        // и Phi не может быть больше 1, вычитаем Phi-
        // из единицы для получения противоположного значения
        phiNegativeArray[i]=m_drawingAlternatives[i].getPhiNegative();
        phiArray[i]=m_drawingAlternatives[i].getPhi();
    }

    scene3->addRect(greenMid,blackPen,darkGreenBrush);
    scene3->addRect(redMid,blackPen,darkRedBrush);
    str = "+1.0";
    QGraphicsTextItem* positiveOneLabelDiamond = new QGraphicsTextItem();
    positiveOneLabelDiamond->setDefaultTextColor(QColorConstants::Black);
    positiveOneLabelDiamond->setFont(font);
    positiveOneLabelDiamond->setHtml(str);
    positiveOneLabelDiamond->setPos(380,75);
    scene3->addItem(positiveOneLabelDiamond);
    str = "-1.0";
    QGraphicsTextItem* negativeOneLabelDiamond = new QGraphicsTextItem();
    negativeOneLabelDiamond->setDefaultTextColor(QColorConstants::Black);
    negativeOneLabelDiamond->setFont(font);
    negativeOneLabelDiamond->setHtml(str);
    negativeOneLabelDiamond->setPos(380,700);
    scene3->addItem(negativeOneLabelDiamond);
    str = "0.0";
    QGraphicsTextItem* zeroLabelDiamond = new QGraphicsTextItem();
    zeroLabelDiamond->setDefaultTextColor(QColorConstants::Black);
    zeroLabelDiamond->setFont(font);
    zeroLabelDiamond->setHtml(str);
    zeroLabelDiamond->setPos(365,385);
    scene3->addItem(zeroLabelDiamond);
    scene3->addLine(400,100,100,400,blackPen);
    scene3->addLine(100,400,400,700,blackPen); // - Phi-
    scene3->addLine(400,700,700,400,blackPen); //   Phi+
    scene3->addLine(700,400,400,100,blackPen);
    graphY = 0;
    qreal xPhiNeg;
    qreal xPhiPos;
    qreal yPhiNeg;
    qreal yPhiPos;
    qreal xTopPoint;
    for (int i=0; i<m_drawingAlternativesAmount;i++){
        if (phiArray[i] < 0){ //если Фи отрицательное
            phiArray[i] *= -1; // делаем его положительным
            //но строим в нижней (красной) половине отрезка
            //            600/2 = 300       + отступ (100)     +         300            * Фи, при Фи равном -1 рисуем в точке 700 (низ отрезка)
            graphY = m_phiRectangleHeight/2 + m_verticalIndent + m_phiRectangleHeight/2 * phiArray[i];
        } // иначе рисуем в верхней (зеленой) половине отрезка
     // середина отрезка 800/2 = 400   -       300              * Фи, при Фи равном 1 рисуем в точке с Y=100 (верх отрезка)
        else graphY = m_windowHeight/2 - m_phiRectangleHeight/2 * phiArray[i];

        xPhiNeg = 100+300*phiNegativeArray[i];
        xPhiPos = 400+300*phiPositiveArray[i];
        yPhiNeg = 400+300*phiNegativeArray[i];
        yPhiPos = 700-300*phiPositiveArray[i];
        xTopPoint = xPhiNeg+xPhiPos-400;

        scene3->addLine(xPhiNeg,yPhiNeg, xTopPoint, graphY, cyanPen);
        scene3->addLine(xPhiPos,yPhiPos, xTopPoint, graphY, cyanPen);
        scene3->addLine(xTopPoint, graphY, xTopPoint+150+i*10, graphY, cyanPen);
        QString name = m_drawingAlternatives[i].getName();
        QRectF *highlightRectName = new QRectF;
        int nameLength = name.length();
        highlightRectName->setCoords(xTopPoint+150+i*10+nameLength*7,graphY-8,xTopPoint+150+i*10,graphY+8);
        scene3->addRect(*highlightRectName,cyanPen,cyanBrush);
        delete(highlightRectName);
        QGraphicsTextItem* alernativeNametag = new QGraphicsTextItem();
        alernativeNametag->setDefaultTextColor(QColorConstants::Black);
        alernativeNametag->setFont(nametagFont);
        alernativeNametag->setHtml(name);
        alernativeNametag->setPos(xTopPoint+150+i*10,graphY-7);
        scene3->addItem(alernativeNametag);

    }
    ui->graphicsView_3->setScene(scene3);







    //рисуем 4 график

    double maxphi = -99999;
    int pos_maxphi= 0;
    double secondmaxphi = -99999;
    int pos_secondmaxphi= 0;
    double x_half;
    double y_half;
    int r=250;
    int n = m_drawingAlternativesAmount;
    m_point = new double* [n];
    mas = new int* [n];
    int* connections =new int[n];
    double slice = 2 * 3.14 / n;
    QBrush whitebrush(Qt::white);
    bool* vis;


    for (int i = 0; i < n; i++) {
                 mas[i] = new int[n];
    }
    for (int i = 0; i < n; i++) {
         for (int j = 0; j < n; j++) {
              mas[i][j] = 0;
         }
    }
    if(n>10) {
        r += (n-10)*15;
    }
    for(int i = 0 ; i < n; i++) {
        m_point[i] = new double[2];
    }
    for(int i = 0 ; i < n; i++) {
        double angle = slice * i;
        m_point[i][0] = (double)(400 + r * cos(angle));
        m_point[i][1] = (double)(400 + r * sin(angle));
    }


    for (int i = 0 ; i < n; i++) {
        for (int j = 0 ; j < n; j++) {

        if ((m_drawingAlternatives[i].getPhiPositive() > m_drawingAlternatives[j].getPhiPositive()) && (m_drawingAlternatives[i].getPhiNegative() < m_drawingAlternatives[j].getPhiNegative())
                || (m_drawingAlternatives[i].getPhiPositive() > m_drawingAlternatives[j].getPhiPositive()) && (m_drawingAlternatives[i].getPhiNegative() == m_drawingAlternatives[j].getPhiNegative())
                || (m_drawingAlternatives[i].getPhiPositive() == m_drawingAlternatives[j].getPhiPositive()) && (m_drawingAlternatives[i].getPhiNegative() < m_drawingAlternatives[j].getPhiNegative()))
        {
            mas[i][j]=1;
            mas[j][i]=0;
        }
        else if ((m_drawingAlternatives[i].getPhiPositive() < m_drawingAlternatives[j].getPhiPositive()) && (m_drawingAlternatives[i].getPhiNegative() > m_drawingAlternatives[j].getPhiNegative())
                 || (m_drawingAlternatives[i].getPhiPositive() < m_drawingAlternatives[j].getPhiPositive()) && (m_drawingAlternatives[i].getPhiNegative() == m_drawingAlternatives[j].getPhiNegative())
                 || (m_drawingAlternatives[i].getPhiPositive() == m_drawingAlternatives[j].getPhiPositive()) && (m_drawingAlternatives[i].getPhiNegative() > m_drawingAlternatives[j].getPhiNegative()))
        {
            mas[i][j]=0;
            mas[j][i]=1;
        }
        else {
            mas[i][j]=0;
            mas[j][i]=0;
        }
        }
    }
        vis = new bool[n];
        for(int i = 0; i < n; i++) {
            vis[i] = false;
        }

        for(int i=0; i < n; i++) {
                connections[i]=0;
                }
        for(int i=0; i < n; i++) {
            if(m_drawingAlternatives[i].getPhi() > maxphi) {
                maxphi=m_drawingAlternatives[i].getPhi();
                pos_maxphi=i;
        } if ((m_drawingAlternatives[i].getPhi() > secondmaxphi)&&(m_drawingAlternatives[i].getPhi()!=maxphi)){
                secondmaxphi = m_drawingAlternatives[i].getPhi();
                pos_secondmaxphi=i;
                }
            }
        for(int i=0; i < n; i++) {
                for(int j=0; j < n; j++) {
                    if((mas[i][j]>0)) {
                        connections[i]++;
                    }
                }

             }

            int start = pos_maxphi;
            qDebug() << start;
            qDebug() << pos_secondmaxphi;
            qDebug() << maxphi;
            qDebug() << secondmaxphi;
            QStack<int> q;
           q.push(start);
           DFS(mas,vis,start,start,q,n,scene4,m_point,blackPen);
           for (int i = 0; i < n; i++)
                {
                    if ((vis[i] == false)&&(connections[i] > 0))
                {
                    for (int j = 0; j<n;j++)
                    {
                        if ((mas[i][j]==1) && (vis[j]==false))
                        {
                            DFS(mas,vis,j,i,q,n,scene4,m_point,blackPen);
                        }
                     }
                 }
                 }


    for(int i = 0; i < n; i++) {

        //вершины графа
        QString name = m_drawingAlternatives[i].getName();
        int nameLength = name.length();
        QString phiPos = QString::number(m_drawingAlternatives[i].getPhiPositive());
        int phiPosLength = phiPos.length();
        QString phiNeg = QString::number(m_drawingAlternatives[i].getPhiNegative());
        int phiNegLength = phiNeg.length();
        QGraphicsTextItem* phiNametag = new QGraphicsTextItem();
        phiNametag->setDefaultTextColor(QColorConstants::Black);
        phiNametag->setFont(nametagFont);
        phiNametag->setHtml("phi+: "+phiPos+" phi-: "+phiNeg);
        phiNametag->setPos(m_point[i][0],m_point[i][1]+15);
        QGraphicsTextItem* alernativeNametag = new QGraphicsTextItem();
        alernativeNametag->setDefaultTextColor(QColorConstants::Black);
        alernativeNametag->setFont(nametagFont);
        alernativeNametag->setHtml(name);
        alernativeNametag->setPos(m_point[i][0],m_point[i][1]);
        scene4->addRect(QRectF(m_point[i][0],m_point[i][1],std::max(60+phiPosLength*7+phiNegLength*7,nameLength*7), 40), blackPen ,whitebrush);
        scene4->addItem(phiNametag);
        scene4->addItem(alernativeNametag);

    }
    /*
    QString matrixoutput;
    for (int i = 0; i < n; i++) //
         {
             for (int j = 0; j<n;j++)
             {
                 matrixoutput+=QString::number(mas[i][j])+" ";

              }
             matrixoutput+="\n";
          }
    QGraphicsTextItem* moutp = new QGraphicsTextItem();
    moutp->setDefaultTextColor(QColorConstants::Black);
    moutp->setFont(nametagFont);
    moutp->setHtml(matrixoutput);
    moutp->setPos(100,20);
    scene4->addItem(moutp);
*/

    ui->graphicsView_4->setScene(scene4);


}

Drawing::~Drawing()
{
    delete ui;
}


