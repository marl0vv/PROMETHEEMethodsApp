#ifndef DRAWING_H
#define DRAWING_H

#include <QWidget>
#include <mainwindow.h>
namespace Ui {
class Drawing;
}

class Drawing : public QWidget
{
    Q_OBJECT

public:
    explicit Drawing(QWidget *parent,std::vector<Actions> &actions,int &count);
    ~Drawing();
private:
    Ui::Drawing *ui;
    std::vector<Actions> m_drawingAlternatives; //все альтернативы в векторе
    int m_drawingAlternativesAmount; //кол-во альтернатив
};

#endif // DRAWING_H
