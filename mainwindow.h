#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Actions.h"

#include <QMainWindow>
#include <vector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void buildTable();
    void setColumnTypeNum(int column);
    void setColumnTypeQuality(int column);
    void setColumnTypeMoney(int column);

private slots:
    void on_actionNew_triggered();
    void onComboBoxChanged(int index);
    void onCriteriaSpinBoxChanged(double d);
    void onCriteriaComboBoxChanged(int index);

private:
    const int m_defaultColumnCount = 2;
    const int m_defaultRowCount = 13;
    Ui::MainWindow *ui;
    int m_actionsCount;
    int m_criteriasCount;
    std::vector<Actions> m_actions;
};
#endif // MAINWINDOW_H
