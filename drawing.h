#ifndef DRAWING_H
#define DRAWING_H

#include <QWidget>

namespace Ui {
class drawing;
}

class drawing : public QWidget
{
    Q_OBJECT

public:
    explicit drawing(QWidget *parent = nullptr);
    ~drawing();

private slots:
    void on_pushButton_clicked();

private:
    Ui::drawing *ui;
};

#endif // DRAWING_H
