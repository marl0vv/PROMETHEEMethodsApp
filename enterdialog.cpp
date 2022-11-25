#include "enterdialog.h"
#include "ui_enterdialog.h"

EnterDialog::EnterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterDialog)
{
    ui->setupUi(this);
}

EnterDialog::~EnterDialog()
{
    delete ui;
}
