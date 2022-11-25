#ifndef ENTERDIALOG_H
#define ENTERDIALOG_H

#include <QDialog>

namespace Ui {
class EnterDialog;
}

class EnterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnterDialog(QWidget *parent = nullptr);
    ~EnterDialog();

private:
    Ui::EnterDialog *ui;
};

#endif // ENTERDIALOG_H
