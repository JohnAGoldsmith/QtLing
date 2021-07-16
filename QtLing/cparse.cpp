#include "cparse.h"




CParse::CParse(QString stem, QString affix, bool suffix_flag){
    m_stem = stem;
    m_affix = affix;
    m_suffix_flag = suffix_flag;
}

QString CParse::display(){
    if (m_affix == "NULL"){
        return m_stem;
    }
    if (m_suffix_flag){
        return m_stem + m_affix;
    }
    return m_affix + m_stem;
}
QString CParse::display_with_gap(){
    if(m_affix == "NULL" ){
        return m_stem;
    }
    if (m_suffix_flag){
        return m_stem + " " + m_affix;
    }
    return m_affix + " " + m_stem;
}
QString CParse::get_left_string(){
    if(m_suffix_flag){
        return m_stem;
    } else{
        return m_affix;
    }
}
QString CParse::get_right_string(){
    if(m_suffix_flag){
        return m_affix;
    } else{
        return m_stem;
    }
}
