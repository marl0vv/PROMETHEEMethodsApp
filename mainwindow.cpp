#include "mainwindow.h"
#include "Drawing.h"
#include "newproblemdialog.h"
#include "helpdialog.h"
#include "Actions.h"
#include "qapplication.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLineEdit>
#include <QMessageBox>
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Изменение комбобокса в разделе "Шкала"
//устанавливает спинбокс для получения обычных числовых значений
//для критериев в разделе альтернатив
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
//Изменение комбобокса в разделе "Шкала"
//устанавливает спинбокс для получения обычных числовых значений,
//но содержит рублёвый префикс, для демонстрации того, что содержимое - валюта
//для критериев в разделе альтернатив
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
//Изменение комбобокса в разделе "Шкала"
//Устанавливает комбобокс в котором содержатся качественные значения
//для критериев в разделе альтернатив
void MainWindow::setColumnTypeQuality(int column)
{
    for (int i = 0; i < m_actionsCount; i++)
    {
        QComboBox* actionValue = new QComboBox();
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
//Функция, которая отслеживает, какое значение было установлено
//в комбобоксе в разделе "Шкала". Вызывает нужную функцию, для
//установки корректного виджета для критериев в разделе альтернатив
void MainWindow::onTypeComboBoxChanged(int index)
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

//Изменение значения критерия,
//если в критерии альтернативы находится спинбокс
void MainWindow::onCriteriaSpinBoxChanged(double d)
{
    int row = sender()->property("row").toInt();
    int column = sender()->property("column").toInt();

    //Магические константы были ошибкой.
    //Нужно было сделать отдельную переменную, но уже поздно.
    //-13 потому что первая альтернатива находится в 13 строке
    //таким образом, нулевая альтернатива будет при row-13, с column -2 аналогично
    m_actions[row-13].getCriteria()[column-2] = d;

    //тестовый кусок для вывода каждой альтернативы
    qDebug() << "row: " << row << " column: " << column << " value: " << d;
    for (int i = 0; i < m_actionsCount; ++i)
    {
        for (int j = 0; j < m_criteriasCount; ++j)
        {
            qDebug() << "action " << i+1 << " criteria " << j+1 << " " << m_actions[i].getCriteria()[j];
        }
    }

    //При изменении значений у критериев, нужно пересчитывать
    //все характеристики и метод PROMETHEE
    statsFindMax();
    statsFindMin();
    statsFindAverage();
    statsFindDeviation();
    PrometheeMethod();
}

//Аналогично предыдущей функции, но для комбобокса.
void MainWindow::onCriteriaComboBoxChanged(int index)
{
    int row = sender()->property("row").toInt();
    int column = sender()->property("column").toInt();

    //значение индекса - это числовое значение качества
    m_actions[row-13].getCriteria()[column-2] = index;

    //тестовый кусок для вывода каждой альтернативы
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
    PrometheeMethod();
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

//Аналогично функции для нахождения максимального, только другой знак
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

//Смотри формулу среднеквадратического отклонения в Гугле
void MainWindow::statsFindDeviation()
{
    double average;
    double deviation;
    for (int j = 0; j < m_criteriasCount; j++)
    {
        double sum = 0;
        for (int i = 0; i < m_actionsCount; i++)
        {
            sum += m_actions[i].getCriteria()[j];
        }
        average = (double) sum / (double) m_actionsCount;

        //sumOfPowsOfDiff - сумма квадратов разностей отдельного значения выборки и
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

//По изменению значения комбобокса определяем, наиболее предпочтительно
//минимальное или максимальное значение. Если минимальное, то затем умножим элементы в таблице разностей
//на -1
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

    PrometheeMethod();
}

void MainWindow::onWeightComboBoxChanged(double d)
{
    int column = sender()->property("column").toInt();
    int sum = 0;
    m_criteriasTableWeight[column-2] = d;
    for (int j = 0; j < m_criteriasCount; j++)
    {
        sum += m_criteriasTableWeight[j];
        qDebug() << "Table Weight " << j+1 << m_criteriasTableWeight[j];
    }
    for (int j = 0; j < m_criteriasCount; j++)
    {
        m_criteriasWeight[j] = m_criteriasTableWeight[j] / sum;
        qDebug() << " Weight " << j+1 << m_criteriasWeight[j];
    }

    PrometheeMethod();
}

//Имя альтернативы из виджета переносится в массив
//Затем эти названия используются для обозначения прямых на графическом отображении
void MainWindow::onActionNameLineEditChanged(const QString &text)
{
    int row = sender()->property("row").toInt();
    qDebug() << "ROW: " << row;
    m_actions[row - 13].getName() = text;

    for (int i = 0; i < m_actionsCount; i++)
    {
        qDebug() << "Action  " << i+1  << " name: "<< m_actions[i].getName();
    }
    PrometheeMethod();
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

    //Дальше идёт заполнение остальных столбцов и таблиц заглушками,
    //которые пользователь будет менять на свои значения

    //setItem идёт в 2+i, потому что 2 - это имя строки
    for (int i = 0; i < m_criteriasCount; i++)
    {
        //заглушки для имён критериев
        QString criteriaName = "Критерий" + QString::number(i+1);
        ui->tableWidget->setItem(1, 2+i, new QTableWidgetItem(criteriaName));


        //В столбцы строки "Шкала" помещаются комбобоксы.
        //Эти комбобоксы отражают то, в какой шкале измеряется критерий, определяемый этим столбцом.
        //Сигнал изменения индекса комбобокса отправляется в слот onTypeComboBoxChanged
        //Этот слот затем использует функции для изменения шкал, в которых измеряются критерии для каждой альтернативы.
        QComboBox* comboScale = new QComboBox();
        comboScale->setProperty("column", QVariant(2+i));
        connect(comboScale, &QComboBox::currentIndexChanged, this, &MainWindow::onTypeComboBoxChanged);
        ui->tableWidget->setCellWidget(2, 2+i, comboScale);
        comboScale->addItem("Числовая");
        comboScale->addItem("Денежная");
        comboScale->addItem("Качественная");

        //заглушка для единиц измерения
        ui->tableWidget->setItem(3, 2+i, new QTableWidgetItem("единица измерения"));

        //Изменение значений этого комбобокса влияет на то, минимальное или максимальное значение будет
        //использоваться для высчитывания при применении методов
        QComboBox* minOrMax = new QComboBox();
        minOrMax->setProperty("column", QVariant(2+i));
        connect(minOrMax, &QComboBox::currentIndexChanged, this, &MainWindow::onMinOrMaxComboBoxChanged);
        ui->tableWidget->setCellWidget(5, 2+i, minOrMax);
        minOrMax->addItem("Минимум");
        minOrMax->addItem("Максимум");

        //вес
        QDoubleSpinBox* comboWeight = new QDoubleSpinBox();
        comboWeight->setProperty("column", QVariant(2+i));
        comboWeight->setValue(1);
        std::fill(m_criteriasTableWeight.begin(), m_criteriasTableWeight.end(), 1);
        qDebug() << "Table Weight" << m_criteriasTableWeight;
        m_criteriasWeight[i] = 1/ m_criteriasCount;
        connect(comboWeight, &QDoubleSpinBox::valueChanged, this, &MainWindow::onWeightComboBoxChanged);
        ui->tableWidget->setCellWidget(6, 2+i, comboWeight);


        ui->tableWidget->setItem(8, 2+i, new QTableWidgetItem("n/a"));
        ui->tableWidget->setItem(9, 2+i, new QTableWidgetItem("n/a"));
        ui->tableWidget->setItem(10, 2+i, new QTableWidgetItem("n/a"));
        ui->tableWidget->setItem(11, 2+i, new QTableWidgetItem("n/a"));
    }

    //Устанавливается виджет с именем альтернативы, связывается со слотом,
    //который при изменении имени, добавляет изменённое имя в массив, который передаётся в класс
    //для рисования
    for (int i = 0; i < m_actionsCount; i++)
    {
        QLineEdit* actionName = new QLineEdit();
        actionName->setProperty("row", QVariant(13+i));
        connect(actionName, &QLineEdit::textChanged, this, &MainWindow::onActionNameLineEditChanged);
        QString actionNameString = "Альтернатива" + QString::number(i+1);
        actionName->setText(actionNameString);
        ui->tableWidget->setCellWidget(13+i, 1, actionName);
    }

    //Заполняем значения критериев у альтернатив спинбоксами по умолчанию
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
        }
    }

}

//вызываетсф при нажатии Файл -> Создать новый
void MainWindow::on_actionNew_triggered()
{
    NewProblemDialog newProblemDialog(nullptr);
    newProblemDialog.setModal(true);
    if (newProblemDialog.exec() == 0) //если окно закрыто, то программа возвращается к исходному состоянию
    {
        return;
    }
    m_actionsCount = newProblemDialog.getActions();
    m_criteriasCount = newProblemDialog.getCriterias();
    m_criteriasMinMax.resize(m_criteriasCount);
    m_criteriasWeight.resize(m_criteriasCount);
    m_criteriasTableWeight.resize(m_criteriasCount);

    //Расширение вектора альтернатив, созданного в классе mainwindow,
    //до размеров m_actionsCount и m_criteriasCount
    //Затем расширение вектора положительных предпочтений,
    //вектора отрицательных предпочтений и вектора критерий для каждой альтернативы
    m_actions.resize(m_actionsCount, Actions());
    for(int i = 0; i < m_actions.size(); i++)
    {
        m_actions[i].getCriteria().resize(m_criteriasCount, 0);
        m_actions[i].getPositivePreferenceIndicies().resize(m_actionsCount);
        m_actions[i].getNegativePreferenceIndicies().resize(m_actionsCount);
    }

    buildTable();

    //в этом кусочке векторы объекта m_actions заполняются нулями,
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

}

void MainWindow::PrometheeMethod()
{
    //Обнуляем массивы, чтобы при каждом новом использовании метода
    //не дублировалась старая информация
    for (int i = 0; i < m_actionsCount; i++)
    {
        m_actions[i].getDifferTable().clear();
        for (int j = 0; j < m_actionsCount; j++)
        {
            m_actions[i].getPositivePreferenceIndicies()[j] = 0;
            m_actions[i].getNegativePreferenceIndicies()[j] = 0;
        }
    }
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

    //ВНИМАНИЕ! В целях тестирования разделил на два разных цикла. Потом можно засунуть в один
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
        m_actions[i].getPhiNegative() = sumNegativePreferenceIndicies/(m_actionsCount-1);
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

//Сложный момент. k, которую мы передаём данной функции - это то, для какой альтернативы мы в данный момент ищем строки в матрицах разности других альтернатив
//(первый внешний цикл)
//z - это номер матрицы разности другой альтернативы, в которой мы ищем строки, относящиеся к альтернативе k
//j - это столбец в искомой строке матрицы z и эта строка относится к альтернативе k
//Проще это представлять, если отобразить все матрицы в экселе.
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

void MainWindow::on_action_2_triggered()
{
    if (m_actions.empty())
    {
        QMessageBox::warning(this, "Ошибка!","Для отображения Рейтинга PROMETHEE сначала необходимо сгенерировать таблицу!");
        return;
    }
    Drawing *drawing = new Drawing(nullptr, m_actions, m_actionsCount);
    drawing->setAttribute(Qt::WA_DeleteOnClose);
    drawing->setWindowModality(Qt::ApplicationModal);
    drawing->show();
}


void MainWindow::on_action_help_triggered()
{
    helpdialog helpdialog(nullptr);
    helpdialog.setModal(true);
    helpdialog.exec();
}

