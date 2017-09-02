#include "Signature.h"

CSignature::CSignature(CStringSurrogate ssWord) : CParse(ssWord), m_Signature(ssWord.GetKey()), m_Affixes() {}

CSignature::CSignature(CSignature& signature) {
    m_Signature = signature.GetSignature();
    m_Affixes = signature.GetList();
}

CSignature::CSignature(QString  qstring) {
    m_Signature = qstring;
}



void CSignature::add_stem_pointer(CStem* pStem)
{
    m_Stems.append(pStem);
    
}

QString CSignature::display()
{
    return m_Signature;
}

QString CSignature::display_stems()
{
    QString outstring;
    CStem *qStem;
    foreach (qStem, m_Stems){
        outstring += " " + qStem->display();
    }
    return outstring;
}
