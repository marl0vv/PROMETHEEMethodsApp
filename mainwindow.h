#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_actionNew_triggered();

private:
    const int m_defaultColumnCount = 2;
    const int m_defaultRowCount = 13;
    Ui::MainWindow *ui;
    int m_actions;
    int m_criterias;
};
#endif // MAINWINDOW_H
