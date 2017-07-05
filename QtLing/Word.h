#ifndef CWORD_H
#define CWORD_H

#include "Parse.h"
#include "StringSurrogate.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include "generaldefinitions.h"


class CWord : public CParse
{
protected:
    QString m_Word;
    int m_WordCount;

public:
    CWord(CStringSurrogate ssWord);
    CWord(CWord&);

public:
    //Accessors
    int                     GetWordCount() const { return m_WordCount; }
    QString                 GetWord()      const { return m_Word; }

    void                    IncrementWordCount(int n = 1);

};

#endif // CWORD_H
