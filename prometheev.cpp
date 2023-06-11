#include "prometheev.h"
#include "ui_prometheev.h"
#include "ortools/linear_solver/linear_solver.h"

#include <QDebug>
#include <QString>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QComboBox>



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

void PrometheeV::on_pushButtonAddConstraint_clicked()
{
    //Заполнение коэффициентов ограничений

    //при нажатии на кнопку добавляется новая строка
    //количество строк по умолчанию увеличивается, чтобы при следующем нажатии на кнопку
    //увеличивать уже от изменённого количества строк
    ui->tableWidget->setRowCount(m_defaultRowCount + 1);
    m_defaultRowCount++;

    constraint_coeffs.push_back(std::vector<double>());
    //количество строк, которые нужно отнять от позиции элемента в таблице, чтобы
    //получить место в векторе, начиная от нуля (нулевой элемент находится на 4 строке)
    int zeroRowPosition = 4;
    for (int i = 0; i < getActionsCount(); i++)
    {
        constraint_coeffs[m_defaultRowCount - zeroRowPosition].push_back(0);
    }
    //тестовый кусочек для проверки заполнения массива
    for (int i = 0; i < constraint_coeffs.size(); i++)
    {
        for (int j = 0; j < getActionsCount(); j++)
        {
            qDebug() << "constraint " << i+1 << " coeff " << j+1 << " " << constraint_coeffs[i][j];
        }
    }

    //так как m_defaultRowCount - это количество, а мне нужен
    //индекс для подстановки виджетов в ячейки, я уменьшаю
    //m_defaultRowCount на один
    int defaultConstraintRow = m_defaultRowCount - 1;
    //потому что нужно заполнять от третьего (нумерация от нуля) столбца
    int defaultConstraintCoeffColumn = 2;
    for (int i = 0; i < getActionsCount(); i++)
    {
        QDoubleSpinBox* constraintCoeffs = new QDoubleSpinBox(0);
        constraintCoeffs->setProperty("row", defaultConstraintRow);
        constraintCoeffs->setProperty("column", defaultConstraintCoeffColumn+i);
        connect(constraintCoeffs, &QDoubleSpinBox::valueChanged, this, &PrometheeV::onConstraintCoeffsSpinBoxChanged);

        constraintCoeffs->setMinimum(INT_MIN);
        constraintCoeffs->setMaximum(INT_MAX);

        ui->tableWidget->setCellWidget(defaultConstraintRow, defaultConstraintCoeffColumn + i, constraintCoeffs);

    }

    //Заполнение выбора больше, меньше или равно ограничению должны быть коэффициенты
    int defaultConstraintCompareColumn = 2;
    QComboBox* constraintCompare = new QComboBox();
    constraintCompare->setProperty("row", defaultConstraintRow);
    connect(constraintCompare, &QComboBox::currentIndexChanged, this, &PrometheeV::onConstraintCompareComboBoxChanged);

    constraintCompare->addItem("<=");
    constraintCompare->addItem("=");
    constraintCompare->addItem(">=");
    ui->tableWidget->setCellWidget(defaultConstraintRow, defaultConstraintCompareColumn + getActionsCount(), constraintCompare);

    //Заполнение значения ограничения
    int defaultConstraintColumn = 3;
    QDoubleSpinBox* constraintValue = new QDoubleSpinBox(0);
    connect(constraintValue, &QDoubleSpinBox::valueChanged, this, &PrometheeV::onConstraintValueSpinBoxChanged);

    constraintValue->setMinimum(INT_MIN);
    constraintValue->setMaximum(INT_MAX);
    ui->tableWidget->setCellWidget(defaultConstraintRow, defaultConstraintColumn + getActionsCount(), constraintValue);


}
void PrometheeV::onConstraintCoeffsSpinBoxChanged(double d)
{
    int row = sender()->property("row").toInt();
    int column = sender()->property("column").toInt();

    //значения, при вычитании которых от
    //индексов строк и столбцов элемента в таблице мы получим значение
    //этого элемента в векторе constraint_coeffs
    int defaultRow = 3;
    int defaultColumn = 2;
    constraint_coeffs[row - defaultRow][column - defaultColumn] = d;

    //тестовый кусок для вывода каждого коэффициента ограничения
    qDebug() << "row: " << row << " column: " << column << " value: " << d;
    for (int i = 0; i < constraint_coeffs.size(); i++)
    {
        for (int j = 0; j < getActionsCount(); j++)
        {
             qDebug() << "constraint " << i+1 << "coeff " << j+1 << " " << constraint_coeffs[i][j];
        }
    }

}

void PrometheeV::onConstraintCompareComboBoxChanged(int index)
{
    constraintCompare.push_back(index);

    //тестовый вывод содержимого
    for(int i = 0; i < constraintCompare.size(); i++)
    {
        qDebug() << "index " << i << "compare index:" << constraintCompare[i];
    }
}

void PrometheeV::onConstraintValueSpinBoxChanged(double d)
{
    bounds.push_back(d);
    //тестовый вывод содержимого
    for(int i = 0; i < bounds.size(); i++)
    {
        qDebug() << "bound " << i << "bound content:" << bounds[i];
    }
}

void PrometheeV::prometheeV()
{
    int numConstraints = bounds.size(); //количество ограничений; количество коэффициентов мы содержим в getActionsCount()

}
