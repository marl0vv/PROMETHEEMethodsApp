#include "prometheev.h"
#include "ortools/linear_solver/linear_solver.h"
#include "ui_prometheev.h"

#include <QDebug>
#include <QString>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <memory>



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
    constraintCompare.push_back(0);
    QComboBox* constraintCompare = new QComboBox();
    constraintCompare->setProperty("row", defaultConstraintRow);
    constraintCompare->setProperty("column", defaultConstraintCompareColumn + getActionsCount());
    connect(constraintCompare, &QComboBox::currentIndexChanged, this, &PrometheeV::onConstraintCompareComboBoxChanged);

    constraintCompare->addItem("<=");
    constraintCompare->addItem("=");
    constraintCompare->addItem(">=");
    ui->tableWidget->setCellWidget(defaultConstraintRow, defaultConstraintCompareColumn + getActionsCount(), constraintCompare);

    //Заполнение значения ограничения
    int defaultConstraintColumn = 3;
    bounds.push_back(0);
    QDoubleSpinBox* constraintValue = new QDoubleSpinBox(0);
    constraintValue->setProperty("row", defaultConstraintRow);
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

    prometheeV();
}

void PrometheeV::onConstraintCompareComboBoxChanged(int index)
{
    int row = sender()->property("row").toInt();


    int defaultRow = 3;

    constraintCompare[row - defaultRow] = index;

    //тестовый вывод содержимого
    for(int i = 0; i < constraintCompare.size(); i++)
    {
        qDebug() << "index " << i << "compare index:" << constraintCompare[i];
    }
}

void PrometheeV::onConstraintValueSpinBoxChanged(double d)
{
    int row = sender()->property("row").toInt();
    int defaultRow = 3;

    bounds[row - defaultRow] = d;

    //тестовый вывод содержимого
    for(int i = 0; i < bounds.size(); i++)
    {
        qDebug() << "bound " << i << "bound content:" << bounds[i];
    }
}

void PrometheeV::prometheeV()
{
    results.resize(getActionsCount());
    int numConstraints = bounds.size(); //количество ограничений; количество коэффициентов мы содержим в getActionsCount()
    std::unique_ptr<operations_research::MPSolver> solver(operations_research::MPSolver::CreateSolver("SCIP"));
    if (!solver) {
        qDebug() << "SCIP solver unavailable.";
        return;
    }

    //устанавливаем для итоговых значений варианты 0 или 1,
    //как прописано в методе
    for (int j = 0; j < getActionsCount(); ++j)
    {
        results[j] = solver->MakeIntVar(0.0, 1, "");
    }
    qDebug() << "Number of variables = " << solver->NumVariables();

    // Задаём ограничения.
    for (int i = 0; i < numConstraints; ++i)
    {
        operations_research::MPConstraint* constraint = solver->MakeRowConstraint(0, bounds[i], "");
        for (int j = 0; j < getActionsCount(); ++j)
        {
             constraint->SetCoefficient(results[j], constraint_coeffs[i][j]);
        }
    }
    qDebug() << "Number of constraints = " << solver->NumConstraints();

    //Устанавливаем задачу максимизации или минимизации
    operations_research::MPObjective* const objective = solver->MutableObjective();
    for (int j = 0; j < getActionsCount(); ++j) {
        objective->SetCoefficient(results[j], m_actions[j].getPhi());
    }
    objective->SetMaximization();

    const operations_research::MPSolver::ResultStatus result_status = solver->Solve();

    // Check that the problem has an optimal solution.
    if (result_status != operations_research::MPSolver::OPTIMAL) {
        qDebug() << "The problem does not have an optimal solution.";
    }
    qDebug() << "Solution:";
    qDebug() << "Optimal objective value = " << objective->Value();

    for (int j = 0; j < getActionsCount(); ++j)
    {
        qDebug() << "x[" << j << "] = " << results[j]->solution_value();
    }

}

