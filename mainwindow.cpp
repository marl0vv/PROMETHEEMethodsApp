#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newproblemdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    NewProblemDialog newProblemDialog;
    newProblemDialog.setModal(true);
    newProblemDialog.exec();

}

