#ifndef NEWPROBLEMDIALOG_H
#define NEWPROBLEMDIALOG_H

#include <QDialog>

namespace Ui {
class NewProblemDialog;
}

class NewProblemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProblemDialog(QWidget *parent = nullptr, int* actions = 0, int* criterias = 0);

    ~NewProblemDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewProblemDialog *ui;
};

#endif // NEWPROBLEMDIALOG_H
