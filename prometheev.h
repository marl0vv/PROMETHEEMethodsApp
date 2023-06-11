#ifndef PROMETHEEV_H
#define PROMETHEEV_H

#include <QWidget>

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
    void prometheeV();


private slots:
    void on_pushButtonAddConstraint_clicked();
    void onConstraintCoeffsSpinBoxChanged(double d);
    void onConstraintCompareComboBoxChanged(int index);
    void onConstraintValueSpinBoxChanged(double d);

private:
    Ui::PrometheeV *ui;

    std::vector<Actions> m_actions;
    int m_defaultColumnCount = 4;
    int m_defaultRowCount = 3;
    std::vector<std::vector<double>> constraint_coeffs;
    std::vector<double> bounds;
    std::vector<int> constraintCompare;
};

#endif // PROMETHEEV_H
