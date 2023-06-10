#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Actions.h"
#include "prometheev.h"

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
    void statsFindMax();
    void statsFindMin();
    void statsFindAverage();
    void statsFindDeviation();
    void buildPositivePreferenceIndicies(int k);
    void buildNegativePreferenceIndicies(int k);
    void PrometheeMethod();
    std::vector<Actions> &getActions() {return m_actions;}
private slots:
    void on_actionNew_triggered();
    void onTypeComboBoxChanged(int index);
    void onCriteriaSpinBoxChanged(double d);
    void onCriteriaComboBoxChanged(int index);
    void onMinOrMaxComboBoxChanged(int index);
    void onWeightComboBoxChanged(double d);
    void onActionNameLineEditChanged(const QString &text);
    void on_action_2_triggered();
    void on_action_help_triggered();
    void on_action_prometheeTable_triggered();

    void on_actionPROMETHEE_V_triggered();

private:
    const int m_defaultColumnCount = 2;
    const int m_defaultRowCount = 13;
    Ui::MainWindow *ui;
    int m_actionsCount;
    int m_criteriasCount;
    std::vector<Actions> m_actions;
    std::vector<double> m_criteriasTableWeight;
    std::vector<double> m_criteriasWeight;
    std::vector<int> m_criteriasMinMax;

};
#endif // MAINWINDOW_H
