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

void buildTable()
{

}
void MainWindow::on_actionNew_triggered()
{
    NewProblemDialog newProblemDialog(nullptr);
    newProblemDialog.setModal(true);
    newProblemDialog.exec();
    m_actions = newProblemDialog.getActions();
    m_criterias = newProblemDialog.getCriterias();


}

