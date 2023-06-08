#include "helpdialog.h"
#include "ui_helpdialog.h"

helpdialog::helpdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpdialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

helpdialog::~helpdialog()
{
    delete ui;
}
