#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

namespace Ui {
class helpdialog;
}

class helpdialog : public QDialog
{
    Q_OBJECT

public:
    explicit helpdialog(QWidget *parent = nullptr);
    ~helpdialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::helpdialog *ui;
};

#endif // HELPDIALOG_H
