#ifndef CSUFFIX_H
#define CSUFFIX_H

#include "Parse.h"
#include "StringSurrogate.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include "generaldefinitions.h"

class CSuffix : public CParse
{
protected:
    QString m_Suffix;
    int m_frequency;
public:
    CSuffix(CStringSurrogate ssWord);
    CSuffix(CSuffix&);
public:
    //Accessors
    QString GetSuffix() const { return m_Suffix;    }
    int     GetFrequency()    { return m_frequency; }
    void    SetFrequency(int frequency) { m_frequency = frequency; }
};

#endif // CSUFFIX_H
