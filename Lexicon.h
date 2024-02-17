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
class sig_pair;
class word_sig_pair;
class SigPairCollection;

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
    int                             M_MINIMUM_STEM_COUNT;
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
    CSignatureCollection *          m_VirtualSignatures; // a temporary group of "virtual signatures"; a signature is virtual if it is a proper subsignature of an empirical signature.
    CompoundWordCollection *        m_Compounds;
    CompoundComponentCollection *   m_CompoundComponents;
    QMap<QString, CParse*>          m_Raw_parses;
    //QList<CParse*> *                m_Parses; // get rid of this list, just use the QMap
    QMap<QString, CParse*>          m_ParseMap;

    //QMap<QString, protostem*>       m_suffix_protostems;
    //QMap<QString, protostem*>       m_prefix_protostems;
    QMultiMap<QString, QString>     m_word2suffix_sigs;
    QMultiMap<QString, QString>     m_word2prefix_sigs;
    CStemCollection               * m_suffix_protostems;
    CStemCollection               * m_prefix_protostems;

    bool                            m_suffix_flag;
    CLexicon*                       m_parent_lexicon;
    QMap<QString, int>              m_internal_suffix_counts;
    QMap<QString, int>              m_internal_prefix_counts; // we assign numbers to the names of word-internal affixes, like ":5" to the end of a suffix, and "5:" to the beginning of a prefix.

    // all of the possible continuations
    // affixes that are "thrown out" in Crab2
    CSignatureCollection*           m_ParaSignatures;   /*!<  the information we have about stems which we have not yet integrated into a morphological system. */
    CSuffixCollection *             m_ParaSuffixes;
    //QMap<QString, QStringList*>*    m_ParaSuffixes;
    QMap<QString, QStringList*>*    m_ParaPrefixes;
    CStemCollection *               m_ResidualStems;
    CSignatureCollection *          m_ResidualPrefixSignatures;
    CStemCollection *               m_StemsFromSubsignatures;
    CSignatureCollection*           m_Subsignatures;

    // Finds the difference between signatures, e.g. {ed, es, er, e, ing} vs {d, s, r, NULL}
    SigPairCollection *             m_suffixal_sig_pairs;

    CSignatureCollection *          m_PassiveSignatures;  /*!< these signatures have stems one letter off from another signature. */
    CSignatureCollection *          m_SequentialSignatures; /*! signatures where one affix leads to another signature. */
    // Generalizes repeating
    QList<CHypothesis*> *           m_Hypotheses;
    QMap<QString, CHypothesis*> *   m_Hypothesis_map;
// add component 1

    Sig_to_stem_map                 m_temp_sig2stem_map; // used during computation, not permanent.
    Stem_to_sig_map                 m_temp_stem2sig_map; // used during computation, not permanent.

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
    CLexicon(MainWindow *, CLexicon* parent_lexicon = NULL, bool suffix_flag = true);
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
    //void                                        dump_suffixes(QList<QString>*);
    void                                        add_parse(CParse*);
    void                                        add_paraprefix(QString paraprefix, QString word);
    void                                        add_parasuffix(QString parasuffix, QString word);
    CSignature*                                 add_signature(QString);
    void                                        add_stem(QString);
    CWord*                                      add_word(QString);
    void                                        add_affix_to_signature(QString affix, CSignature*  pSig);

    void                                        add_suffix_analysis_to_word(CWord* pWord,CParse& parse, QString sig_string,
                                                                            QString name_of_calling_function);
    void                                        add_prefix_analysis_to_word(CWord* pWord, CParse& parse, QString sig_string,
                                                                            QString name_of_calling_function);
    void                                        change_minimum_stem_count(int count) {M_MINIMUM_STEM_COUNT = count;}
    //double                                      compute_MDL_1(); // for MDL project with Marina E.
    double                                      compute_MDL(); // for MDL project with Marina E.
    CSignature *                                find_signature_of_stem(QString stem, bool suffix_flag);
    CSignatureCollection*                       get_active_signature_collection();
    CStemCollection*                            get_active_stem_collection();

    int                                         get_affix_count(QString affix);
    QStringList                                 get_affix_continuation(QString affix, bool suffix_flag ,QStringList continuations );
    QMap<QString, eComponentType> &             get_category_types()        { return m_category_types;}
    CompoundWordCollection*                     get_compounds()             { return m_Compounds; }

    void                                        generate_virtual_signatures();

    double                                      get_entropy_threshold_for_positive_signatures() {return m_entropy_threshold_for_stems;}
    //void                                        get_epositive_signatures(QList<CSignature*> *);
    QList<CHypothesis*>*                        get_hypotheses ()           {return m_Hypotheses;}
    QMap<QString, CHypothesis*>  *              get_hypothesis_map()        { return m_Hypothesis_map;}
    CHypothesis*                                get_hypothesis(QString hypothesis_label);
    int                                         get_internal_affix_count(QString );
    QMap<QString, CParse* > *                   get_parses()                {return &m_ParseMap;}
    CSuffixCollection*                          get_parasuffixes()          { return m_ParaSuffixes;}
    //QMap<QString,QStringList*> *                 get_parasuffixes()          {return m_ParaSuffixes;}
    CSignatureCollection*                       get_passive_signatures()    { return m_PassiveSignatures;}
    CSignatureCollection*                       get_prefix_signatures()     { return m_PrefixSignatures;}
    CStemCollection *                           get_prefixal_stems()        { return m_prefixal_stems;}
    CPrefixCollection *                         get_prefixes()              { return m_Prefixes; }
    CStemCollection *                           get_prefixal_protostems()    { return m_prefix_protostems; }
    CSignatureCollection *                      get_residual_signatures()   { return m_ParaSignatures;}
    CSignatureCollection *                      get_sequential_signatures() { return m_SequentialSignatures;}
    CSignatureCollection*                       get_signatures()            { return m_Signatures;}
    //QList<Spine*>  *                             get_suffixal_spines()                {return m_suffixal_spines;}
    CSignatureCollection*                       get_suffixal_signatures() {return m_Signatures;}
    SigPairCollection *                         get_suffixal_sig_pairs()             {return m_suffixal_sig_pairs;}
    CSignatureCollection*                       get_suffix_signatures()     { return m_Signatures;}
    CSuffixCollection*                          get_suffixes()              {return m_Suffixes;}
    CStemCollection *                           get_suffixal_stems()        { return m_suffixal_stems;}
    CStemCollection *                           get_suffixal_protostems()  { return m_suffix_protostems; }
    //lxa_sig_graph_edge_map *                    get_sig_graph_edge_map()    { return & m_SigPairs;}
    //sig_pair*                                   get_sig_graph_edge(QString label) {return m_SigPairMap[label];}
    QListIterator<word_sig_pair*>    *          get_sig_graph_edge_list_iter();
    sig_pair_iter *                             get_sig_graph_edge_map_iter();
    //sig_pair_map *                              get_suffixal_sig_pair_map() {return & m_SigPairMap;}

    bool                                        get_suffix_flag()           { return m_suffix_flag; }
    CSignatureCollection*                       get_virtual_signatures()    {return m_VirtualSignatures;}
    CWordCollection*                            get_word_collection()       { return m_Words; }
    CWordCollection *                           get_words()                 { return m_Words;}

    void                                        input_words(QMap<QString, int> word_counts);

    //helper functions:
    // Crab1:
    //void                                        make_protostem_if_appropriate_suffix_case(QString this_word, QString next_word);
    //void                                        make_protostem_if_appropriate_prefix_case(QString this_word, QString next_word);
    void                                        mark_progress(int); // mark progress bar and process events too.

    QString                                     process_a_word_into_stem_and_affix(int wordno, int stemlength, bool suffix_flag);
    void                                        initialize_progress(int max_value);

    void                                        attach_stem_to_intermediate_signature(QString stem);


    void                                        keep_only_one_parse_per_word();


    QList<QString> *                            get_stem_autobiography(const stem_t& stem)   { return m_stem_autobiographies[stem];}
    void                                        add_to_stem_autobiographies (const QString& stem, const QString& message);
    bool                                        stem_autobiographies_contains(const QString& stem);
    QList<QString> *                            get_word_autobiography(const word_t& word) { return m_word_autobiographies[word];}
    void                                        add_to_word_autobiographies (const QString& word, const QString& message);
    void                                        add_to_word_biographies (int analysis_number,
                                                    QString shorter_stem,
                                                    QString shorter_stem_sig_string,
                                                    QString difference,
                                                    CWord * pWord,
                                                    QString longer_stem,
                                                    QString longer_stem_sig_string);
    bool                                        test_if_analysis_has_been_done();

    bool                                        word_autobiographies_contains(const QString& word);
    void                                        word_autobiography_positive_notice( QString word, QString stem, QString sig_string, QString calling_function);
    //void                                        word_autobiography_positive_notice_2( QString word, QString stem, QString sig_string, QString calling_function);
    void                                        set_progress_bar (QProgressBar * pPB) { m_ProgressBar = pPB;}
    void                                        set_status_bar(QStatusBar* pBar) {m_StatusBar = pBar;}
    void                                        set_prefixes_flag()         { m_suffix_flag = false;}
    void                                        set_suffixes_flag()         { m_suffix_flag = true;}
    void                                        set_suffix_flag(bool flag)  { m_suffix_flag = flag;}
    void                                        set_window(MainWindow* window) { m_main_window = window; }

    void                                        sort_parse_triples_on_all_words(bool suffixes_flag);

    CLexicon *                                  build_sublexicon(MainWindow* = NULL);


    void                                        time_stamp(const QString& message);
    bool                                        remove_parse(QString full_display_of_parse);
    bool                                        remove_parse(CParse*);
    bool                                        remove_parse(CParse&);
//MDL:
    /*void                                        MDL_process_each_signature(CSignature*, QString,
                                                                           QMap<QString, QMap<QString, int>* >&,
                                                                           QMap<QString,int>, // word counts
                                                                           double,            // type counts of analyzed words
                                                                           double,          // corpus count of analyzed words
                                                                           QMap<QString,int>&, // stems
                                                                           QMap<QString, int>&, // affixes
                                                                           QMap<QString, QString>,
                                                                           QMap<QString, QString>,
                                                                           QMap<QString, QString>);


*/
public:
    // insert functions here
    void step1_from_words_to_protostems();
    void step1a_make_protostem_if_appropriate(QString this_word, QString next_word);
    void step1b_make_protostem_objects_suffix_case(const QStringList* Words);
    void step1b_make_protostem_objects_prefix_case(const QStringList* Words);

    void step2_from_protostems_to_parses();
    void step2a_scan_word_for_protostems_suffix_case(QString word);
    void step2a_scan_word_for_protostems_prefix_case(QString word);

    void step3_from_parses_to_stem2sig_maps(QString name_of_calling_function);
    void step3a_clear_lexicon_except_protostems();
    void step3c_attach_affix_to_stem(CParse*);
    void step3b_from_parses_to_stem2sig_map( );
    void step3d_from_stem2sig_map_to_sig2stem_map();

    void step4_create_signatures(const QString& name_of_calling_function,
                                 eMinimumStemCountFlag min_stem_count_flag = MS_respect_mininmum_stem_count);
    void step4a_clear_active_signatures_and_affixes();
    void step4d_link_signature_with_affix(CSignature*, stem_t, affix_t, QString name_of_calling_function);
    void step4c_create_stem_then_link_sig_and_stem(CSignature * pSig,  QString stem, QString name_of_calling_function);
    void step4b_link_sig_with_stems_and_affixes(CSignature*, QSet<stem_t>* stem_set,  QString name_of_calling_function);
    void step4e_link_all_words_to_signatures(QString name_of_calling_function);
    void step4f_add_analysis_to_word_for_suffix_case(CParse&,  CSignature* pSig,
                                                     QString name_of_calling_function);
    void step4f_add_analysis_to_word_for_prefix_case(CParse&,  CSignature* pSig,
                                                     QString name_of_calling_function);
    //void link_signatures_with_words(QString& name_of_calling_function);

    void replace_parse_pairs_from_current_signature_structure();
    void repair_low_entropy_signatures();
     void collect_parasuffixes();

    void ReSignaturizeWithKnownAffixes();
    void step6a_create_temporary_map_from_stems_to_affix_sets(Stem_to_sig_map&); //map_sigstring_to_stem_list &); will delete this
    void create_new_parse_set_from_known_affixes();
    void find_good_signatures_inside_bad();
    void find_all_suffixal_sigpairs();
    void find_new_affixes(protostem*, CSignatureCollection*, CStemCollection*, QStringList& );
    void find_parasuffixes();
    void find_parasuffixes(int wordno, int stem_length, QStringList working_affix_string_list);
    void find_paraprefixes(int wordno, int stem_length, QStringList working_affix_string_list);

    void print_signature_transforms();

    void step6c_from_stem_to_sig_maps_to_xxx(QString, Stem_to_sig_map ) {return;}

    void step8a_compute_word_sig_pairs();
    void step8c_from_sig_pairs_to_parses_Create_hypotheses();

    typedef QMap<sigstring_t, DoomedSignatureInfo> DoomedSignatureInfoMap;
    void step9_from_sig_pair_map_to_hypotheses();
    void step9a_from_doomed_info_map_to_parses(DoomedSignatureInfoMap& ref_doomed_info_map);
    void step9b_redirect_ptrs_in_sig_graph_edges_map(const DoomedSignatureInfoMap& ref_doomed_info_map);
    void step9c_from_doomed_info_map_to_hypotheses(const DoomedSignatureInfoMap& ref_doomed_info_map);
    void step10_find_compounds();
    void find_hyphenated_compounds();
    void find_stems_made_of_stems();
    void find_words_made_of_words();
    void step11_find_allomorphic_signatures();


    void clear_parses();
    void compare_opposite_sets_of_signatures(QSet<CSignature*>* sig_set_1, QSet<CSignature*>* sig_set_2,QString letter);

    void Crab_1();
    void Crab_2();
    void Crab_3();
    void Crab_4();
    void Crab_5();
    void Crab_6();
    void Crab_compounds();
    void create_sublexicon();

    void check_autobiography_consistency();

    void test_json_functionality();
    void assign_json_id();
    void write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED);



     void test_for_phonological_relations_between_signatures();

};

#endif // CLEXICON_H
