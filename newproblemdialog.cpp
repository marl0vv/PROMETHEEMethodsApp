#include "newproblemdialog.h"
#include "ui_newproblemdialog.h"
#include "mainwindow.h"

NewProblemDialog::NewProblemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProblemDialog)
{
    ui->setupUi(this);
}

NewProblemDialog::~NewProblemDialog()
{
    delete ui;
}

void NewProblemDialog::on_buttonBox_accepted()
{

}

