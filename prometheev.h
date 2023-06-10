#ifndef PROMETHEEV_H
#define PROMETHEEV_H

#include <QWidget>
#include <QLineEdit>
#include <QString>

#include "Actions.h"

namespace Ui {
class PrometheeV;
}

class PrometheeV : public QWidget
{
    Q_OBJECT

public:
    explicit PrometheeV(QWidget *parent, std::vector<Actions> &actions);
    ~PrometheeV();
    void buildTable();
    int getActionsCount() const {return m_actions.size();}

private:
    Ui::PrometheeV *ui;

    std::vector<Actions> m_actions;
    const int m_defaultColumnCount = 2;
    const int m_defaultRowCount = 3;
};

#endif // PROMETHEEV_H
