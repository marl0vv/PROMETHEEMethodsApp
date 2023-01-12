#pragma once

#include <vector>
#include <unordered_map>
#include <QString>

class Actions
{
public:
    Actions() = default;
    Actions(int criterias)
    {
        m_criterias.resize(criterias);
    }
    std::vector<double> &getCriteria() {return m_criterias;}
    std::unordered_map<std::size_t, std::vector<double>> &getDifferTable() {return differTable;}
    std::vector<double> &getPositivePreferenceIndicies() {return m_positivePreferenceIndices;}
    std::vector<double> &getNegativePreferenceIndicies() {return m_negativePreferenceIndices;}
    double &getPhiPositive() {return m_phiPositive;}
    double &getPhiNegative() {return m_phiNegative;}
    double &getPhi() {return m_phi;}
    QString &getName() {return name;}
private:
    std::vector<double> m_criterias;
    std::unordered_map<std::size_t, std::vector<double>> differTable;
    std::vector<double> m_positivePreferenceIndices;
    std::vector<double> m_negativePreferenceIndices;
    QString name;
    double m_phiPositive;
    double m_phiNegative;
    double m_phi;
};


