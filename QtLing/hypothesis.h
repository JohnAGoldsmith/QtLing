#ifndef HYPOTHESIS_H
#define HYPOTHESIS_H

#include "QString"
#include "generaldefinitions.h"

class CHypothesis
{
    QString             m_key;
    eHypothesisType     m_hypothesis_type;
    sigstring_t         m_signature_1;
    sigstring_t         m_signature_2;
    affix_t             m_affix;
    double              m_minimum_saving;
    double              m_maximum_saving;

public:
    CHypothesis (eHypothesisType,  )
    QString  get_key() {return m_key;}
};

#endif // HYPOTHESIS_H
