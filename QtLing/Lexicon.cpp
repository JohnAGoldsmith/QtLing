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

CLexicon::CLexicon( CLexicon* lexicon, bool suffix_flag)
{
    m_Signatures            = new CSignatureCollection(this, true);
    m_PrefixSignatures      = new CSignatureCollection(this,false);
    m_Words                 = new CWordCollection(this);
    m_prefixal_stems        = new CStemCollection(this);
    m_suffixal_stems        = new CStemCollection(this);
    m_Suffixes              = new CSuffixCollection(this);
    m_Prefixes              = new CPrefixCollection(this);
    m_Compounds             = new CompoundWordCollection(this);
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
    delete m_Compounds;
    delete m_Parses;
}

CSignatureCollection* CLexicon::get_active_signature_collection(){
    if (m_SuffixesFlag){
        return m_Signatures;
    }else{
        return m_PrefixSignatures;
    }

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


bool CLexicon::stem_autobiographies_contains(QString stem) {
    //qDebug() << 121 << stem << m_stem_autobiographies.contains(stem);
    return m_stem_autobiographies.contains(stem);
}

void CLexicon::add_to_stem_autobiographies(QString stem, QString message)
{
    if (! m_stem_autobiographies.contains(stem)){
        //qDebug() << 128 << stem << message;
        m_stem_autobiographies[stem] = new QStringList;
    }
    m_stem_autobiographies[stem]->append("=" + message);
    //qDebug() << 125 << stem << message;

}

bool CLexicon::word_autobiographies_contains(QString word) {
    //qDebug() << 121 << stem << m_stem_autobiographies.contains(stem);
    return m_word_autobiographies.contains(word);
}

void CLexicon::add_to_word_autobiographies(QString word, QString message)
{
    if (! m_word_autobiographies.contains(word)){
        //qDebug() << 128 << stem << message;
        m_word_autobiographies[word] = new QStringList;
    }
    m_word_autobiographies[word]->append(message);
    //qDebug() << 125 << stem << message;

}

void CLexicon::time_stamp(QString message)
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


////////////////////////////////////////////////////////////////////////////////////////////////////
//linguistic methods
/**
 * @brief CLexicon::Crab_1
 * The division of the Crab algorithm into two parts (as I have done here) is only temporary.
 * I have done it so that I can do code development more quickly; sometimes I only need to
 * get part of the algorithm going to test what I am doing.
 */


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

/* Crab_1:
 * Used after MainWindow::read_dx1_file, which parses the dx1 file and
 * stores words and their counts into CWordCollection object in Lexicon,
 * and generates the SortedStringArrays.
 *
 */
void CLexicon::Crab_1()
{
    step1_find_protostems();

    step2_create_stem_affix_parses();

    time_stamp("crab 1");

    step3_assign_affixes_to_stems(QString("crab_1"));

    step4_create_signatures(QString("Crab1"));

    replace_parse_pairs_from_current_signature_structure();

    find_full_signatures();

    collect_parasuffixes();

    m_SuffixesFlag?
                m_Signatures->compute_containment_list():
                m_PrefixSignatures->compute_containment_list();

}

//  <-------------->

////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * This is the first of the three initial parts of finding signatures.
 * This makes a cut at every point in a word where the successor frequency
 * is greater than 1.
 * It is divided into two parts;  the first finds protostems, by detecting
 * successfor frequency greater than 1; the second breaks a word after a protostem.
 */
void CLexicon::step1_find_protostems()
{   word_t          this_word, previous_word;
    bool            StartFlag = true;
    bool            DifferenceFoundFlag = false;
    stem_t          potential_stem;
    int             this_word_length(0), previous_word_length(0);
    int             MINIMUM_STEM_LENGTH = 4;

    QStringList *  Words;
    if (m_SuffixesFlag){
        Words = get_word_collection()->GetSortedStringArray();
    }else{
        Words = get_word_collection()->get_reverse_sort_list();
    }

    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(Words->size());
    m_StatusBar->showMessage("1. Find proto-stems.");
    m_Parses->clear();
    //m_Parse_map.clear();


    int temp_j = 0;
    for (int wordno=0; wordno<Words->size(); wordno ++){

        temp_j++; if (temp_j++ == 5000) {temp_j = 0; m_ProgressBar->setValue(wordno); qApp->processEvents();}

        if (m_SuffixesFlag){
            this_word = Words->at(wordno);
        } else{
            this_word = get_words()->get_reverse_sort_list()->at(wordno);
        }
        this_word_length = this_word.length();
        if (StartFlag){
            StartFlag = false;
            previous_word = this_word;
            previous_word_length = this_word_length;
            continue;
        }
        DifferenceFoundFlag = false;
        int end = qMin(this_word_length, previous_word_length);
        if (m_SuffixesFlag){   // -->   Suffix case   <-- //
            for (int i=0; i <end; i++){
                if (i < MINIMUM_STEM_LENGTH ) continue;
                if (previous_word[i] != this_word[i]){
                    potential_stem = previous_word.left(i);
                    if (potential_stem.length()== 0){continue;}
                    DifferenceFoundFlag = true;
                    if (!m_suffix_protostems.contains(potential_stem)){
                        m_suffix_protostems[potential_stem] = new protostem(potential_stem, true);
                    }
                    break;
                } // end of having found a difference.
            }// end of loop over this word;
            if (DifferenceFoundFlag == false){
                if (!m_suffix_protostems.contains(previous_word)){
                    m_suffix_protostems[previous_word] = new protostem(previous_word, false);
                }
            }
        }  // end of suffix case.
        else
        {       // -->   Prefix case   <-- //
            for (int i=1; i <=end; i++){
                if (previous_word.right(i) != this_word.right(i)){
                    if (i-1 < MINIMUM_STEM_LENGTH) continue;
                    potential_stem = previous_word.right(i-1);
                    if (potential_stem.length() == 0) {continue;}
                    DifferenceFoundFlag = true;
                    if (!m_prefix_protostems.contains(potential_stem)) {
                        m_prefix_protostems[potential_stem] = new protostem(potential_stem);
                    }
                    break;
                }// end of having found a difference.
            } // end of loop over this word;
            if (DifferenceFoundFlag == false){
                if (!m_prefix_protostems.contains(previous_word)){
                    m_prefix_protostems[previous_word] = new protostem(previous_word);
                }
            }
        } // end of prefix case.
        previous_word = this_word;
        previous_word_length = this_word_length;
    } // word loop.
    //Second half:  finding protostem objects, which mark their beginning and their end on the alphabetized word list.
    if (m_SuffixesFlag){
        m_ProgressBar->reset();
        m_ProgressBar->setMinimum(0);
        m_ProgressBar->setMaximum(m_suffix_protostems.size());
        QStringList alphabetized_protostems = m_suffix_protostems.keys();
        alphabetized_protostems.sort();
        int alpha = 0;
        int omega = 0; int start_index = 0;

        for (int p = 0; p<alphabetized_protostems.length(); p++){
            QString this_protostem_t = alphabetized_protostems[p];
            int protostem_length = this_protostem_t.length();
            int i = alpha;
            while (Words->at(i).left(protostem_length) != this_protostem_t){
                i++;
            }
            int j = i;
            alpha = i;
            while (j < Words->length() && Words->at(j).left(protostem_length) == this_protostem_t){
                j++;
            }
            m_suffix_protostems[this_protostem_t]->set_start_and_end_word(i,j-1);
        }
    } // end of suffix case
    else { // beginning of prefix case
        m_ProgressBar->reset();
        m_ProgressBar->setMinimum(0);
        m_ProgressBar->setMaximum(m_prefix_protostems.size());
        QStringList alphabetized_protostems = m_prefix_protostems.keys();
        SortQStringListFromRight(alphabetized_protostems);
        int alpha = 0;
        int omega = 0; int start_index = 0;
        for (int p = 0; p<alphabetized_protostems.length(); p++){
            QString this_protostem_t = alphabetized_protostems[p];
            int protostem_length = this_protostem_t.length();
            int i = alpha;
            while (Words->at(i).right(protostem_length) != this_protostem_t){
                i++;
            }
            int j = i;
            alpha = i;
            while (j < Words->length() && Words->at(j).right(protostem_length) == this_protostem_t){
                j++;
            }
            m_prefix_protostems[this_protostem_t]->set_start_and_end_word(i,j-1);
        }
    } //end of prefix case.
    return;
}


/*!
 * This is the second of the three initial parts of finding signatures.
 * This creates stem/affix pairs, which are put in a long list of "Parses":
 * QList<QPair<QString,QString>>* m_Parses
 *
 */
void CLexicon::step2_create_stem_affix_parses()
{
    int MAXIMUM_AFFIX_LENGTH = 10;
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_Words->get_count());
    m_StatusBar->showMessage("2: find stem-affix pairs.");
    QString                     stem, suffix, word, prefix;
    int                         suffix_length, prefix_length, wordno (0);
    map_string_to_word_ptr_iter *   word_iter = m_Words->get_iterator();
    bool                        DoNotParseCompoundsFlag = true;
    while (word_iter->hasNext())   {
        word = word_iter->next().value()->get_word();
        if (DoNotParseCompoundsFlag and word.contains("-")){
            //*m_Compounds << word;
            continue;
        }

        wordno++;
        qApp->processEvents();
        m_ProgressBar->setValue(wordno);
        int MinimumStemLength = 5;
        for (int letterno = MinimumStemLength; letterno < word.length(); letterno++){
            if (m_SuffixesFlag){
                stem = word.left(letterno);
                if(m_suffix_protostems.contains(stem))  {
                    suffix_length = word.length() - letterno;
                    if (suffix_length > MAXIMUM_AFFIX_LENGTH){
                        continue;
                    }
                    suffix = word.right(suffix_length);
                    CParse* this_parse = new CParse(stem, suffix);
                    m_Parses->append(this_parse);
                    if (m_Words->contains(stem)){
                        CParse* that_parse = new CParse(stem, QString("NULL"), true );
                        m_Parses->append(that_parse);
                    }
                }
            }else{
                stem = word.right(letterno);
                if (m_prefix_protostems.contains(stem)){
                    prefix_length = word.length() - letterno;
                    if (prefix_length > MAXIMUM_AFFIX_LENGTH){
                        continue;
                    }
                    prefix = word.left(prefix_length);
                    CParse* this_parse = new CParse(prefix, stem);
                    m_Parses->append(this_parse);
                    if (m_Words->contains(stem)){
                        CParse * that_parse = new CParse(QString("NULL"), stem, false);
                        m_Parses->append(that_parse);
                    }
                }
            } // end of prefixes.
        }
    }
}

// ==========================================================================  //

QString convert_set_to_qstring(affix_set* this_affix_set){
    QStringList temp_presignature;
    foreach(QString this_affix, *this_affix_set)
    {
        temp_presignature.append(this_affix);
    }
    temp_presignature.sort();
    return  temp_presignature.join("=");
}

// ==========================================================================  //



/*!
 * This is the third of the three initial parts of finding signatures.
 * This creates signatures, which in turn creates stems and affixes.
 * It is divided into two parts. The second part is reused by later functions.
 */
void   CLexicon::step3_assign_affixes_to_stems(QString name_of_calling_function)
{  // const int MINIMUM_NUMBER_OF_STEMS = 2;

    QString                     this_stem_t, this_suffix, this_prefix, this_affix_t, this_signature_string, this_word;
    Protosigs                   these_protosigs;
    m_intermediate_signature_to_stems_map.clear();   //replaces "this_signature_to_stems_map"

    morph_set *                 pSet;
    CWord*                      pWord;

    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_Parses->size());
    m_StatusBar->showMessage("3: assign affixes to stems.");
    qApp->processEvents();
    clear_lexicon();

    //--> We establish a temporary map from stems to sets of affixes as we iterate through parses. <--//

    step3a_take_stem_affix_parses_and_create_protosigs(m_Parses, m_SuffixesFlag, &these_protosigs);

    //-----------------------------------------------------------------------------------------------//

    //--> We iterate through these stems and for each stem, create QStringLists of their affixes. <--//
    //--> then we create a "pre-signature" in a map that points to lists of stems. <--//
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(these_protosigs.count());

    int count= 0;
    foreach(QString this_stem_t, these_protosigs.keys()){
        count++; m_ProgressBar->setValue(count);
        this_signature_string = convert_set_to_qstring( these_protosigs[this_stem_t] );
        m_intermediate_signature_to_stems_map.attach_stem_to_signature(this_stem_t, this_signature_string);
    }
    //-----------------------------------------------------------------------------------------------//'

}




/*!
 * \brief step3a_take_stem_affix_parses_and_create_protosigs
 * \param parses
 * \param suffix_flag
 * \param these_protosigs
 * This function is called by step3 (just below)
 */

void CLexicon::step3a_take_stem_affix_parses_and_create_protosigs(QList<CParse*> * parses, bool suffix_flag,Protosigs* these_protosigs )
{       QString this_stem_t, this_affix_t;
        suffix_set * pSet;
            int i = 0;
                for (int parseno = 0; parseno < parses->size(); parseno++){
                    CParse * this_pair = parses->at(parseno);
                    if (suffix_flag){
                        this_stem_t = this_pair->get_string1();
                        this_affix_t = this_pair->get_string2();
                    } else{
                        this_stem_t = this_pair->get_string2();
                        this_affix_t = this_pair->get_string1();
                    }
                    i++;
                    if (i == 100000){i = 0;
                        m_StatusBar->showMessage("3a: "+ this_stem_t + this_affix_t);
                        qApp->processEvents();

                    }

                    if (! these_protosigs->contains(this_stem_t)){
                        if (suffix_flag){
                            pSet = new suffix_set();
                        } else{
                            pSet = new prefix_set();
                        }
                        these_protosigs->insert(this_stem_t, pSet);
                    }
                    these_protosigs->value(this_stem_t)->insert(this_affix_t);
                }
};
// ==========================================================================  //


void CLexicon::step4_create_signatures(QString name_of_calling_function)
{
    sigstring_t             this_signature_string;
    stem_t                  this_stem_t;
    affix_t                 this_affix_t;
    word_t                  this_word_t;
    CWord*                  pWord;

    const int               MINIMUM_NUMBER_OF_STEMS = 8;
    CSignature*             pSig;

    //-->  create signatures, stems, affixes:  <--//
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_intermediate_signature_to_stems_map.m_core.count());
    m_StatusBar->showMessage("4: create signatures.");


    if (m_SuffixesFlag){
        m_Suffixes->clear();
        m_Signatures->clear();
    }
    else {
        m_Prefixes->clear();
        m_PrefixSignatures->clear();
    }
    // -->  Iterate through tentative signatures.    <-- //
    int temp_i = 0;
    int count (0);
    foreach (this_signature_string, m_intermediate_signature_to_stems_map.m_core.keys() )
    {
        qApp->processEvents();
        count++;
        m_ProgressBar->setValue(count);
        temp_i++;
        if (temp_i == 1000){
            temp_i = 0;
            m_StatusBar->showMessage("4: create signatures: " + this_signature_string);
            qApp->processEvents();
        }

        QSet<stem_t> * this_stem_set = m_intermediate_signature_to_stems_map.get_stem_set(this_signature_string);

        affix_list this_affix_list = this_signature_string.split("=");
        if (this_stem_set->size() >= MINIMUM_NUMBER_OF_STEMS)
        {
            /* [1] Create CSignature object
             * Add signature to m_Signatures
             */
            if( m_SuffixesFlag) {
                // CSignature* pSig;
                pSig = *m_Signatures       << this_signature_string;
            } else {
                pSig = *m_PrefixSignatures << this_signature_string;
                pSig->set_suffix_flag(false);
            }
            foreach (QString this_affix_t, this_affix_list){
                step4a_link_signature_and_affix(pSig,this_affix_t);
            }
            foreach (this_stem_t, *this_stem_set){
                step4b_link_signature_and_stem_and_word(this_stem_t,pSig, this_signature_string);
            }
        } // end of condition of having enough stems in the signature.
        else
        {   // if there are not enough stems for this signature: this is here just for words ability to remember where they were and how they were analyzed.

            foreach(this_stem_t, *this_stem_set){
                foreach (this_affix_t,this_affix_list){
                    if (this_affix_t == "NULL"){
                        this_affix_t = "";
                    }
                    m_SuffixesFlag?
                                this_word_t = this_stem_t + this_affix_t:
                            this_word_t = this_affix_t + this_stem_t;
                    pWord = m_Words->find_or_fail(this_word_t);
                    pWord->add_to_autobiography("failure of " + name_of_calling_function + "=" + this_stem_t + "=" + this_signature_string );
                }
            }
        }
    }

    m_Suffixes->sort_by_count();
    m_Prefixes->sort_by_count();
    m_SuffixesFlag ?
                m_Signatures->calculate_stem_entropy():
                m_PrefixSignatures->calculate_stem_entropy();
    m_StatusBar->showMessage("Computation of Crab 1 completed.");
}
void CLexicon::step4a_link_signature_and_affix(CSignature * pSig, affix_t this_affix)
{
    if (m_SuffixesFlag){
        CSuffix* pSuffix = m_Suffixes->find_or_add(this_affix);
        pSuffix->increment_count();
        pSig->add_affix_ptr(pSuffix);
        //qDebug() << 686 << this_affix;
    } else {
        CPrefix* pPrefix = m_Prefixes->find_or_add(this_affix);
        pPrefix->increment_count();
        pSig->add_affix_ptr(pPrefix);
    }
}
void CLexicon::step4b_link_signature_and_stem_and_word(stem_t this_stem_t , CSignature*  pSig,  QString this_signature_string)
{
    CStem* pStem;
    QString this_affix, this_word;
    m_SuffixesFlag ?
                pStem = m_suffixal_stems->find_or_add(this_stem_t):
            pStem = m_prefixal_stems->find_or_add(this_stem_t);
    pStem->add_signature (pSig);
    pSig->add_stem_pointer(pStem);
    add_to_stem_autobiographies(this_stem_t, this_signature_string);


    int stem_count = 0;
    affix_list this_affix_list = this_signature_string.split("=");
    foreach (this_affix, this_affix_list){
        if (this_affix == "NULL"){
            this_word = this_stem_t;
        } else{
            m_SuffixesFlag ?
                    this_word = this_stem_t + this_affix :
                    this_word = this_affix + this_stem_t ;
        }
        CWord* pWord = m_Words->get_word(this_word);
        if (!pWord){
            qDebug() << this_word <<  "Error: this_word not found among words. Line 577" << this_stem_t  << this_affix << pSig->get_key() << this_signature_string;
        } else{
            stem_count += pWord->get_word_count();
            pWord->add_parse_triple(this_stem_t, this_affix, pSig->get_key());
            QString message = this_signature_string;
            if (this_affix_list.size() > 50){message = "Super long signature";};
            pWord->add_to_autobiography(QString("Normal formation") + "=" + this_stem_t + "=" + message);
        }
    }
    pStem->set_count(stem_count);

}


bool contains(QList<QString> * list2, QList<QString> * list1){
    for (int i=0; i < list1->length();i++){
        bool success = false;
        for (int j = 0; j < list2->length();j++){
            if (list1->at(i) == list2->at(j)){
                success = true;
            }
        }
        if (success == false){
            return false;
        }
    }
    return true;
}

void CLexicon::find_compounds()
{
    int MIN_STEM_LENGTH = 2;
    CStemCollection* p_stems = m_SuffixesFlag ? m_suffixal_stems : m_prefixal_stems;
    const QMap<QString, protostem*>& ref_protostem_map
            = m_SuffixesFlag ? m_suffix_protostems : m_prefix_protostems;

    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(p_stems->get_count());
    m_StatusBar->showMessage("5: Finding Compounds - part 1.");
    int itercount = 0;

    // iterate through stems
    QMap<QString, CStem*>* p_stem_map = p_stems->get_map();
    QMap<QString, CStem*>::ConstIterator stem_map_iter;
    for (stem_map_iter = p_stem_map->constBegin();
         stem_map_iter != p_stem_map->constEnd();
         stem_map_iter++) {
        m_ProgressBar->setValue(itercount++);

        // for each stem, find its corresponding protostem
        const QString& str_stem = stem_map_iter.key();
        CStem* p_stem = stem_map_iter.value();
        protostem* p_protostem = ref_protostem_map[str_stem];
        int stem_length = str_stem.length();

        if (stem_length < MIN_STEM_LENGTH) continue;

        // iterate through all words in the protostem
        int wordno;
        int end_word = p_protostem->get_end_word();
        for (wordno = p_protostem->get_start_word();
             wordno <= end_word;
             wordno++) {
            // find the continuation of a word, i.e. the part of the word
            // left over after stem part is removed
            const QString& str_word = m_Words->get_word_string(wordno);
            const QString str_continuation = m_SuffixesFlag ?
                        str_word.mid(stem_length) :
                        str_word.left(str_word.length()-stem_length);
            bool continuation_valid = true;
            // determine if the continuation is already in stem's signature,
            // skip if it is.
            CSignature* p_signature;
            foreach (p_signature, *(p_stem->GetSignatures())) {
                if (m_SuffixesFlag) {
                    QList<CSuffix*>* p_suffixes = p_signature->get_suffix_list();
                    CSuffix* p_suffix;
                    foreach (p_suffix, *p_suffixes) {
                        if (p_suffix->get_key() == str_continuation) {
                            continuation_valid = false;
                            break;
                        }
                    }
                } else {
                    QList<CPrefix*>* p_prefixes = p_signature->get_prefix_list();
                    CPrefix* p_prefix;
                    foreach (p_prefix, *p_prefixes) {
                        if (p_prefix->get_key() == str_continuation) {
                            continuation_valid = false;
                            break;
                        }
                    }
                }
            }
            if (!continuation_valid) continue;

            // determine if the continuation is a word or a stem,
            // skip if it is not.
            if (m_Words->get_word(str_continuation) == NULL) {
                continuation_valid = (p_stems->find_or_fail(str_continuation) != NULL);
            } else {
                continuation_valid = true;
            }
            if (!continuation_valid) continue;

            m_Compounds->add_compound_word(str_word, str_stem, str_continuation);
        }
    }
    // finished part 1

    // starting part 2
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(p_stems->get_count());
    m_StatusBar->showMessage("9: Finding Compounds - part 2: "
                             "removing invalid components.");
    itercount = 0;

    QList<CompoundComponent*> list_to_remove;
    CompoundComponentCollection* p_components = m_Compounds->get_components();
    QMap<QString, CompoundComponent*>& ref_components_map = p_components->get_map();
    qDebug() << QString("Found %1 components").arg(ref_components_map.size());
    QMap<QString, CompoundComponent*>::iterator components_iter;
    for (components_iter = ref_components_map.begin();
         components_iter != ref_components_map.end();
         components_iter++) {
        m_ProgressBar->setValue(itercount++);
        CompoundComponent* p_component = components_iter.value();
        qDebug() << QString("Checking validity of component: [%1]").arg(p_component->get_word());
        if (!p_component->check_valid()) {
            qDebug() << "\tinvalid.";
            list_to_remove.append(p_component);
        } else {
            qDebug() << "\tvalid.";
        }
    }
    CompoundComponent* p_component_to_remove;
    qDebug() << "Removing invalid components";
    foreach (p_component_to_remove, list_to_remove) {
        p_components->remove_component(p_component_to_remove);
    }

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      End of Crab 1
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      Find full signatures

void remove_final_letter (QStringList & stem_list, bool suffix_flag){
    for (int i = 0; i < stem_list.size(); i++){
        if (suffix_flag){
            stem_list[i] = stem_list[i].left(stem_list[i].length()-1);
        }else{
            stem_list[i] = stem_list[i].mid(1);
        }
    }
}

void add_initial_letter (QStringList & affix_list, QString letter, bool suffix_flag){
    for (int i = 0; i < affix_list.size(); i++){
        if (affix_list[i]== "NULL"){
            affix_list[i] = letter;
        } else {
            if (suffix_flag){
                affix_list[i] = letter + affix_list[i];
            }else{
                affix_list[i] = affix_list[i] + letter;
            }
        }
    }
}

void CLexicon::find_full_signatures()
{
/*  1. iterate through signatures.
        a. if the edge has zero-entropy, make a shorter stem, add the stem-affix pair to Parses, for all stems in sig.
        b. keep pairs of (new stem, whole word) and see if the set of new stems has zero-entropy. if so, repeat. If not, exit.
    2. Call AssignSuffixesToStems.
*/
    int temp_i = 0;
    m_StatusBar->showMessage("5: Find full signatures inside hollow signatures.");
    qApp->processEvents();
    CStemCollection * stems;
    CSignature * pSig;
    QString letter, affix, stem;
    QStringList affix_list, stem_list;
    CSignatureCollection * signatures;
    m_SuffixesFlag ?
                stems = m_suffixal_stems:
            stems = m_prefixal_stems;
    m_SuffixesFlag ?
                signatures = m_Signatures:
            signatures = m_PrefixSignatures;
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(signatures->get_count());
    QMapIterator<sigstring_t,CSignature*> * sig_iter = new QMapIterator<sigstring_t,CSignature*> (* signatures->get_map() );
    while (sig_iter->hasNext()){

        pSig = sig_iter->next().value();

        temp_i++;
        if (temp_i == 10000){
            temp_i = 0;
            m_StatusBar->showMessage("5: Find full signatures inside hollow signatures: " + pSig->display() );
            qApp->processEvents();
        }

        //qDebug() << 774 << pSig->display();
        if (pSig->get_stem_entropy() > 0){
            continue;
        }
        m_SuffixesFlag?
                    letter = pSig->get_stems()->at(0)->get_key().right(1):
                letter = pSig->get_stems()->at(0)->get_key().left(1);
        stem_list.clear();
        pSig->get_stem_strings(stem_list); // save list of stems into stem_list
        remove_final_letter(stem_list, m_SuffixesFlag);
        pSig->get_string_list(affix_list);
        add_initial_letter (affix_list, letter, m_SuffixesFlag);

        CParse* pParse;
        for (int i = 0; i < stem_list.size(); i++){
            stem = stem_list.at(i);
            for (int affno = 0; affno < affix_list.size(); affno++){
                affix = affix_list.at(affno);
                if (m_SuffixesFlag){
                    pParse = new CParse(stem,affix);
                    m_Parses->append(pParse);
                } else{
                    pParse = new CParse(affix, stem);
                    m_Parses->append(pParse);
                    if (stem.length() == 0 ){
                        qDebug() << 523 << "null stem"   ;
                    }
                }
            }
        }
    }

    step4_create_signatures("Finding full signatures");

}




/*
struct{
    bool operator ()(CSignature* pSig_a, CSignature* pSig_b) const {
    // return pSig_a->number_of_true_suffixes() > pSig_b->number_of_true_suffixes();
    }
}custom_compare_residual_sig;
*/


/*!
 * Replace parse pairs from current signature structure. This allows us to
 * delete the old signature structure, and replace them with the set of
 * parse-pairs that exactly describe the current signature structure.
 */

void CLexicon::replace_parse_pairs_from_current_signature_structure(bool  ) {


    m_Parses->clear();
    CParse*                         this_parse;
    //m_Parse_map.clear();
    QString                         sig_string;
    QStringList                     affixes;
    affix_t                         this_affix;
    CSignature*                     pSig;
    CStem*                          pStem;
    QList<CStem*> *                 stem_list;
    QString                         this_stem;
    map_sigstring_to_sig_ptr_iter  * sig_iter =   new map_sigstring_to_sig_ptr_iter(*  get_signatures()->get_map() );
    QList<CSignature*> *           these_signatures;

    m_SuffixesFlag?
                these_signatures = m_Signatures->get_signature_list():
            these_signatures = m_PrefixSignatures->get_signature_list();
    foreach (pSig, *these_signatures ){
        affixes = pSig->display().split("=");
        stem_list = pSig->get_stems();
        foreach (pStem, *stem_list){
            this_stem = pStem->display();
            foreach (this_affix, affixes){
                if (this_affix == "NULL") this_affix = "";
                m_SuffixesFlag?
                            this_parse = new CParse(this_stem, this_affix, true):
                        this_parse = new CParse(this_affix, this_stem, false);
                m_Parses->append(this_parse);

            }
        }

    }

    while (sig_iter->hasNext()){
        pSig = sig_iter->next().value();
        affixes = pSig->display().split("=");

        stem_list =  pSig->get_stems();
        foreach (CStem* p_this_stem, *stem_list){
            foreach (this_affix, affixes){
                if (this_affix == "NULL") this_affix = "";
                m_SuffixesFlag?
                            this_parse = new CParse(this_stem, this_affix, true):
                        this_parse = new CParse(this_affix, this_stem, false);
            }
        }
    }
}





/*!
 * This function looks at pairs of signatures joined by a sig-tree-edge, where
 * the morpheme that separates them is a single letter. We look to see  how tight
 * the fit is between these two sets of signatures.
 */
void CLexicon::test_for_phonological_relations_between_signatures()
{
    lxa_sig_graph_edge_map_iter  sig_iter (m_SigGraphEdgeMap);
    QString difference;
    QSet<QString> differences_1_letter, differences_longer;
    while (sig_iter.hasNext()){
        sig_iter.next();
        QString morph = sig_iter.value()->morph;
        if (morph.length() == 1){
            differences_1_letter.insert(sig_iter.value()->morph);
        } else{
            differences_longer.insert(sig_iter.value()->morph);
        }
    }

    QStringList differences = differences_1_letter.toList();
    differences.sort();
    QSet<CSignature*> SignatureSet_1, SignatureSet_2;

    for (int i = 0; i < differences.size(); i++){
        difference = differences[i];
        //--> Pull out the sig_graph_edges with each particular difference (morpheme).
        sig_iter.toFront();
        while (sig_iter.hasNext()){
            sig_iter.next();
            if (sig_iter.value()->morph == difference){
                CSignature* pSig1 = sig_iter.value()->m_sig_1;
                CSignature* pSig2 = sig_iter.value()->m_sig_2;
                SignatureSet_1.insert(pSig1);
                SignatureSet_2.insert(pSig2);
                *m_PassiveSignatures << pSig1;
            }
        }

        //--> Now these two sets have signatures which are from opposite sides of the phonological rule. <---//
        compare_opposite_sets_of_signatures(&SignatureSet_1, &SignatureSet_2, difference);

    }
}

/*!
 * This function takes two sets of signatures, each taken from the opposing signatures
 * in a sig-tree-edge, where the morph separating them is a specific morph of length = 1 letter.
 *
 * This is not currently being used.
 */

void CLexicon::compare_opposite_sets_of_signatures(QSet<CSignature*>* sig_set_1, QSet<CSignature*>* sig_set_2, QString morph)
{   sig_graph_edge * p_edge;
    CSignature* pSig_1, *pSig_2;
    QHash<QString,int> Counts;
    foreach(p_edge,  m_SigGraphEdgeMap){
        if (p_edge->morph == morph){
            pSig_1 = p_edge->m_sig_1;
            pSig_2 = p_edge->m_sig_2;
            QString code = pSig_1->get_key() + "@" + pSig_2->get_key();
            Counts[code] = p_edge->get_number_of_words();
        }
    }
}



void CLexicon::dump_suffixes(QList<QString> * pList)
{
    return m_Suffixes->get_suffixes(pList);
}

/**
 * @brief CLexicon::collect_parasuffixes
 * Parasignatures are signatures with only a single stem, hence are not used directly.
 * Parasuffixes are affixes found in a parasignature.
 *
 *
 */

void CLexicon::collect_parasuffixes()
{
    sigstring_t     sigstring;
    suffix_t        suffix;
    CSignature*     pSig;
    CSuffix *       pSuffix;
    QStringList     suffixes;
    map_sigstring_to_sig_ptr_iter sig_iter (* m_ParaSignatures->get_map());

    while (sig_iter.hasNext())
    {
        pSig = sig_iter.next().value();
        sigstring = pSig->get_key();
        suffixes = sigstring.split("=");
        foreach (suffix, suffixes){
            pSuffix = *m_ParaSuffixes <<  suffix;
            pSuffix->increment_count();
        }
    }
    m_ParaSuffixes->sort_by_count();
}
