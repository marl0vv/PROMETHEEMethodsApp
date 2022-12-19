#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newproblemdialog.h"
#include "Actions.h"

#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDebug>


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
        ui->tableWidget->setCellWidget(5, 2+i, minOrMax);
        minOrMax->addItem("Максимум");
        minOrMax->addItem("Минимум");


        //вес
        ui->tableWidget->setItem(6, 2+i, new QTableWidgetItem(QString::number(1)));


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

    buildTable();

    //идёт расширение вектора альтернатив, созданного в классе mainwindow
    //до размеров m_actionsCount и m_criteriasCount
    m_actions.resize(m_actionsCount, Actions());
    for(int i = 0; i < m_actions.size(); i++)
    {
        m_actions[i].getCriteria().resize(m_criteriasCount, 0);
    }

    for (int i = 0; i < m_actionsCount; ++i)
    {
        for (int j = 0; j < m_criteriasCount; ++j)
        {
            qDebug() << "action " << i+1 << " criteria " << j+1 << " " << m_actions[i].getCriteria()[j];
        }
    }
}


