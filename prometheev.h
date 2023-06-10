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

private:
    Ui::PrometheeV *ui;

    std::vector<Actions> m_actions;
};

#endif // PROMETHEEV_H
