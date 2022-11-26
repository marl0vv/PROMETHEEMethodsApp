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
    ui->tableWidget->setColumnCount(m_criterias + m_defaultColumnCount);
    ui->tableWidget->setRowCount(m_actions);

    QWidget* pWidget = new QWidget();
    QPushButton* btn_edit = new QPushButton();
    btn_edit->setText("Edit");
    QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
    pLayout->addWidget(btn_edit);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pWidget->setLayout(pLayout);
    ui->tableWidget->setCellWidget(0, 3, pWidget);

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

