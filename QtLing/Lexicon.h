#ifndef CLEXICON_H
#define CLEXICON_H

#include <QMap>
#include <QMapIterator>
#include <QString>
#include <QList>
#include <QPair>
#include "SignatureCollection.h"

class CWordCollection;
class CStemCollection;
class CSuffixCollection;
class CStringSurrogate;

class CLexicon
{
protected:
    CWordCollection * m_Words;
    CStemCollection * m_Stems;
    CSuffixCollection * m_Suffixes;
    CSignatureCollection * m_Signatures;
    QList<QPair<QString,QString>> *m_Parses;
    QMap<QString, int> m_Protostems;
public:
    CLexicon();
public:
    // accessors and protostems


    CWordCollection*      GetWordCollection() { return m_Words; }
    CStemCollection*      GetStemCollection() { return m_Stems; }
    CSuffixCollection*    GetSuffixCollection() { return m_Suffixes; }
    CSignatureCollection*    GetSignatures() { return m_Signatures;}
    CSignatureCollection* GetSignatureCollection()     { return m_Signatures; }
    QList<QPair<QString,QString>>*           GetParses() {return m_Parses;}
    QMap<QString,int>*   GetProtostems() {return &m_Protostems;}
public:
    // insert functions here
    void Crab_1();
    void FindProtostems();
    void CreateStemAffixPairs();
    void AssignSuffixesToStems();
};

#endif // CLEXICON_H
