#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newproblemdialog.h"

#include <QPushButton>
#include <QComboBox>


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

void MainWindow::buildTable()
{
    //Создаём таблицу и заголовки
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(m_criterias + m_defaultColumnCount);
    ui->tableWidget->setRowCount(m_actions + m_defaultRowCount);


    QFont font("Roboto", 8, QFont::Bold);

    //выделяем серым заголовок Свойств
    for (int i = 0; i < m_criterias + m_defaultColumnCount; i++)
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
    for (int i = 0; i < m_criterias + m_defaultColumnCount; i++)
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
    for (int i = 0; i < m_criterias + m_defaultColumnCount; i++)
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
    for (int i = 0; i < m_criterias; i++)
    {
        QString criteriaName = "Критерий" + QString::number(i+1);
        ui->tableWidget->setItem(1, 2+i, new QTableWidgetItem(criteriaName));

        //надо бы потом это delete, наверное
        //вообще изменение этого комбобокса должно будет влиять на строку
        //значения у actions. Скорее всего это нужно будет делать через слоты.
        //Пока что построю остальную таблицу, там посмотрим.
        QComboBox* comboScale = new QComboBox();
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
    for (int i = 0; i < m_actions; i++)
    {
        QString actionName = "Критерий" + QString::number(i+1);
        ui->tableWidget->setItem(13+i, 1, new QTableWidgetItem(actionName));
    }

    //заполняем строки с actions и criterias
    for (int i = 0; i < m_actions; i++)
    {
        for (int j = 0; j < m_criterias; j++)
        {
            ui->tableWidget->setItem(13+i, 2+j, new QTableWidgetItem("n/a"));
        }
    }

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

