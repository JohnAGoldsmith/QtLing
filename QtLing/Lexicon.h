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
class CPrefixCollection;
class QProgressBar;


class simple_sig_tree_edge{
public:
    CSignature*         sig_1;
    CSignature*         sig_2;
    morph_t             morph;
    word_t              word; // get rid of word, stem_1, stem_2, replayed by word_stems;
    stem_t              stem_1;
    stem_t              stem_2;
    simple_sig_tree_edge();
    simple_sig_tree_edge(CSignature* sig1, CSignature* sig2,morph_t m,word_t w, stem_t stem1, stem_t stem2)
    {
        sig_1 = sig1;
        sig_2 = sig2;
        morph = m;
        word = w;
        stem_1 = stem1;
        stem_2 = stem2;
    };
    QString label() {return morph + "/" + sig_1->get_key() + "/" + sig_2->get_key(); }
};



class sig_tree_edge{
public:
    CSignature* sig_1;
    CSignature* sig_2;
    morph_t     morph;
    QList<word_t> words;  // will be removed?
    QList<QPair<stem_t, stem_t>* > stem_pairs; // will be removed?
    QList< word_stem_pair_pair * >  shared_items;  // will be removed
    QList<word_stem_struct*>         shared_word_stems;

    sig_tree_edge();
    sig_tree_edge(CSignature* sig1, CSignature* sig2,morph_t m,word_t this_word, stem_t stem1, stem_t stem2)
    {
        sig_1 = sig1;
        sig_2 = sig2;
        morph = m;
        word_stem_struct * this_word_stems = new word_stem_struct;
        this_word_stems->word = this_word;
        this_word_stems->stem_1 = stem1;
        this_word_stems->stem_2 = stem2;
        shared_word_stems.append(this_word_stems);
    };

    QString label() {return morph + "/" + sig_1->get_key() + "/" + sig_2->get_key(); }
};


class CLexicon
{
protected:
    CWordCollection *               m_Words;
    CStemCollection *               m_Stems;
    CSuffixCollection *             m_Suffixes;
    CPrefixCollection *             m_Prefixes;
    CSignatureCollection *          m_Signatures;
    CSignatureCollection *          m_PrefixSignatures;
    CWordCollection *               m_Compounds;
    QList<QPair<QString,QString>> * m_Parses;
    QMap<QString,int>               m_Parse_map;
    QMap<QString, int>              m_Protostems;

    bool                            m_SuffixesFlag;


    CSignatureCollection*           m_ParaSignatures;   /*!<  the information we have about stems which we have not yet integrated into a morphological system. */
    CSuffixCollection *             m_ParaSuffixes;
    CStemCollection *               m_ResidualStems;
    CSignatureCollection *          m_ResidualPrefixSignatures;
    CStemCollection *               m_StemsFromSubsignatures;
    CSignatureCollection*           m_Subsignatures;
    QList<simple_sig_tree_edge*>    m_SigTreeEdgeList; /*!< the sig_tree_edges in here contain only one word associated with each. */
    QMap<QString, sig_tree_edge*>   m_SigTreeEdgeMap;  /*!< the sig_tree_edges in here contain lists of words associated with them. */
    QProgressBar*                   m_ProgressBar;




public:
    CLexicon(bool suffix_flag = true);
public:
    // accessors and protostems


    CWordCollection*                            get_word_collection()       { return m_Words; }
    CStemCollection*                            GetStemCollection()         { return m_Stems; }
    CSuffixCollection*                          GetSuffixCollection()       { return m_Suffixes; }
    CSuffixCollection*                          get_suffixes()              {return m_Suffixes;}
    CStemCollection *                           get_stems()                 { return m_Stems;}
    void                                        dump_suffixes(QList<QString>*);
    CWordCollection *                           get_words()                 { return m_Words;}
    CSignatureCollection*                       get_signatures()            { return m_Signatures;}
    CSignatureCollection*                       get_prefix_signatures()     { return m_PrefixSignatures;}
    CSignatureCollection*                       GetSignatureCollection()    { return m_Signatures; }
    CSignatureCollection *                      get_residual_signatures()   { return m_ParaSignatures;}
    CSuffixCollection*                          get_parasuffixes()          { return m_ParaSuffixes;}
    void                                        collect_parasuffixes();


    QList<QPair<QString,QString>>*              GetParses()                 { return m_Parses;}
    QMap<QString,int>*                          get_protostems()            { return &m_Protostems;}
    void                                        compute_sig_tree_edges();
    QList<simple_sig_tree_edge*> *              get_sig_tree_edges()        { return &m_SigTreeEdgeList;}
    QMap<QString, sig_tree_edge*>    *          get_sig_tree_edge_map()     { return & m_SigTreeEdgeMap;}
    QListIterator<simple_sig_tree_edge*>    *          get_sig_tree_edge_list_iter();
    QMapIterator<QString, sig_tree_edge*> *     get_sig_tree_edge_map_iter();
    void                                        set_progress_bar (QProgressBar * pPB) { m_ProgressBar = pPB;}
    void                                        set_prefixes_flag()         { m_SuffixesFlag = false;}
    bool                                        get_suffix_flag()         { return m_SuffixesFlag; }
public:
    // insert functions here
    void Crab_1();
    void Crab_2();
    void FindProtostems();
    void CreateStemAffixPairs();
    void AssignSuffixesToStems();
    void ReSignaturizeWithKnownAffixes();
    void FindGoodSignaturesInsideParaSignatures();
    void compute_sig_tree_edge_map();
    void replace_parse_pairs_from_current_signature_structure(bool FindSuffixesFlag=true);

};

#endif // CLEXICON_H
