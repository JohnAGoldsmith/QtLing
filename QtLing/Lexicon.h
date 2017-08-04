#ifndef CLEXICON_H
#define CLEXICON_H

#include <QMap>
#include <QString>


#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "StringSurrogate.h"

class CLexicon
{
protected:
//    CWordCollection m_Words;
//    CStemCollection m_Stems;
//    CSuffixCollection m_Suffixes;
//    CSignatureCollection m_Signatures;
public:
    CLexicon();
    CWordCollection m_Words;
    CStemCollection m_Stems;
    CSuffixCollection m_Suffixes;
    CSignatureCollection m_Signatures;

public:
    // accessors
    QMap<CStringSurrogate, int> m_Protostems;

    void FindProtostems();
    void CreateStemAffixPairs();
    void AssignSuffixesToStems();

};

#endif // CLEXICON_H
