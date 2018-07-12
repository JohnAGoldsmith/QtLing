#ifndef CPARSE_H
#define CPARSE_H

#include <QString>

class CParse
{

    QString  m_string1;
    QString  m_string2;
    bool      m_suffix_flag;
public:
    CParse(QString, QString, bool suffix_flag = true);
    QString get_string1() {return m_string1;}
    QString get_string2() {return m_string2;}
};



#endif // CPARSE_H
