#include "prometheev.h"
#include "ui_prometheev.h"

PrometheeV::PrometheeV(QWidget *parent, std::vector<Actions> &actions) :
    QWidget(parent),
    ui(new Ui::PrometheeV)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0, 30);
    m_actions = actions;
    buildTable();
}

PrometheeV::~PrometheeV()
{
    delete ui;
}

void PrometheeV::buildTable()
{
    ui->tableWidget->setColumnCount(m_defaultColumnCount + getActionsCount());
    ui->tableWidget->setRowCount(m_defaultRowCount);

    //заполняем имена альтернатив
    for (int i = 0; i < getActionsCount(); i++)
    {
        int defaultColumn = 2;
        int defaultRow = 1;
        QTableWidgetItem* actionName = new QTableWidgetItem();
        actionName->setText(m_actions[i].getName());
        ui->tableWidget->setItem(defaultRow, defaultColumn + i, actionName);
    }

    //заполняем потоки предпочтения
    for (int i = 0; i < getActionsCount(); i++)
    {
        int defaultColumn = 2;
        int defaultRow = 2;
        QTableWidgetItem* actionName = new QTableWidgetItem();
        actionName->setText(QString::number(m_actions[i].getPhi()));
        ui->tableWidget->setItem(defaultRow, defaultColumn + i, actionName);
    }
}
