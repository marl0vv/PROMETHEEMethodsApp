#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newproblemdialog.h"

#include <QPushButton>

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

void MainWindow::buildTable()
{
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
   // ui->tableWidget->setColumnCount(m_criterias + m_defaultColumnCount);
   // ui->tableWidget->setRowCount(m_actions);


}
void MainWindow::on_actionNew_triggered()
{
    NewProblemDialog newProblemDialog(nullptr);
    newProblemDialog.setModal(true);
    newProblemDialog.exec();
    m_actions = newProblemDialog.getActions();
    m_criterias = newProblemDialog.getCriterias();

    buildTable();

}

