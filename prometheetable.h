#ifndef PROMETHEETABLE_H
#define PROMETHEETABLE_H

#include <QWidget>
#include <mainwindow.h>

namespace Ui {
class PrometheeTable;
}

class PrometheeTable : public QWidget
{
    Q_OBJECT

public:
    //explicit PrometheeTable(QWidget *parent = nullptr);
    explicit PrometheeTable(QWidget *parent, const std::vector<Actions> &actions, const int &count);
    ~PrometheeTable();
    void phiSort();
    void buildTable();

private:
    Ui::PrometheeTable *ui;
    std::vector<Actions> m_actions;
    int m_actionsCount;
};

#endif // PROMETHEETABLE_H
