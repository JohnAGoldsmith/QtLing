#ifndef CLEXICON_H
#define CLEXICON_H

#include <QMap>
#include <QString>
#include "SignatureCollection.h"

class CWordCollection;
class CStemCollection;
class CSuffixCollection;
class CStringSurrogate;

class CLexicon
{
protected:
    CWordCollection *m_Words;
    CStemCollection *m_Stems;
    CSuffixCollection *m_Suffixes;
    CSignatureCollection *m_Signatures;
public:
    CLexicon();
public:
    // accessors and protostems
    QMap<CStringSurrogate, int> m_Protostems;

    CWordCollection*      GetWordCollection() { return m_Words; }
    CStemCollection*      GetStemCollection() { return m_Stems; }
    CSuffixCollection*    GetSuffixCollection() { return m_Suffixes; }
    CSignatureCollection* GetSignatureCollection()     { return m_Signatures; }
public:
    // insert functions here
    void FindProtostems();
    void CreateStemAffixPairs();
    void AssignSuffixesToStems();
};

#endif // CLEXICON_H
