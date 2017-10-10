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


typedef  QPair<CStem*,CSignature*>  stem_sig_pair;
typedef  QPair<stem_sig_pair*,  stem_sig_pair*> pair_of_stem_sig_pairs;
typedef  QPair<QString, pair_of_stem_sig_pairs*> five_tuple_sig_diffs;

class CLexicon
{
protected:
    CWordCollection *               m_Words;
    CStemCollection *               m_Stems;
    CSuffixCollection *             m_Suffixes;
    CSignatureCollection *          m_Signatures;
    QList<QPair<QString,QString>> * m_Parses;
    QMap<QString, int>              m_Protostems;
    QList<five_tuple_sig_diffs>     m_Multiparses;
public:
    CLexicon();
public:
    // accessors and protostems


    CWordCollection*      GetWordCollection() { return m_Words; }
    CStemCollection*      GetStemCollection() { return m_Stems; }
    CSuffixCollection*    GetSuffixCollection() { return m_Suffixes; }
    CWordCollection *        get_words() {return m_Words;}
    CSignatureCollection*    get_signatures() { return m_Signatures;}
    CSignatureCollection* GetSignatureCollection()     { return m_Signatures; }
    QList<QPair<QString,QString>>*           GetParses() {return m_Parses;}
    QMap<QString,int>*   GetProtostems() {return &m_Protostems;}
public:
    // insert functions here
    void Crab_1();
    void FindProtostems();
    void CreateStemAffixPairs();
    void AssignSuffixesToStems();
    void ComputeMultiparses();
    void SortMultiparses(int styleno);
};

#endif // CLEXICON_H
