#pragma once

#include <vector>

class Actions
{
public:
    Actions() = default;
    Actions(int criterias)
    {
        m_criterias.resize(criterias);
    }
    std::vector<double> &getCriteria() {return m_criterias;}
private:
    std::vector<double> m_criterias;
};


