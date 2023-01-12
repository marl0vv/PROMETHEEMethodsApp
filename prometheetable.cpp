#include "prometheetable.h"
#include "ui_prometheetable.h"
#include <QDebug>
#include <algorithm>

PrometheeTable::PrometheeTable(QWidget *parent, const std::vector<Actions> &actions, const int &count) :
    QWidget(parent),
    ui(new Ui::PrometheeTable),
    m_actions(actions),
    m_actionsCount(count)
{
    ui->setupUi(this);

    phiSort();
    buildTable();
}

PrometheeTable::~PrometheeTable()
{
    delete ui;
}

void PrometheeTable::phiSort()
{
    std::sort(m_actions.begin(), m_actions.end(),
              [] (Actions & a, Actions  &b) {return a.getPhi() > b.getPhi();});

    for (int i = 0; i < m_actionsCount; i++)
    {
       qDebug() << m_actions[i].getPhi();
    }
}
void PrometheeTable::buildTable()
{
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setRowCount(m_actionsCount);
    for (int i = 0; i < m_actionsCount; i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(m_actions[i].getName()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(m_actions[i].getPhi())));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(m_actions[i].getPhiPositive())));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(m_actions[i].getPhiNegative())));
    }
}





