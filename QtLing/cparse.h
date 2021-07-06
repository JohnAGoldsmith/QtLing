#ifndef CPARSE_H
#define CPARSE_H

#include <QString>

class CParse
{

    QString  m_stem;
    QString  m_affix;
    bool      m_suffix_flag;
public:
    CParse(QString, QString, bool suffix_flag = true);

    //QString get_string1() {return m_string1;}
    //QString get_string2() {return m_string2;}
    QString get_stem() {return m_stem;}
    QString get_affix() {return m_affix;}
    QString display();
    QString get_word();
    QString display_with_gap();
};



#endif // CPARSE_H
