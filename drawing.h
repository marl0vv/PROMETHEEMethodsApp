#ifndef DRAWING_H
#define DRAWING_H

#include <QWidget>

#include "Actions.h"

namespace Ui {
class Drawing;
}

class Drawing : public QWidget
{
    Q_OBJECT

public:
    explicit Drawing(QWidget *parent, std::vector<Actions> &actions,int &count);
    ~Drawing();
private:
    Ui::Drawing *ui;
    std::vector<Actions> m_drawingAlternatives; //все альтернативы в векторе
    int m_drawingAlternativesAmount; //кол-во альтернатив
    int m_windowHeight = 800;
    int m_windowWidth = 800;
    int m_verticalIndent = 100;
    int m_phiRectangleHeight = 600;
    int m_phiPosLeftEdge = 100;
    int m_phiPosRightEdge = 120;
    int m_phiNegLeftEdge = 680;
    int m_phiNegRightEdge = 700;
    double** m_point;
        int** mas;
};

#endif // DRAWING_H
