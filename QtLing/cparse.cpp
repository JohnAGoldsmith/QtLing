#include "cparse.h"




CParse::CParse(QString stem, QString affix, bool suffix_flag){
    m_stem = stem;
    m_affix = affix;
    m_suffix_flag = suffix_flag;
}

QString CParse::display(){
    if (m_suffix_flag){
        return m_stem + m_affix;
    }
    return m_affix + m_stem;
}
QString CParse::display_with_gap(){
    if (m_suffix_flag){
        return m_stem + " " + m_affix;
    }
    return m_affix + " " + m_stem;
}
