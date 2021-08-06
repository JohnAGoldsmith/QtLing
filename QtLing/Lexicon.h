#ifndef CLEXICON_H
#define CLEXICON_H

#include <QMap>
#include <QMapIterator>
#include <QString>
#include <QList>
#include <QPair>
#include <QSet>
#include <QStatusBar>
#include <QStringList>
#include "SignatureCollection.h"
#include "Typedefs.h"
#include "evaluation.h"
#include <QJsonValue>
#include "generaldefinitions.h"

class MainWindow;
class CWordCollection;
class CStemCollection;
class CSuffixCollection;
class CPrefixCollection;
class QProgressBar;
class CHypothesis;
class CParse;
class CompoundWordCollection;
class CompoundComponentCollection;


/* The principal objects we use on the way to morphological analysis are:
 *
 * Parse: a pair of strings, typically stem and affix;
 * Protostem: a triple, one string and two integers, showing the stem and the beginning and end point on the list of words where it appears.
 * Protosig: A pair consisting of a stem and a set of affixes
 *
 * Stem_to_sig_map: a map from a stem to a set of affixes (or to a string representation of affixes?). Was "Protosigs".
 * Sig_to_stem_map: a map from a sig-string to a set of stems. Was "Sig_to_stems".
 *
 * Also:
 * Parse_list
 */


class CJsonInfo {
public:
    bool suffix_flag;
    QList<CWord*> words;
    QList<CStem*> suffix_stems;
    QList<CStem*> prefix_stems;
    QList<CSignature*> suffix_signatures;
    QList<CSignature*> prefix_signatures;
    QList<CSuffix*> suffixes;
    QList<CPrefix*> prefixes;
    typedef QPair<QJsonValue::Type, QString> JsonTag;
    typedef QList<JsonTag> JsonTagList;
    static bool check_tags(const QJsonObject& ref_json, const QList<QPair<QJsonValue::Type, QString>>& tags);
    static QString eSortStyle_to_string(const eSortStyle sortstyle);
    static eSortStyle string_to_eSortStyle(const QString& str);
};


//  part of an experiment:
class Collection
{
    eComponentType      m_component_type;

public:
    Collection(eComponentType this_type)        {m_component_type = this_type;}
    QString             get_output();
};

//  end of experiment

// a word_sig_pair expresses information about pairs of signatures that both contain a particular word
// under two different stem-analyses.
//-----------------------------------------------------------------------//
class word_sig_pair{
//-----------------------------------------------------------------------//
public:
    CSignature*         m_sig_1;
    CSignature*         m_sig_2;
    CLexicon *          m_Lexicon;
    sigstring_t         m_sig_string_1;
    sigstring_t         m_sig_string_2;
    double              m_sig_1_entropy = -1;
    double              m_sig_2_entropy = -1;
    morph_t             morph;
    word_t              word;
    stem_t              longer_stem; // the longer stem, was stem_1
    stem_t              shorter_stem;  // was stem_2
    word_sig_pair();
    word_sig_pair(CLexicon* lexicon, CSignature* pSig1, CSignature* pSig2, sigstring_t sig_string_1, sigstring_t sig_string_2,morph_t m,word_t w, stem_t stem1, stem_t stem2)
    {

        m_sig_1 = pSig1;
        m_sig_2 = pSig2;
        m_Lexicon = lexicon;
        m_sig_string_1 = sig_string_1;
        m_sig_string_2 = sig_string_2;
        morph = m;
        word = w;
        longer_stem = stem1;
        shorter_stem = stem2;
        if (longer_stem.length() < shorter_stem.length()){
            qDebug() << "constructor of word_sig_pair, the stems are in reversed order.";
        }
    };
    QString label() {return morph + "/" + m_sig_string_1 + "/" + m_sig_string_2; }
};
//-----------------------------------------------------------------------//


//-----------------------------------------------------------------------//
class sig_pair{   // was: sig_graph_edge{
//-----------------------------------------------------------------------//
public:
    CSignature* m_sig_1;
    CSignature* m_sig_2;
    CLexicon*   m_lexicon;
    sigstring_t  m_sig_string_1;
    sigstring_t  m_sig_string_2;
    double m_sig_1_entropy = -1;
    double m_sig_2_entropy = -1;
    morph_t     morph;
    QMap<QString, word_stem_struct*>         shared_word_stems;
    sig_pair();

    sig_pair(word_sig_pair this_word_sig_pair){
             m_lexicon = this_word_sig_pair.m_Lexicon;
             m_sig_1 = this_word_sig_pair.m_sig_1;
             m_sig_2 = this_word_sig_pair.m_sig_2;
             m_sig_string_1  = this_word_sig_pair.m_sig_string_1;
             m_sig_string_2  = this_word_sig_pair.m_sig_string_2;
             m_sig_1_entropy = this_word_sig_pair.m_sig_1_entropy;
             m_sig_2_entropy = this_word_sig_pair.m_sig_2_entropy;
             morph = this_word_sig_pair.morph;
             /*
             word_stem_struct * this_word_stems = new word_stem_struct;
             this_word_stems->word = this_word_sig_pair.word;
             this_word_stems->stem_1 = this_word_sig_pair.stem_1;
             this_word_stems->stem_2 = this_word_sig_pair.stem_2;
             */
             word_stem_struct * this_word_stems = new word_stem_struct (
                         this_word_sig_pair.word,
                         this_word_sig_pair.longer_stem,
                         this_word_sig_pair.shorter_stem);
             shared_word_stems[this_word_stems->get_label()] = this_word_stems;
         }
    QString     label() {return morph + "/" + m_sig_string_1 + "/" + m_sig_string_2; }
    int         get_number_of_words() {return shared_word_stems.size();}
    CSignature* get_sig_1() {return m_sig_1;}
    CSignature* get_sig_2() {return m_sig_2;}
    sigstring_t get_sig1_string() { return m_sig_string_1;}
    sigstring_t get_sig2_string() { return m_sig_string_2;}
    morph_t     get_morph() {return morph;}
    QString     display();
};

struct DoomedSignatureInfo {
    sig_pair* m_edge_ptr;

    QList<affix_t> m_doomed_affixes;
    sigstring_t m_str_revised_sig;


    DoomedSignatureInfo() {
        m_edge_ptr = NULL;
        m_doomed_affixes = QStringList();
        m_str_revised_sig = QString();
    }

    DoomedSignatureInfo(sig_pair* p_edge, QStringList& affixes) {
        m_edge_ptr = p_edge;
        m_doomed_affixes = affixes;
        m_str_revised_sig = QString();
    }
};

//-----------------------------------------------------------------------//
class protostem{
//-----------------------------------------------------------------------//
    /*!
         * \brief m_protostem
         */
        QString     m_protostem;
        int         m_start_word;
        int         m_end_word;
        bool        m_suffix_flag;
public:
        protostem(QString stem, int start_word, int end_word=-1, bool m_suffix_flag = true);
        protostem(QString stem, bool m_suffix_flag = true);
        ~protostem();

        QString     get_stem() { return m_protostem;}
        int         get_start_word() { return m_start_word;}
        int         get_end_word()   {return m_end_word;}
        void        set_start_and_end_word(int a, int b) {m_start_word = a; m_end_word = b;}
};

//-----------------------------------------------------------------------//
class CLexicon
//-----------------------------------------------------------------------//

{
protected:
    const int                       M_MINIMUM_STEM_LENGTH;
    const int                       M_MINIMUM_STEM_COUNT;
    const int                       M_MAXIMUM_AFFIX_LENGTH;
    const int                       M_MINIMUM_HYPOTHESIS_WORD_COUNT;
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
    CompoundWordCollection *        m_Compounds;
    CompoundComponentCollection *   m_CompoundComponents;
    QMap<QString, CParse*>          m_Raw_parses;
    //QList<CParse*> *                m_Parses; // get rid of this list, just use the QMap
    QMap<QString, CParse*>          m_ParseMap;

    QMap<QString, protostem*>       m_suffix_protostems;
    QMap<QString, protostem*>       m_prefix_protostems;

    bool                            m_SuffixesFlag;
    CLexicon*                       m_parent_lexicon;

    // all of the possible continuations
    // affixes that are "thrown out" in Crab2
    CSignatureCollection*           m_ParaSignatures;   /*!<  the information we have about stems which we have not yet integrated into a morphological system. */
    //CSuffixCollection *             m_ParaSuffixes;
    QMap<QString, QStringList*>*      m_ParaSuffixes;
    CStemCollection *               m_ResidualStems;
    CSignatureCollection *          m_ResidualPrefixSignatures;
    CStemCollection *               m_StemsFromSubsignatures;
    CSignatureCollection*           m_Subsignatures;

    // Finds the difference between signatures, e.g. {ed, es, er, e, ing} vs {d, s, r, NULL}
    QList<word_sig_pair*>                m_WordSigPairList; //m_SigGraphEdgeList; /*!< the sig_graph_edges in here contain only one word associated with each. */
    //lxa_sig_graph_edge_map          m_SigGraphEdgeMap;  /*!< the sig_graph_edges in here contain lists of words associated with them. */
   // sig_pair_map                    m_SigGraphEdgeMap;  /*!< the sig_graph_edges in here contain lists of words associated with them. */
    sig_pair_map                    m_SigPairMap;  /*!< the sig_pair_edges in here contain lists of words associated with them. */
    CSignatureCollection *          m_PassiveSignatures;  /*!< these signatures have stems one letter off from another signature. */
    CSignatureCollection *          m_SequentialSignatures; /*! signatures where one affix leads to another signature. */
    // Generalizes repeating
    QList<CHypothesis*> *           m_Hypotheses;
    QMap<QString, CHypothesis*> *   m_Hypothesis_map;
// add component 1

    Sig_to_stem_map                 m_intermediate_sig_to_stem_map; // used during computation, not permanent.
    Stem_to_sig_map                 m_intermediate_stem_to_sig_map; // used during computation, not permanent.

    QProgressBar*                   m_ProgressBar;
    QStatusBar *                    m_StatusBar;

    QMap<QString, QStringList*>     m_stem_autobiographies;
    QMap<QString, QStringList*>     m_word_autobiographies;

    double                          m_entropy_threshold_for_stems;

    // experiment for gold standard evaluation code
    GoldStandard*                   m_goldstandard;
    EvalParses*                     m_eval_parses;
    // end of experiment

public:
    CLexicon(CLexicon* parent_lexicon = NULL, bool suffix_flag = true);
public:

    ~CLexicon();

    // experiment for gold standard evaluation code
    GoldStandard*                               get_goldstandard()          { return m_goldstandard; }
    GoldStandard*                               new_goldstandard_from_xml(QString& file_name);
    void                                        delete_goldstandard()       { delete m_goldstandard; m_goldstandard = NULL; }
    bool                                        do_gs_evaluation();

    EvalParses*                                 get_eval_parses()      { return m_eval_parses; }
    EvalParses*                                 new_eval_parses_from_txt(QString& file_name);
    void                                        delete_eval_parses();
    bool                                        do_gs_evaluation_on_eval_parses();


    void                                        dump_signatures_to_debug();
    // accessors and protostems
    void                                        dump_suffixes(QList<QString>*);
    void                                        add_parse(CParse*);
    void                                        add_parasuffix(QString parasuffix, QString word);
    CSignatureCollection*                       get_active_signature_collection();
    QMap<QString, eComponentType> &             get_category_types()        { return m_category_types;}
    CompoundWordCollection*                     get_compounds()             { return m_Compounds; }
    double                                      get_entropy_threshold_for_positive_signatures() {return m_entropy_threshold_for_stems;}
    //void                                        get_epositive_signatures(QList<CSignature*> *);
    QList<CHypothesis*>*                        get_hypotheses ()           {return m_Hypotheses;}
    QMap<QString, CHypothesis*>  *              get_hypothesis_map()        { return m_Hypothesis_map;}
    CHypothesis*                                get_hypothesis(QString hypothesis_label);
    QMap<QString, CParse* > *                   get_parses()                {return &m_ParseMap;}
    //CSuffixCollection*                          get_parasuffixes()          { return m_ParaSuffixes;}
    QMap<QString,QStringList*> *                 get_parasuffixes()          {return m_ParaSuffixes;}
    CSignatureCollection*                       get_passive_signatures()    { return m_PassiveSignatures;}
    CSignatureCollection*                       get_prefix_signatures()     { return m_PrefixSignatures;}
    CStemCollection *                           get_prefixal_stems()        { return m_prefixal_stems;}
    CPrefixCollection *                         get_prefixes()              { return m_Prefixes; }
    QMap<QString, protostem*>*                  get_prefixal_protostems()    { return &m_prefix_protostems; }
    CSignatureCollection *                      get_residual_signatures()   { return m_ParaSignatures;}
    CSignatureCollection *                      get_sequential_signatures() { return m_SequentialSignatures;}
    CSignatureCollection*                       get_signatures()            { return m_Signatures;}
    CSignatureCollection*                       get_suffix_signatures()     { return m_Signatures;}
    CSuffixCollection*                          get_suffixes()              {return m_Suffixes;}
    CStemCollection *                           get_suffixal_stems()        { return m_suffixal_stems;}
    QMap<QString, protostem*>*                  get_suffixal_protostems()  { return &m_suffix_protostems; }
    lxa_sig_graph_edge_map *                    get_sig_graph_edge_map()    { return & m_SigPairMap;}
    sig_pair*                                   get_sig_graph_edge(QString label) {return m_SigPairMap[label];}
    QListIterator<word_sig_pair*>    *          get_sig_graph_edge_list_iter();
    sig_pair_iter *                             get_sig_graph_edge_map_iter();

    bool                                        get_suffix_flag()           { return m_SuffixesFlag; }
    CWordCollection*                            get_word_collection()       { return m_Words; }
    CWordCollection *                           get_words()                 { return m_Words;}

    QList<QString> *                            get_stem_autobiography(const stem_t& stem)   { return m_stem_autobiographies[stem];}
    void                                        add_to_stem_autobiographies (const QString& stem, const QString& message);
    bool                                        stem_autobiographies_contains(const QString& stem);
    QList<QString> *                            get_word_autobiography(const word_t& word) { return m_word_autobiographies[word];}
    void                                        add_to_word_autobiographies (const QString& word, const QString& message);



    bool                                        word_autobiographies_contains(const QString& word);
    void                                        word_autobiography_positive_notice( QString word, QString stem, QString sig_string, QString calling_function);
    void                                        word_autobiography_positive_notice_2( QString word, QString stem, QString sig_string, QString calling_function);
    void                                        set_progress_bar (QProgressBar * pPB) { m_ProgressBar = pPB;}
    void                                        set_status_bar(QStatusBar* pBar) {m_StatusBar = pBar;}
    void                                        set_prefixes_flag()         { m_SuffixesFlag = false;}
    void                                        set_suffixes_flag()         { m_SuffixesFlag = true;}
    void                                        set_suffix_flag(bool flag)  { m_SuffixesFlag = flag;}
    void                                        set_window(MainWindow* window) { m_main_window = window; }

    CLexicon *                                  build_sublexicon(MainWindow* = NULL);


    void                                        time_stamp(const QString& message);
    bool                                        verify_parses(); //check parses against the parses in each word;
    bool                                        remove_parse(QString full_display_of_parse);
    bool                                        remove_parse(CParse*);


public:
    // insert functions here
    void step1_from_words_to_protostems();
    void step2_from_protostems_to_parses();

    void step3_from_parses_to_stem_to_sig_maps(QString name_of_calling_function);
    void step3a_from_parses_to_stem_to_sig_map(bool suffix_flag);
    void step3b_from_stem_to_sig_map_to_sig_to_stem_map();

    //void step4_assign_affixes_to_stems(QString name_of_calling_function);
    //void step4_create_signatures(QString name_of_calling_function);
    void step4_create_signatures(const QString& name_of_calling_function,
                                 eMinimumStemCountFlag min_stem_count_flag = MS_respect_mininmum_stem_count);
    void step4a_link_signature_and_affix(CSignature*, affix_t);
    void step4b_link_signature_and_stem_and_word(stem_t , CSignature*, const QString& name_of_calling_function);

    void replace_parse_pairs_from_current_signature_structure();
    void repair_low_entropy_signatures();
     void collect_parasuffixes();

    void ReSignaturizeWithKnownAffixes();
    void step6a_create_temporary_map_from_stems_to_affix_sets(Stem_to_sig_map&); //map_sigstring_to_stem_list &); will delete this
    //void step6a_create_temporary_stem_to_sig_map(); // will replace preceding version;
    void create_new_parse_set_from_known_affixes();

    void step7_FindGoodSignaturesInsideParaSignatures();
    void step6c_from_stem_to_sig_maps_to_xxx(QString, Stem_to_sig_map ) {return;}

    void step8a_compute_word_sig_pairs();
    void step8b_compute_sig_pair_map();
    void step8c_from_sig_pairs_to_parses();

    typedef QMap<sigstring_t, DoomedSignatureInfo> DoomedSignatureInfoMap;
    void step9_from_sig_pair_map_to_hypotheses();
    void step9a_from_doomed_info_map_to_parses(DoomedSignatureInfoMap& ref_doomed_info_map);
    void step9b_redirect_ptrs_in_sig_graph_edges_map(const DoomedSignatureInfoMap& ref_doomed_info_map);
    void step9c_from_doomed_info_map_to_hypotheses(const DoomedSignatureInfoMap& ref_doomed_info_map);

    void step10_find_compounds();

    void clear_lexicon();
    void compare_opposite_sets_of_signatures(QSet<CSignature*>* sig_set_1, QSet<CSignature*>* sig_set_2,QString letter);

    void Crab_1();
    void Crab_2();
    void Crab_3();
    void Crab_4();
    void Crab_5();
    void Crab_6();
    void create_sublexicon();

    void check_autobiography_consistency();

    void test_json_functionality();
    void assign_json_id();
    void write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED);



     void test_for_phonological_relations_between_signatures();

    void clear_parses();
};

#endif // CLEXICON_H
