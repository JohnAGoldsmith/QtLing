#ifndef CLEXICON_H
#define CLEXICON_H

#include <QMap>
#include <QMapIterator>
#include <QString>
#include <QList>
#include <QPair>
#include "SignatureCollection.h"
#include "Typedefs.h"

class CWordCollection;
class CStemCollection;
class CSuffixCollection;


//typedef  QPair<sig_string, CSignature*>                    sig_and_pointer;


class sig_tree_edge{
public:
    CSignature* sig_1;
    CSignature* sig_2;
    morph_t     morph;
    word_t      word;
    QList<word_t> words;
    sig_tree_edge();
    sig_tree_edge(CSignature* sig1, CSignature* sig2,morph_t m,word_t w)
    {
        sig_1 = sig1;
        sig_2 = sig2;
        morph = m;
        word = w;
    };
    sig_tree_edge(CSignature* sig1, CSignature* sig2,morph_t m)
    {
        sig_1 = sig1;
        sig_2 = sig2;
        morph = m;
    };
    QString label() {return morph + "/" + sig_1->get_key() + "/" + sig_2->get_key(); }

};


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
    CSignatureCollection*           m_ResidualSignatures;   /*!<  the information we have about stems which we have not yet integrated into a morphological system. */
    CStemCollection *               m_ResidualStems;
    QMap<QString,int>               m_RawSuffixes;
    QList<sig_tree_edge*>           m_SigTreeEdgeList; /*!< the sig_tree_edges in here contain only one word associated with each. */
    QMap<QString, sig_tree_edge*>   m_SigTreeEdgeMap;  /*!< the sig_tree_edges in here contain lists of words associated with them. */


public:
    CLexicon();
public:
    // accessors and protostems


    CWordCollection*                            GetWordCollection()         { return m_Words; }
    CStemCollection*                            GetStemCollection()         { return m_Stems; }
    CSuffixCollection*                          GetSuffixCollection()       { return m_Suffixes; }
    CWordCollection *                           get_words()                 { return m_Words;}
    CSignatureCollection*                       get_signatures()            { return m_Signatures;}
    CSignatureCollection*                       GetSignatureCollection()    { return m_Signatures; }
    CSignatureCollection *                      get_residual_signatures()   { return m_ResidualSignatures;}
    QList<QPair<QString,QString>>*              GetParses()                 { return m_Parses;}
    QMap<QString,int>*                          get_protostems()            { return &m_Protostems;}
    void                                        compute_sig_tree_edges();
    QList<sig_tree_edge*> *                     get_sig_tree_edges()        { return &m_SigTreeEdgeList;}
    QMap<QString, sig_tree_edge*>    *          get_sig_tree_edge_map()     { return & m_SigTreeEdgeMap;}
    QListIterator<sig_tree_edge*>    *          get_sig_tree_edge_list_iter();
    QMapIterator<QString, sig_tree_edge*> *     get_sig_tree_edge_map_iter();

public:
    // insert functions here
    void Crab_1();
    void FindProtostems();
    void CreateStemAffixPairs();
    void AssignSuffixesToStems();
    void PurifyResidualSignatures();
    void compute_sig_tree_edge_map();
    void find_good_signatures_inside_raw_signature(bool FindSuffixesFlag);
    void replace_parse_pairs_from_current_signature_structure(bool FindSuffixesFlag=true);

};

#endif // CLEXICON_H
