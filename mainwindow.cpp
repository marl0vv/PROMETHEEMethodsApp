#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newproblemdialog.h"
#include "Actions.h"

#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDebug>
#include <cmath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFont font("Roboto", 8);
    QApplication::setFont(font);

    ui->tableWidget->setColumnWidth(0, 30);
    //ui->tableWidget->resizeRowsToContents();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Устанавливает спинбокс для получения обычных числовых значений
void MainWindow::setColumnTypeNum(int column)
{
    for (int i = 0; i < m_actionsCount; i++)
    {
        QDoubleSpinBox* actionValue = new QDoubleSpinBox(0);
        actionValue->setProperty("row", 13+i);
        actionValue->setProperty("column", column);
        connect(actionValue, &QDoubleSpinBox::valueChanged, this, &MainWindow::onCriteriaSpinBoxChanged);

        actionValue->setMinimum(INT_MIN);
        actionValue->setMaximum(INT_MAX);
        ui->tableWidget->setCellWidget(13+i, column, actionValue);
    }
}
//Устанавливает спинбокс для получения обычных числовых значений,
//но содержит рублёвый префикс, для симуляции того, что содержимое - валюта
void MainWindow::setColumnTypeMoney(int column)
{

    for (int i = 0; i < m_actionsCount; i++)
    {
        QDoubleSpinBox* actionValue = new QDoubleSpinBox(0);
        actionValue->setProperty("row", 13+i);
        actionValue->setProperty("column", column);
        connect(actionValue, &QDoubleSpinBox::valueChanged, this, &MainWindow::onCriteriaSpinBoxChanged);

        actionValue->setPrefix("₽ ");
        actionValue->setMaximum(INT_MAX);
        ui->tableWidget->setCellWidget(13+i, column, actionValue);

    }
}
//Устанавливает комбобокс в котором содержатся качественные значения
void MainWindow::setColumnTypeQuality(int column)
{
    for (int i = 0; i < m_actionsCount; i++)
    {
        QComboBox* actionValue = new QComboBox();
        //comboScale->setProperty("column", QVariant(2+i));
        //connect(comboScale, &QComboBox::currentIndexChanged, this, &MainWindow::onComboBoxChanged);
        actionValue->setProperty("row", 13+i);
        actionValue->setProperty("column", column);
        connect(actionValue, &QComboBox::currentIndexChanged, this, &MainWindow::onCriteriaComboBoxChanged);

        actionValue->addItem("n/a");
        actionValue->addItem("очень плохо");
        actionValue->addItem("плохо");
        actionValue->addItem("средне");
        actionValue->addItem("хорошо");
        actionValue->addItem("очень хорошо");
        ui->tableWidget->setCellWidget(13+i, column, actionValue);
    }
}
//
void MainWindow::onComboBoxChanged(int index)
{
    //qDebug() << "column:" << sender()->property("column").toInt();
    int column = sender()->property("column").toInt();
    if (index == 0)
    {
        setColumnTypeNum(column);
    }
    if (index == 1)
    {
        setColumnTypeMoney(column);
    }
    if (index == 2)
    {
        setColumnTypeQuality(column);
    }

    // qDebug() << index;
}

void MainWindow::onCriteriaSpinBoxChanged(double d)
{
    int row = sender()->property("row").toInt();
    int column = sender()->property("column").toInt();

    //ля, зачем я ставлю эти константы магические, но уже поздно
    //-13 потому что первая альтернатива находится в 13 строке
    //таким образом, нулевая альтернатива будет при row-13
    //с -2 аналогичная история
    m_actions[row-13].getCriteria()[column-2] = d;
    qDebug() << "row: " << row << " column: " << column << " value: " << d;
    for (int i = 0; i < m_actionsCount; ++i)
    {
        for (int j = 0; j < m_criteriasCount; ++j)
        {
            qDebug() << "action " << i+1 << " criteria " << j+1 << " " << m_actions[i].getCriteria()[j];
        }
    }

    statsFindMax();
    statsFindMin();
    statsFindAverage();
    statsFindDeviation();
}

void MainWindow::onCriteriaComboBoxChanged(int index)
{
    int row = sender()->property("row").toInt();
    int column = sender()->property("column").toInt();


    //значение индекса - это числовое значение качества
    m_actions[row-13].getCriteria()[column-2] = index;

    qDebug() << "row: " << row << " column: " << column << " value: " << index;
    for (int i = 0; i < m_actionsCount; ++i)
    {
        for (int j = 0; j < m_criteriasCount; ++j)
        {
            qDebug() << "action " << i+1 << " criteria " << j+1 << " " << m_actions[i].getCriteria()[j];
        }
    }

    statsFindMax();
    statsFindMin();
    statsFindAverage();
    statsFindDeviation();
}

//функция для вывода максимального элемента по одному критерию среди всех альтернатив в блоке статистики
//Строка не меняется, так как все данные об максимальном значении пишутся
//в одну строку, а столбец меняется, так как у разных критериев разное максимальное значение.
void MainWindow::statsFindMax()
{
    double maxElement;

    for (int j = 0; j < m_criteriasCount; j++)
    {
        maxElement = INT_MIN;
        for (int i = 0; i < m_actionsCount; i++)
        {
            if (maxElement < m_actions[i].getCriteria()[j])
            {
                maxElement = m_actions[i].getCriteria()[j];

            }
        }
        ui->tableWidget->setItem(9, 2+j, new QTableWidgetItem(QString::number(maxElement)));
    }

}

void MainWindow::statsFindMin()
{
    double minElement;

    for (int j = 0; j < m_criteriasCount; j++)
    {
        minElement = INT_MAX;
        for (int i = 0; i < m_actionsCount; i++)
        {
            if (minElement > m_actions[i].getCriteria()[j])
            {
                minElement = m_actions[i].getCriteria()[j];

            }
        }
        ui->tableWidget->setItem(8, 2+j, new QTableWidgetItem(QString::number(minElement)));
    }
}
void MainWindow::statsFindAverage()
{
    double average;
    for (int j = 0; j < m_criteriasCount; j++)
    {
        double sum = 0;
        for (int i = 0; i < m_actionsCount; i++)
        {
            sum += m_actions[i].getCriteria()[j];
        }
        average = (double) sum / (double) m_actionsCount;
        ui->tableWidget->setItem(10, 2+j, new QTableWidgetItem(QString::number(average, 'f', 2)));
    }
}

void MainWindow::statsFindDeviation()
{
    double average;
    double deviation;
    for (int j = 0; j < m_criteriasCount; j++)
    {
        int sum = 0;
        for (int i = 0; i < m_actionsCount; i++)
        {
            sum += m_actions[i].getCriteria()[j];
        }
        average = (double) sum / (double) m_actionsCount;

        //это просто отвратительное название. Здесь я имел в виду сумму квадратов разностей отдельного значения выборки и
        //среднего арифметического выборки
        double sumOfPowsOfDiff = 0;
        for (int i = 0; i < m_actionsCount; i++)
        {
            sumOfPowsOfDiff += std::pow((m_actions[i].getCriteria()[j] - average), 2);
        }
        deviation = std::sqrt(sumOfPowsOfDiff/m_actionsCount);
        ui->tableWidget->setItem(11, 2+j, new QTableWidgetItem(QString::number(deviation, 'f', 2)));
    }
}

void MainWindow::onMinOrMaxComboBoxChanged(int index)
{

    int column = sender()->property("column").toInt();

    if (index == 0)
    {
        m_criteriasMinMax[column-2] = -1;
    }
    if (index == 1)
    {
        m_criteriasMinMax[column-2] = 1;
    }
    for (int i = 0; i < m_criteriasCount; i++)
    {
        qDebug() << "Criteria  " << i+1  << "Min or Max: " << m_criteriasMinMax[i];
    }
}

void MainWindow::onWeightComboBoxChanged(double d)
{
    int column = sender()->property("column").toInt();
    int sum = 0;
    m_criteriasTableWeight[column-2] = d;
    for (int j = 0; j < m_criteriasCount; j++)
    {
        sum += m_criteriasTableWeight[j];
    }
    for (int j = 0; j < m_criteriasCount; j++)
    {
        m_criteriasWeight[j] = m_criteriasTableWeight[j] / sum;
    }

}
void MainWindow::buildTable()
{

    //Создаём таблицу и заголовки
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(m_criteriasCount + m_defaultColumnCount);
    ui->tableWidget->setRowCount(m_actionsCount + m_defaultRowCount);


    QFont font("Roboto", 8, QFont::Bold);

    //выделяем серым заголовок Свойств
    for (int i = 0; i < m_criteriasCount + m_defaultColumnCount; i++)
    {
        QTableWidgetItem* props = new QTableWidgetItem;
        props->setBackground(QColorConstants::LightGray);
        ui->tableWidget->setItem(4, i, props);
    }
    QTableWidgetItem* optionsLabel = new QTableWidgetItem("Свойства");
    optionsLabel->setBackground(QColorConstants::LightGray);
    optionsLabel->setFont(font);
    ui->tableWidget->setItem(4, 1, optionsLabel);

    //выделяем серым заголовок Статистика
    for (int i = 0; i < m_criteriasCount + m_defaultColumnCount; i++)
    {
        QTableWidgetItem* props = new QTableWidgetItem;
        props->setBackground(QColorConstants::LightGray);
        ui->tableWidget->setItem(7, i, props);
    }
    QTableWidgetItem* statLabel = new QTableWidgetItem("Статистика");
    statLabel->setBackground(QColorConstants::LightGray);
    statLabel->setFont(font);
    ui->tableWidget->setItem(7, 1, statLabel);

    //выделяем серым заголовок Альтернативы
    for (int i = 0; i < m_criteriasCount + m_defaultColumnCount; i++)
    {
        QTableWidgetItem* props = new QTableWidgetItem;
        props->setBackground(QColorConstants::LightGray);
        ui->tableWidget->setItem(12, i, props);
    }
    QTableWidgetItem* altLabel = new QTableWidgetItem("Альтернативы");
    altLabel->setBackground(QColorConstants::LightGray);
    altLabel->setFont(font);
    ui->tableWidget->setItem(12, 1, altLabel);

    //Дальше идёт заполнение остальных столбцов и таблиц заглушками, которые пользователь будет менять на свои значения

    //это очень плохо
    //setItem идёт в 2+i, потому что 2 - это имя строки
    for (int i = 0; i < m_criteriasCount; i++)
    {
        QString criteriaName = "Критерий" + QString::number(i+1);
        ui->tableWidget->setItem(1, 2+i, new QTableWidgetItem(criteriaName));


        //В столбцы строки "Шкала" помещаются комбобоксы.
        //Эти комбобоксы отражают то, в какой шкале измеряется критерий, определяемый этим столбцом.
        //Сигнал изменения индекса комбобокса отправляется в слот onComboBoxChanged
        //Этот слот затем использует функции для изменения шкал, в которых измеряются критерии для каждой альтернативы.
        QComboBox* comboScale = new QComboBox();
        comboScale->setProperty("column", QVariant(2+i));
        connect(comboScale, &QComboBox::currentIndexChanged, this, &MainWindow::onComboBoxChanged);
        ui->tableWidget->setCellWidget(2, 2+i, comboScale);
        comboScale->addItem("Числовая");
        comboScale->addItem("Денежная");
        comboScale->addItem("Качественная");

        ui->tableWidget->setItem(3, 2+i, new QTableWidgetItem("единица измерения"));

        //изменения значения этого комбобокса должно влиять на то, минимальное значение будет
        //использоваться для высчитывания по формулам или максимальное
        //но это надо на сами формулы смотреть
        //Ещё хотелось бы, чтобы минимальное и максимальное значения у actions
        //подчёркивалось красным и зелёным
        QComboBox* minOrMax = new QComboBox();
        minOrMax->setProperty("column", QVariant(2+i));
        connect(minOrMax, &QComboBox::currentIndexChanged, this, &MainWindow::onMinOrMaxComboBoxChanged);
        ui->tableWidget->setCellWidget(5, 2+i, minOrMax);
        minOrMax->addItem("Минимум");
        minOrMax->addItem("Максимум");


        //вес
        QDoubleSpinBox* comboWeight = new QDoubleSpinBox();
        comboWeight->setProperty("column", QVariant(2+i));
        connect(comboWeight, &QDoubleSpinBox::valueChanged, this, &MainWindow::onWeightComboBoxChanged);
        ui->tableWidget->setCellWidget(6, 2+i, comboWeight);


        ui->tableWidget->setItem(8, 2+i, new QTableWidgetItem("n/a"));
        ui->tableWidget->setItem(9, 2+i, new QTableWidgetItem("n/a"));
        ui->tableWidget->setItem(10, 2+i, new QTableWidgetItem("n/a"));
        ui->tableWidget->setItem(11, 2+i, new QTableWidgetItem("n/a"));

    }

    //заполняю строки с actions
    for (int i = 0; i < m_actionsCount; i++)
    {
        QString actionName = "Альтернатива" + QString::number(i+1);
        ui->tableWidget->setItem(13+i, 1, new QTableWidgetItem(actionName));
    }

    //заполняем строки с actions и criterias
    for (int i = 0; i < m_actionsCount; i++)
    {
        for (int j = 0; j < m_criteriasCount; j++)
        {
            QDoubleSpinBox* actionValue = new QDoubleSpinBox(0);
            actionValue->setProperty("row", 13+i);
            actionValue->setProperty("column", 2+j);
            connect(actionValue, &QDoubleSpinBox::valueChanged, this, &MainWindow::onCriteriaSpinBoxChanged);

            actionValue->setMinimum(INT_MIN);
            actionValue->setMaximum(INT_MAX);
            ui->tableWidget->setCellWidget(13+i, 2+j, actionValue);
            //ui->tableWidget->setItem(13+i, 2+j, new QTableWidgetItem("n/a"));
        }
    }


}
void MainWindow::on_actionNew_triggered()
{
    NewProblemDialog newProblemDialog(nullptr);
    newProblemDialog.setModal(true);
    newProblemDialog.exec();
    m_actionsCount = newProblemDialog.getActions();
    m_criteriasCount = newProblemDialog.getCriterias();
    m_criteriasMinMax.resize(m_criteriasCount);
    m_criteriasWeight.resize(m_criteriasCount);
    m_criteriasTableWeight.resize(m_criteriasCount);

    buildTable();

    //идёт расширение вектора альтернатив, созданного в классе mainwindow
    //до размеров m_actionsCount и m_criteriasCount
    m_actions.resize(m_actionsCount, Actions());
    for(int i = 0; i < m_actions.size(); i++)
    {
        m_actions[i].getCriteria().resize(m_criteriasCount, 0);
    }


    //в этом кусочке вектора объекта m_actions заполняются нулями,
    //так как логично, что при создании новой таблицы они должны быть равны нулям
    //Если ты до этого создавал таблицу, а потом делал ещё одну, то значения от старой таблицы оставались.
    qDebug() << "Creating new table ";
    for (int i = 0; i < m_actionsCount; ++i)
    {
        std::fill(m_actions[i].getCriteria().begin(), m_actions[i].getCriteria().end(), 0);
        for (int j = 0; j < m_criteriasCount; ++j)
        {
            qDebug() << "action " << i+1 << " criteria " << j+1 << " " << m_actions[i].getCriteria()[j];
        }
    }

    //ВНИМАНИЕ! кусок для заполнения весов единицами. Удалить, когда появится функция для присвоения веса из таблицы
    //На текущий момент это просто заглушка.

    for (int i = 0; i < m_criteriasCount; i++)
    {
        m_criteriasWeight[i] = 1;
    }

}

void MainWindow::PrometheeMethod()
{
    //создаём таблицу разностей.
    //Для каждой альтернативы отнимаем от критериев текущей альтернативы критерии всех остальных альтернатив
    for (int k = 0; k < m_actionsCount; k++)
    {
        for (int i = 0; i < m_actionsCount; i++)
        {
            if (k == i)
            {
                continue;
            }

            for (int j = 0; j < m_criteriasCount; j++)
            {
                //i+1, так как я хочу, чтобы счёт матриц вёлся от единицы
                m_actions[k].getDifferTable()[i+1].push_back((m_actions[k].getCriteria()[j] - m_actions[i].getCriteria()[j]) * m_criteriasMinMax[j]);
            }
        }
    }

    //Кусок для проверки корректности заполнения таблицы разностей
    qDebug() << "Difference Table: ";
    for (int k = 0; k < m_actionsCount; k++)
    {
        for (int i = 0; i < m_actionsCount; i++)
        {
            qDebug() << "Upper action : " << k+1 << "Action in matrix:" << i+1 << m_actions[k].getDifferTable()[i+1];

        }
    }


    //Увеличиваем размер векторов под количество альтернатив. Возможно, это стоит перенести в блок
    //создания таблицы
    for(int i = 0; i < m_actionsCount; i++)
    {
        m_actions[i].getPositivePreferenceIndicies().resize(m_actionsCount);
        m_actions[i].getNegativePreferenceIndicies().resize(m_actionsCount);
    }


    //ВНИМАНИЕ! В целях тестирования разделил на два разных цикла. Затем можно засунуть в один
    qDebug() << "Positive Preference Indicies:";
    for (int k = 0; k < m_actionsCount; k++)
    {
        buildPositivePreferenceIndicies(k);
    }
    qDebug() << "Negative Preference Indicies:";
    for (int k = 0; k < m_actionsCount; k++)
    {
        buildNegativePreferenceIndicies(k);
    }

    //далее идёт подсчёт phi для каждой альтернативы

    for (int i = 0; i < m_actionsCount; i++)
    {
        double sumPositivePreferenceIndicies = 0;
        double sumNegativePreferenceIndicies = 0;
        for (int j = 0; j < m_actionsCount; j++)
        {
             sumPositivePreferenceIndicies += m_actions[i].getPositivePreferenceIndicies()[j];
             sumNegativePreferenceIndicies += m_actions[i].getNegativePreferenceIndicies()[j];
        }
        m_actions[i].getPhiPositive() = sumPositivePreferenceIndicies/(m_actionsCount-1);
        m_actions[i].getPhiNegative() = sumNegativePreferenceIndicies/(m_actionsCount -1);
        m_actions[i].getPhi() = m_actions[i].getPhiPositive() - m_actions[i].getPhiNegative();

        qDebug() << "Action" << i+1 << "Phi Positive" << m_actions[i].getPhiPositive();
        qDebug() << "Action" << i+1 << "Phi Negative" << m_actions[i].getPhiNegative();
        qDebug() << "Action" << i+1 << "Phi " << m_actions[i].getPhi();
    }

}

void MainWindow::buildPositivePreferenceIndicies(int k)
{
    for (int i = 0; i < m_actionsCount; i++)
    {
        for (int j = 0; j < m_criteriasCount; j++)
        {
            if (m_actions[k].getDifferTable()[i+1].empty())
            {
                continue;
            }
            if (m_actions[k].getDifferTable()[i+1][j] <= 0)
            {
                continue;
            }
            if (m_actions[k].getDifferTable()[i+1][j] > 0)
            {
                m_actions[k].getPositivePreferenceIndicies()[i] += 1 * m_criteriasWeight[j];
            }
        }
    }

    //строка для проверки корректности заполнения preferenceIndicies
    qDebug() << "Upper action: " << k+1 << m_actions[k].getPositivePreferenceIndicies();
}

//это ёбаный ужас, сейчас я пойду передохну, а потом подробно опишу, что здесь
void MainWindow::buildNegativePreferenceIndicies(int k)
{
    for (int z = 0; z < m_actionsCount; z++)
    {
        if (k == z)
        {
            continue;
        }

            for (int j = 0; j < m_criteriasCount; j++)
            {
                if (m_actions[z].getDifferTable()[k+1].empty())
                {
                    continue;
                }
                if (m_actions[z].getDifferTable()[k+1][j] <= 0)
                {
                    continue;
                }

                if (m_actions[z].getDifferTable()[k+1][j] > 0)
                {
                    m_actions[k].getNegativePreferenceIndicies()[z] += 1 * m_criteriasWeight[j];
                }
            }
    }

    //строка для проверки корректности заполнения preferenceIndicies
    qDebug() << "Upper action: " << k+1 << m_actions[k].getNegativePreferenceIndicies();
}
void MainWindow::on_pushButton_clicked()
{
    PrometheeMethod();
}

