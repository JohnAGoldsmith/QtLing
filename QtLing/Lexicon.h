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

typedef  QString                    sig_string;
typedef  QPair<CStem*,CSignature*>  stem_sig_pair;
typedef  QPair<stem_sig_pair*,  stem_sig_pair*> pair_of_stem_sig_pairs;
typedef  QPair<sig_string, pair_of_stem_sig_pairs*> five_tuple_sig_diffs;

//typedef  QPair<sig_string, CSignature*>                    sig_and_pointer;
typedef  QPair<CSignature*,CSignature*>      pair_of_sigs;
typedef  QPair<QString, pair_of_sigs*>                  multiparse_edge_label;
typedef  QPair<QList<QString>,multiparse_edge_label*>   multiparse;
class CLexicon
{
protected:
    CWordCollection *               m_Words;
    CStemCollection *               m_Stems;
    CSuffixCollection *             m_Suffixes;
    CSignatureCollection *          m_Signatures;
    QList<QPair<QString,QString>> * m_Parses;
    QMap<QString,int>               m_Parse_map;
    QMap<QString, int>              m_Protostems;
    CSignatureCollection*           m_RawSignatures; // the information we have about stems which we have not yet integrated into a morphological system.
    QMap<QString,int>               m_RawSuffixes;
    QList<five_tuple_sig_diffs*>     m_Multiparses;
    QMap <QString, multiparse_edge_label*>    m_Multiparse_edges; // a map from an "abbreviatioN" of the sigs and edge to the multiparse itself.

public:
    CLexicon();
public:
    // accessors and protostems


    CWordCollection*                            GetWordCollection() { return m_Words; }
    CStemCollection*                            GetStemCollection() { return m_Stems; }
    CSuffixCollection*                          GetSuffixCollection() { return m_Suffixes; }
    CWordCollection *                           get_words() {return m_Words;}
    CSignatureCollection*                       get_signatures() { return m_Signatures;}
    CSignatureCollection*                       GetSignatureCollection()     { return m_Signatures; }
    CSignatureCollection *                      get_raw_signatures() { return m_RawSignatures;}
    QList<QPair<QString,QString>>*              GetParses() {return m_Parses;}
    QMap<QString,int>*                          get_protostems() {return &m_Protostems;}
    QList<five_tuple_sig_diffs*>*               get_multiparses() {return &m_Multiparses;}
    QMap <QString, multiparse_edge_label*> *    get_multiparse_edges() {return & m_Multiparse_edges;}
    void                                        add_multiparse_edges(multiparse*);
public:
    // insert functions here
    void Crab_1();
    void FindProtostems();
    void CreateStemAffixPairs();
    void AssignSuffixesToStems();
    void ComputeMultiparses();
    void find_good_signatures_inside_raw_signature(bool FindSuffixesFlag);
    void replace_parse_pairs_from_current_signature_structure(bool FindSuffixesFlag);
    void SortMultiparses(int styleno);

};

#endif // CLEXICON_H
