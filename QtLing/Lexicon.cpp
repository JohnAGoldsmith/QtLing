#include <QPair>
#include <QList>
#include <QMap>
#include <QMapIterator>
#include <QMultiMap>
#include <QSet>
#include <QtDebug>
#include <QProgressBar>
#include <QApplication>
#include <algorithm>
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

void SortQStringListFromRight(QStringList& ThisStringList);
bool reverse_string_compare(QString,QString);

protostem::protostem(QString stem, int start_word, int end_word, bool suffix_flag)
{
    m_protostem = stem;
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

CLexicon::CLexicon( CLexicon* lexicon, bool suffix_flag):
    M_MINIMUM_STEM_LENGTH(4),
    M_MINIMUM_STEM_COUNT(8),
    M_MAXIMUM_AFFIX_LENGTH(10),
    M_MINIMUM_HYPOTHESIS_WORD_COUNT(6) // Moved to here, originally a fxn variable defined in step 9
{
    m_Signatures            = new CSignatureCollection(this, true);
    m_PrefixSignatures      = new CSignatureCollection(this,false);
    m_Words                 = new CWordCollection(this);
    m_prefixal_stems        = new CStemCollection(this);
    m_suffixal_stems        = new CStemCollection(this);
    m_Suffixes              = new CSuffixCollection(this);
    m_Prefixes              = new CPrefixCollection(this);
    //    m_Parses                = new QList<QPair<QString,QString>>();
    m_Parses                = new QList<CParse*>();
    m_ParaSignatures        =  new CSignatureCollection(this, true);
    m_ParaSuffixes          = new CSuffixCollection(this);
    m_ResidualStems         = new CStemCollection(this);
    m_PassiveSignatures     = new CSignatureCollection(this, true);
    m_SuffixesFlag          = suffix_flag;
    m_Hypotheses            = new QList<CHypothesis*>;
    m_Hypothesis_map        = new QMap<QString, CHypothesis*>;
    m_entropy_threshold_for_stems = 1.2;
    m_parent_lexicon        = lexicon;
    m_goldstandard          = NULL;
    m_eval_parses      = NULL;

    // Put this back in, Hanson 8.1
    m_Compounds             = new CompoundWordCollection(this);

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
    m_category_types["Hypotheses 2"]          = CT_hypothesis2;
    // add component 2
}


QListIterator<simple_sig_graph_edge*> * CLexicon::get_sig_graph_edge_list_iter()
{
    QListIterator<simple_sig_graph_edge*> * iter = new QListIterator<simple_sig_graph_edge*>(m_SigGraphEdgeList);
    return iter;
}

QMapIterator<QString, sig_graph_edge*> * CLexicon::get_sig_graph_edge_map_iter()
{
    QMapIterator<QString, sig_graph_edge*> * iter = new QMapIterator<QString, sig_graph_edge*>(m_SigGraphEdgeMap);
    return iter;
}

CLexicon::~CLexicon()
{
    delete m_Signatures;
    delete m_PrefixSignatures;
    delete m_ParaSignatures;
    delete m_PassiveSignatures;
    delete m_goldstandard;
    delete m_Parses;
}

CSignatureCollection* CLexicon::get_active_signature_collection(){
    if (m_SuffixesFlag){
        return m_Signatures;
    }else{
        return m_PrefixSignatures;
    }

}

void CLexicon::clear_parses()
{
    CParse* p_parse;
    foreach (p_parse, *m_Parses)
        delete p_parse;
    m_Parses->clear();
}

void CLexicon::clear_lexicon(){

    QMapIterator<QString, protostem*> stem_iter(m_prefix_protostems);
    while(stem_iter.hasNext()){
        stem_iter.next();
        //delete stem_iter.value();
    }

    delete m_Signatures;
    m_Signatures = new CSignatureCollection(this, true);
    delete m_PrefixSignatures;
    m_PrefixSignatures = new CSignatureCollection(this, false);
    delete m_prefixal_stems;
    m_prefixal_stems = new CStemCollection(this);
    delete m_suffixal_stems;
    m_suffixal_stems = new CStemCollection(this);
    delete m_Suffixes;
    m_Suffixes = new CSuffixCollection(this);
    delete m_Prefixes;
    m_Prefixes = new CPrefixCollection(this);
    delete m_ParaSignatures;
    m_ParaSignatures = new CSignatureCollection(this, true);
    delete m_ParaSuffixes;
    m_ParaSuffixes = new CSuffixCollection(this);
    delete m_ResidualStems;
    m_ResidualStems = new CStemCollection(this);
    delete m_PassiveSignatures;
    m_PassiveSignatures = new CSignatureCollection(this);
    delete m_Hypotheses;
    m_Hypotheses = new QList<CHypothesis*>;


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
    m_word_autobiographies[word]->append(message);

}

void CLexicon::time_stamp(const QString& message)
{
    QMapIterator<QString, CStem*> stem_iter (*m_suffixal_stems->get_map());
    while (stem_iter.hasNext()){
        stem_iter.next();
        QString stemstring = stem_iter.key();
        if (!m_stem_autobiographies.contains(stemstring))
        { m_stem_autobiographies[stemstring] = new QStringList();}
        m_stem_autobiographies[stemstring]->append(message);
    }
    /*
    QMapIterator<QString, CWord*> word_iter (*m_Words->get_map());
    while (word_iter.hasNext()){
        word_iter.next();
        QString wordstring = word_iter.key();
        if (!m_word_autobiographies.contains(wordstring))
        { m_word_autobiographies[wordstring] = new QStringList();qDebug() << 151 << wordstring << message;}
        m_word_autobiographies[wordstring]->append(message);
    }
    */
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
