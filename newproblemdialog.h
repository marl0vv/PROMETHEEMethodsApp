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
    explicit NewProblemDialog(QWidget *parent = nullptr);

    int getActions(){return m_actions;}
    int getCriterias(){return m_criterias;}
    ~NewProblemDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewProblemDialog *ui;
    int m_actions;
    int m_criterias;
};

#endif // NEWPROBLEMDIALOG_H
