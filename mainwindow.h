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
    void BuildTable();

private slots:
    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    int m_actions;
    int m_criterias;
};
#endif // MAINWINDOW_H
