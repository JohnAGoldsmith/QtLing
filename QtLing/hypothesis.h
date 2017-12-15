#ifndef HYPOTHESIS_H
#define HYPOTHESIS_H

#include "QString"

class CHypothesis
{
    QString m_key;
    QString m_hypothesis_type;
    QString m_signature_1;
    QString m_signature_2;
    QString m_affix_in_signature_1;
    double  m_minimum_saving;
    double  m_maximum_saving;

public:
    QString  get_key() {return m_key;}
};

#endif // HYPOTHESIS_H
