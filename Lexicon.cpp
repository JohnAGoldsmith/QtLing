#include <QPair>
#include <QList>
#include <QMap>
#include <QMapIterator>
#include <QMultiMap>
#include <QSet>
#include <QtDebug>
#include <QProgressBar>
#include <QApplication>
//#include <algorithm>
#include <QChar>
#include <QSet>
#include <QRegularExpression>

#include "Lexicon.h"
#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "Word.h"
#include "evaluation.h"
#include "cparse.h"
#include "compound.h"
#include "sigpaircollection.h"

void SortQStringListFromRight(QStringList& ThisStringList);
//bool reverse_string_compare(QString,QString);

void CLexicon::mark_progress(int n){
    m_ProgressBar->setValue(n);
    qApp->processEvents();
}
void CLexicon::initialize_progress(int max_value){
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(max_value);
}

protostem::protostem(QString stem, int start_word, int end_word, bool suffix_flag)
{   m_protostem = stem;
    m_start_word = start_word;
    m_end_word = end_word;
    m_suffix_flag = suffix_flag;
}
protostem::protostem(QString stem, bool suffix_flag)
{
    m_protostem = stem;
    m_start_word = -1;
    m_end_word = -1;
    m_suffix_flag = suffix_flag;
}
protostem::~protostem()
{
}
CLexicon::CLexicon(MainWindow * main_window, CLexicon* lexicon, bool suffix_flag):
    M_MINIMUM_STEM_LENGTH(2),
    M_MINIMUM_STEM_COUNT(2),
    M_MAXIMUM_AFFIX_LENGTH(10),
    M_MINIMUM_HYPOTHESIS_WORD_COUNT(2) // Moved to here, originally a fxn variable defined in step 9
{
    m_main_window           = main_window;
    m_Signatures            = new CSignatureCollection(this, true);
    m_PrefixSignatures      = new CSignatureCollection(this,false);
    m_VirtualSignatures     = new CSignatureCollection(this, true); // these are really local variables, only in the Lexicon for my convenience.
    m_Words                 = new CWordCollection(this);
    m_prefixal_stems        = new CStemCollection(this);
    m_suffixal_stems        = new CStemCollection(this);
    m_suffix_protostems   = new CStemCollection(this);
    m_prefix_protostems   = new CStemCollection(this);
    m_Suffixes              = new CSuffixCollection(this);
    m_Prefixes              = new CPrefixCollection(this);
    m_ParaSignatures        = new CSignatureCollection(this, true);
    m_ParaSuffixes          = new CSuffixCollection(this);
    m_ParaPrefixes          = new QMap<QString, QStringList*>;
    //m_ParaSuffixes          = new QMap<QString, QStringList*>;
    m_ResidualStems         = new CStemCollection(this);
    m_PassiveSignatures     = new CSignatureCollection(this, true);
    m_suffix_flag          = suffix_flag;
    m_suffixal_sig_pairs   = new SigPairCollection();
    m_Hypotheses            = new QList<CHypothesis*>;
    m_Hypothesis_map        = new QMap<QString, CHypothesis*>;
    m_entropy_threshold_for_stems = 0.6;
    m_parent_lexicon        = lexicon;
    m_goldstandard          = NULL;
    m_eval_parses           = NULL;

    // Put this back in, Hanson 8.1
    m_Compounds             = new CompoundWordCollection(this);

    // this tells us what *kind* of material is kept in various components (words, stems, affixes, signatures, etc.)
    m_category_types["Words"]               = CT_word;
    m_category_types["Suffixal stems"]      = CT_stem;
    m_category_types["Prefixal stems"]      = CT_stem;
    m_category_types["Suffixes"]            = CT_affix;
    m_category_types["Prefixes"]            = CT_affix;
    m_category_types["Suffix signatures"]   = CT_signature;
    m_category_types["Prefix signatures"]   = CT_signature;
    m_category_types["Compounds"]           = CT_compound;
    m_category_types["Parses"]              = CT_parse;
    m_category_types["Protostems"]          = CT_stem;
    m_category_types["Parasignatures"]      = CT_signature;
    m_category_types["Parasuffixes"]        = CT_affix;
    m_category_types["Hypotheses"]          = CT_hypothesis;
    m_category_types["Hypotheses 2"]        = CT_hypothesis2;

}
void  CLexicon::input_words(QMap<QString, int> word_counts){
    m_Words->input_words(word_counts);
}
bool CLexicon::test_if_analysis_has_been_done(){
    if (m_suffix_flag){
        if ( get_suffixal_stems()->get_count() == 0
            ||  get_suffix_signatures()->get_count() == 0 ) {
            return false;
        }
    } else {
        if (get_prefixal_stems()->get_count()  == 0
            || get_prefix_signatures()->get_count() == 0){
            return false;
        }
    }
    return true;
}
/*
QListIterator<word_sig_pair*> * CLexicon::get_sig_graph_edge_list_iter()
{    QListIterator<word_sig_pair*> * iter = new QListIterator<word_sig_pair*>(m_WordSigPairList);
    return iter;
}

QMapIterator<QString, sig_pair*> * CLexicon::get_sig_graph_edge_map_iter()
{    QMapIterator<QString, sig_pair*> * iter = new QMapIterator<QString, sig_pair*>(m_SigPairMap);
    return iter;
}
*/
CLexicon::~CLexicon()
{
    delete m_Signatures;
    delete m_PrefixSignatures;
    delete m_ParaSignatures;
    delete m_PassiveSignatures;
    delete m_goldstandard;
    //delete m_Parses;
}
CWord* CLexicon::add_word(QString word){
    return  m_Words->add_word(word);
}

CSignature* CLexicon::add_signature(QString sig_string){
    return *get_active_signature_collection() << sig_string;
}
void  CLexicon::add_stem(QString stem){
    * get_active_stem_collection() << stem;
}
CSignatureCollection* CLexicon::get_active_signature_collection(){
    if (m_suffix_flag){
        return m_Signatures;
    }else{
        return m_PrefixSignatures;
    }
}
CStemCollection* CLexicon::get_active_stem_collection(){
    if (m_suffix_flag){
        return m_suffixal_stems;
    }else{
        return m_prefixal_stems;
    }
}

void CLexicon::add_parse(CParse * parse){
    if (m_ParseMap.contains(parse->display_full())){
        return;
    }
    m_ParseMap[parse->display_full()] = parse;
}
void CLexicon::clear_parses()
{    CParse* p_parse;
    foreach (p_parse, m_ParseMap)
        delete p_parse;
    m_ParseMap.clear();
}
void CLexicon::step4a_clear_active_signatures_and_affixes(){
    if (m_suffix_flag){
        m_Suffixes->clear();
        m_Signatures->clear();
        m_suffixal_stems->clear();
    }
    else {
        m_Prefixes->clear();
        m_PrefixSignatures->clear();
        m_prefixal_stems->clear();
    }
}
bool CLexicon::remove_parse(QString full_display_of_parse){
    if (m_ParseMap.contains(full_display_of_parse)){
        m_ParseMap.remove(full_display_of_parse);
        return  true;
    } else{
       qDebug() <<  "       Failed to remove parse: " << full_display_of_parse << 149;
       return false;
    }
}
bool CLexicon::remove_parse(CParse* parse){
    if (m_ParseMap.contains(parse->display_full())){
        m_ParseMap.remove(parse->display_full());
        return  true;
    } else{
       return false;
    }
}
// this isn't used: I avoid references, prefer pointers.
bool CLexicon::remove_parse(CParse& parse){
    if (m_ParseMap.contains(parse.display_full())){
        m_ParseMap.remove(parse.display_full());
        return  true;
    } else{
       return false;
    }
}


void CLexicon::step3a_clear_lexicon_except_protostems(){
    if (m_suffix_flag){
        m_Signatures->clear();
        m_suffixal_stems->clear();
        m_Suffixes->clear();
        m_ParaSignatures->clear();
    }else{
        m_PrefixSignatures->clear();
        m_Prefixes->clear();
        m_Prefixes->clear();
        m_ParaPrefixes->clear();
    }
    m_ResidualStems->clear();
    m_PassiveSignatures->clear();
}

// for gold standard
// Return true if evaluation succeeded
// Return false if it did not
GoldStandard* CLexicon::new_goldstandard_from_xml(QString& file_name)
{
    m_goldstandard = new GoldStandard(file_name);
    return m_goldstandard;
}

bool CLexicon::do_gs_evaluation()
{
    if (m_goldstandard == nullptr) {
        qDebug() << 134 << "Lexicon.cpp: evaluation failed: GoldStandard not loaded";
        return false;
    }
    bool evaluation_succeeded = m_goldstandard->evaluate(m_Words);
    if (evaluation_succeeded) {
        qDebug() << 139 << "Lexicon.cpp: evaluation completed";
        return true;
    } else return false;
}
int CLexicon::get_internal_affix_count(QString affix){
    if (affix.endsWith(":")){
        affix.truncate(affix.length()-1);
        if (! m_internal_suffix_counts.contains(affix)){
            m_internal_suffix_counts[affix] = 1;
            return 1;
        }else{
            m_internal_suffix_counts[affix] += 1;
            return m_internal_suffix_counts[affix];
        }
    } else {
        if (affix.startsWith(":")){
            affix = affix.mid(1);
            if (! m_internal_prefix_counts.contains(affix)){
                m_internal_prefix_counts[affix] = 1;
                return 1;
            } else{
                m_internal_prefix_counts[affix] += 1;
                return m_internal_prefix_counts[affix];
            }
        }else{
            return 0;
        }
    }
}

EvalParses* CLexicon::new_eval_parses_from_txt(QString& file_name)
{
    m_eval_parses = new EvalParses(file_name);
    return m_eval_parses;
}

void CLexicon::delete_eval_parses()
{
    delete m_eval_parses;
    m_eval_parses = NULL;
}

bool CLexicon::do_gs_evaluation_on_eval_parses()
{
    if (m_goldstandard == NULL || m_eval_parses == NULL) {
        qDebug() << 153 << "Lexicon.cpp: evaluation failed: GoldStandard or evaluation file not loaded";
        return false;
    }
    bool evaluation_succeeded = m_goldstandard->evaluate(m_eval_parses);
    if (evaluation_succeeded) {
        qDebug() << 158 << "Lexicon.cpp: evaluation on imported parses completed";
        return true;
    } else return false;
}


bool CLexicon::stem_autobiographies_contains(const QString& stem) {
    return m_stem_autobiographies.contains(stem);
}

void CLexicon::add_to_stem_autobiographies(const QString& stem, const QString& message)
{
    if (! m_stem_autobiographies.contains(stem)){
        m_stem_autobiographies[stem] = new QStringList;
    }
    m_stem_autobiographies[stem]->append(message);
}

bool CLexicon::word_autobiographies_contains(const QString& word) {
    return m_word_autobiographies.contains(word);
}

void CLexicon::add_to_word_autobiographies(const QString& word, const QString& message)
{
    if (! m_word_autobiographies.contains(word)){
        m_word_autobiographies[word] = new QStringList;
    }
    if (!m_word_autobiographies.contains(message))  //JohnG August 2019
    {
        m_word_autobiographies[word]->append(message);
    }
    //qDebug() << 244 << word << message;
}
void CLexicon::word_autobiography_positive_notice(QString word, QString stem, QString sig_string, QString calling_function){
    add_to_word_autobiographies(word,
                                QString("[%1]==stem: %2=%3")
                                .arg(calling_function)
                                .arg(stem)
                                .arg(sig_string));
}

void CLexicon::time_stamp(const QString& message)
{    QMapIterator<QString, CStem*> stem_iter (*m_suffixal_stems->get_map());
    while (stem_iter.hasNext()){
        stem_iter.next();
        QString stemstring = stem_iter.key();
        if (!m_stem_autobiographies.contains(stemstring)){
            m_stem_autobiographies[stemstring] = new QStringList();
        }
        m_stem_autobiographies[stemstring]->append(message);
    }
}

void CLexicon::dump_signatures_to_debug()
{
    qDebug() << "********  signature dump ********************* ";
    int i = 0;
    foreach (CSignature* pSig, *m_Signatures->get_signature_list()){
        i++;
        qDebug()<< i << pSig->get_key();
    }
    qDebug() << "******** end of signature dump ********************* ";

}

