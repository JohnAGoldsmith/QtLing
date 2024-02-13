#ifndef CPARSE_H
#define CPARSE_H

#include <QString>

class CParse
{

    QString  m_stem;
    QString  m_affix;
    bool     m_suffix_flag;
public:
    CParse(QString, QString, bool suffix_flag = true);
    QString get_stem() {return m_stem;}
    QString get_affix() {return m_affix;}
    QString display();
    QString get_word();
    QString display_with_gap();
    QString display_full(); //like:  book NULL SUFFIXAL
    QString get_left_string();
    QString get_right_string();
};



#endif // CPARSE_H
