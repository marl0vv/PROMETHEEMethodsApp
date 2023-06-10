#include "prometheev.h"
#include "ui_prometheev.h"

PrometheeV::PrometheeV(QWidget *parent, std::vector<Actions> &actions) :
    QWidget(parent),
    ui(new Ui::PrometheeV)
{
    ui->setupUi(this);
    m_actions = actions;
}

PrometheeV::~PrometheeV()
{
    delete ui;
}
