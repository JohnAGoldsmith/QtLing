#ifndef CSUFFIX_H
#define CSUFFIX_H

#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include "generaldefinitions.h"

class CSuffix
{
protected:
    QString m_key;
    int m_frequency;
public:
    CSuffix(QString ssWord);
    CSuffix(CSuffix&);
public:
    //Accessors
    QString get_key() const {return m_key;}
    QString GetSuffix() const { return m_key;    }
    int     GetFrequency()    { return m_frequency; }
    void    SetFrequency(int frequency) { m_frequency = frequency; }
};

#endif // CSUFFIX_H
