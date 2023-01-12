#include "helpdialog.h"
#include "ui_helpdialog.h"

helpdialog::helpdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpdialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

helpdialog::~helpdialog()
{
    delete ui;
}

void helpdialog::on_pushButton_clicked()
{
    hide();
    close();
}

