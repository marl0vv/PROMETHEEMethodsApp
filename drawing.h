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
    explicit Drawing(QWidget *parent,std::vector<Actions> a,int n);
    ~Drawing();
private:
    Ui::Drawing *ui;
    std::vector<Actions> m_drawingAlternatives; //все альтернативы
    int m_drawingAlternativesAmount; //кол-во альтернатив
};

#endif // DRAWING_H
