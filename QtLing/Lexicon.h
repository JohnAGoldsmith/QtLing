#ifndef CLEXICON_H
#define CLEXICON_H

#include <QMap>
#include <QMapIterator>
#include <QString>
#include <QList>
#include <QPair>
#include <QSet>
#include <QStatusBar>
#include "SignatureCollection.h"
#include "Typedefs.h"
class MainWindow;
class CWordCollection;
class CStemCollection;
class CSuffixCollection;
class CPrefixCollection;
class QProgressBar;
class CHypothesis;

class CParse;

//  part of an experiment:
class Collection
{
    eComponentType      m_component_type;

public:
    Collection(eComponentType this_type)        {m_component_type = this_type;}
    QString             get_output();
};

//  end of experiment

//-----------------------------------------------------------------------//
class simple_sig_graph_edge{
//-----------------------------------------------------------------------//
public:
    CSignature*         m_sig_1;
    CSignature*         m_sig_2;
    CLexicon *          m_Lexicon;
    sig_string          m_sig_string_1;
    sig_string          m_sig_string_2;
    double              m_sig_1_entropy = -1;
    double              m_sig_2_entropy = -1;
    morph_t             morph;
    word_t              word;
    stem_t              stem_1;
    stem_t              stem_2;
    simple_sig_graph_edge();
    simple_sig_graph_edge(CLexicon* lexicon, CSignature* pSig1, CSignature* pSig2, sigstring_t sig_string_1, sigstring_t sig_string_2,morph_t m,word_t w, stem_t stem1, stem_t stem2)
    {

        m_sig_1 = pSig1;
        m_sig_2 = pSig2;
        m_Lexicon = lexicon;
        m_sig_string_1 = sig_string_1;
        m_sig_string_2 = sig_string_2;
        morph = m;
        word = w;
        stem_1 = stem1;
        stem_2 = stem2;
    };
    QString label() {return morph + "/" + m_sig_string_1 + "/" + m_sig_string_2; }
};
//-----------------------------------------------------------------------//


//-----------------------------------------------------------------------//
class sig_graph_edge{
//-----------------------------------------------------------------------//
public:
    CSignature* m_sig_1;
    CSignature* m_sig_2;
    CLexicon*   m_lexicon;
    sig_string  m_sig_string_1;
    sig_string  m_sig_string_2;
    double m_sig_1_entropy = -1;
    double m_sig_2_entropy = -1;
    morph_t     morph;
    QMap<QString, word_stem_struct*>         shared_word_stems;
    sig_graph_edge();

    sig_graph_edge(simple_sig_graph_edge this_edge){
             m_lexicon = this_edge.m_Lexicon;
             m_sig_1 = this_edge.m_sig_1;
             m_sig_2 = this_edge.m_sig_2;
             m_sig_string_1  = this_edge.m_sig_string_1;
             m_sig_string_2  = this_edge.m_sig_string_2;
             m_sig_1_entropy = this_edge.m_sig_1_entropy;
             m_sig_2_entropy = this_edge.m_sig_2_entropy;
             morph = this_edge.morph;
             word_stem_struct * this_word_stems = new word_stem_struct;
             this_word_stems->word = this_edge.word;
             this_word_stems->stem_1 = this_edge.stem_1;
             this_word_stems->stem_2 = this_edge.stem_2;
             shared_word_stems[this_word_stems->get_label()] = this_word_stems;
         }
    QString label() {return morph + "/" + m_sig_string_1 + "/" + m_sig_string_2; }
    int     get_number_of_words() {return shared_word_stems.size();}
    CSignature*     get_sig_1() {return m_sig_1;}
    CSignature*     get_sig_2() {return m_sig_2;}
    sig_string      get_sig1_string() { return m_sig_string_1;}
    sig_string      get_sig2_string() { return m_sig_string_2;}
    morph_t         get_morph() {return morph;}


};

//-----------------------------------------------------------------------//
class protostem{
//-----------------------------------------------------------------------//
        QString     m_protostem;
        int         m_start_word;
        int         m_end_word;
public:
        protostem(QString stem, int start_word, int end_word=-1) { m_protostem = stem; m_start_word = start_word; m_end_word = end_word;}
        QString     get_stem() { return m_protostem;}
        int         get_start_word() { return m_start_word;}
        int         get_end_word()   {return m_end_word;}
};

//-----------------------------------------------------------------------//
class CLexicon
//-----------------------------------------------------------------------//

{
protected:
                                                         // this is part of an experiment.
    QMap<QString,eComponentType>    m_category_types;    // part of the experiment. It serves
                                                        // as the principal way in which the Lexicon communicates
                                                        // with the GUI as far as architecture is concerned.
                                                        // Each entry in this must have a pointer to a real Collection (of the sort that follows here):
    MainWindow*                     m_main_window;
    CWordCollection *               m_Words;
    CStemCollection *               m_suffixal_stems;
    CStemCollection *               m_prefixal_stems;
    CSuffixCollection *             m_Suffixes;
    CPrefixCollection *             m_Prefixes;
    CSignatureCollection *          m_Signatures;
    CSignatureCollection *          m_PrefixSignatures;
    CWordCollection *               m_Compounds;
    //QList<QPair<QString,QString>> * m_Parses;
    QList<CParse*> *                 m_Parses;

    QMap<QString,int>               m_Parse_map;
//    QMap<QString, int>              m_suffix_protostems;
    QMap<QString, int>              m_prefix_protostems;

    // m_protostems_2 is used in order to keep track of exactly which interval of words in the word list begins
    // with a particular proto-stem (i.e., a word-beginning). This replaces using a huge signature to store
    // that same information.
    QMap<QString, protostem*>        m_suffix_protostems_2;
    QMap<QString, protostem*>        m_prefix_protostems_2;


    bool                            m_SuffixesFlag;
    CLexicon*                       m_parent_lexicon;

    CSignatureCollection*           m_ParaSignatures;   /*!<  the information we have about stems which we have not yet integrated into a morphological system. */
    CSuffixCollection *             m_ParaSuffixes;
    CStemCollection *               m_ResidualStems;
    CSignatureCollection *          m_ResidualPrefixSignatures;
    CStemCollection *               m_StemsFromSubsignatures;
    CSignatureCollection*           m_Subsignatures;
    QList<simple_sig_graph_edge*>   m_SigGraphEdgeList; /*!< the sig_graph_edges in here contain only one word associated with each. */
    lxa_sig_graph_edge_map          m_SigGraphEdgeMap;  /*!< the sig_graph_edges in here contain lists of words associated with them. */
    CSignatureCollection *          m_PassiveSignatures;  /*!< these signatures have stems one letter off from another signature. */
    CSignatureCollection *          m_SequentialSignatures; /*! signatures where one affix leads to another signature. */
    QList<CHypothesis*> *            m_Hypotheses;
    QMap<QString, CHypothesis*> *            m_Hypothesis_map;
// add component 1

    QProgressBar*                   m_ProgressBar;
    QStatusBar *                      m_StatusBar;

    double                          m_entropy_threshold_for_stems;

public:
    CLexicon(CLexicon* parent_lexicon = NULL, bool suffix_flag = true);
public:

    ~CLexicon();

    void                                        dump_signatures_to_debug();
    // accessors and protostems
    void                                        dump_suffixes(QList<QString>*);
    void                                        collect_parasuffixes();
    void                                        compute_sig_graph_edges();
    void                                        generate_hypotheses();
    CSignatureCollection*                       get_active_signature_collection();
    QMap<QString, eComponentType> &             get_category_types()        { return m_category_types;}
    double                                      get_entropy_threshold_for_positive_signatures() {return m_entropy_threshold_for_stems;}
    //void                                        get_epositive_signatures(QList<CSignature*> *);
    QList<CHypothesis*>*                        get_hypotheses ()           {return m_Hypotheses;}
    QMap<QString, CHypothesis*>  *              get_hypothesis_map()        { return m_Hypothesis_map;}
    CHypothesis*                                get_hypothesis(QString hypothesis_label);
//    QList<QPair<QString,QString> > *            get_parses()                {return m_Parses;}
    QList<CParse* > *                           get_parses()                {return m_Parses;}
    CSuffixCollection*                          get_parasuffixes()          { return m_ParaSuffixes;}
    CSignatureCollection*                       get_passive_signatures()    { return m_PassiveSignatures;}
    CSignatureCollection*                       get_prefix_signatures()     { return m_PrefixSignatures;}
    CStemCollection *                           get_prefixal_stems()        { return m_prefixal_stems;}
    CPrefixCollection *                         get_prefixes()              { return m_Prefixes; }
    CSignatureCollection *                      get_residual_signatures()   { return m_ParaSignatures;}
    CSignatureCollection *                      get_sequential_signatures() { return m_SequentialSignatures;}
    CSignatureCollection*                       get_signatures()            { return m_Signatures;}
    CSignatureCollection*                       get_suffix_signatures()     { return m_Signatures;}
    CSuffixCollection*                          get_suffixes()              {return m_Suffixes;}
    CStemCollection *                           get_suffixal_stems()        { return m_suffixal_stems;}
//    QMap<QString,int>*                          get_protostems()            { return &m_suffix_protostems;}
    QList<simple_sig_graph_edge*> *             get_sig_graph_edges()       { return &m_SigGraphEdgeList;}
    lxa_sig_graph_edge_map *                    get_sig_graph_edge_map()    { return & m_SigGraphEdgeMap;}
    sig_graph_edge*                             get_sig_graph_edge(QString label) {return m_SigGraphEdgeMap[label];}
    QListIterator<simple_sig_graph_edge*>    *  get_sig_graph_edge_list_iter();
    lxa_sig_graph_edge_map_iter *               get_sig_graph_edge_map_iter();

    bool                                        get_suffix_flag()           { return m_SuffixesFlag; }
    CWordCollection*                            get_word_collection()       { return m_Words; }
    CWordCollection *                           get_words()                 { return m_Words;}

    void                                        set_progress_bar (QProgressBar * pPB) { m_ProgressBar = pPB;}
    void                                        set_status_bar(QStatusBar* pBar) {m_StatusBar = pBar;}
    void                                        set_prefixes_flag()         { m_SuffixesFlag = false;}
    void                                        set_suffixes_flag()         { m_SuffixesFlag = true;}
    void                                        set_suffix_flag(bool flag)  { m_SuffixesFlag = flag;}
    void                                        set_window(MainWindow* window) { m_main_window = window; }

    CLexicon *                                  build_sublexicon(MainWindow* = NULL);
    void                                        link_signature_and_stem(stem_t , CSignature*, QString this_signature_string );
    void                                        link_signature_and_affix(CSignature*, affix_t);

public:
    // insert functions here
    void assign_suffixes_to_stems(QString name_of_calling_function);
    void clear_lexicon();
    void compare_opposite_sets_of_signatures(QSet<CSignature*>* sig_set_1, QSet<CSignature*>* sig_set_2,QString letter);
    void compute_sig_graph_edge_map();
    void Crab_1();
    void Crab_2();
    void CreateStemAffixPairs();
    void create_temporary_map_from_stems_to_affix_sets(map_sigstring_to_morph_set   &); //map_sigstring_to_stem_list &);
    void create_sublexicon ();
    void find_full_signatures();
    void FindGoodSignaturesInsideParaSignatures();
    void FindProtostems();
    void replace_parse_pairs_from_current_signature_structure(bool FindSuffixesFlag=true);
    void ReSignaturizeWithKnownAffixes();
    void test_for_phonological_relations_between_signatures();
};

#endif // CLEXICON_H
