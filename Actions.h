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
    std::vector<std::vector<double>> &getDifferTable() {return differTable;}
    std::vector<double> &getPreferenceIndicies() {return m_preferenceIndices;}
private:
    std::vector<double> m_criterias;
    std::vector<std::vector<double>> differTable;
    std::vector<double> m_preferenceIndices;
    double phiPositive;
    double phiNegative;
    double phi;
};


