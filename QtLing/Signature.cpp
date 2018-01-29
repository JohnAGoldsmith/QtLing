#include "Signature.h"
#include "SignatureCollection.h"
#include "Suffix.h"
#include <QDebug>
#include <QChar>
#include <QtMath>
#include "Lexicon.h"
#include "WordCollection.h"
#include "Word.h"

CSignature::CSignature(QString signature_string, bool suffix_flag, CSignatureCollection* Signatures)
{
  m_Signature = signature_string;
  m_Stems = new CStem_ptr_list();
  m_Suffixes = new CSuffix_ptr_list;
  m_Prefixes = new CPrefix_ptr_list;
  m_SuffixFlag = suffix_flag;
  m_SignatureCollection = Signatures;
  m_stem_entropy = 0;
}

CSignature::CSignature(CSignature& signature) {
    m_Signature = signature.GetSignature();
    m_Prefixes = signature.get_prefix_list();
    m_Suffixes = signature.get_suffix_list();
    m_Stems = new CStem_ptr_list();
    m_SuffixFlag = signature.get_suffix_flag();
    m_stem_entropy = 0;
}
CSignature::~CSignature()
{
  delete m_Stems;
  delete m_Prefixes;
  delete m_Suffixes;
}
QStringList CSignature::get_string_list(QStringList& affix_string_list){
    affix_string_list.clear();
    affix_string_list = m_Signature.split("=");
    return affix_string_list;
}


void CSignature::add_stem_pointer(CStem* pStem)
{
    if (m_Stems->contains(pStem)){
        return;
    }
    m_Stems->append(pStem);
}

bool compare_stems_of_sig(const CStem* pStem1, const CStem* pStem2)
{
 return  pStem1->get_key() < pStem2->get_key();
}
void CSignature::sort_stems(){
    qSort(m_Stems->begin(), m_Stems->end(), compare_stems_of_sig);
}

bool CSignature::contains_affix_string(affix_t affix)
{
    QStringList affixes = m_Signature.split("=");
    if (affixes.contains(affix) ){
        return true;
    }
    return false;
}


void CSignature::dump_string_set_of_suffixes(QSet<QString> & this_StringSet)
{   QString affix;
    QStringList affixlist = m_Signature.split("=");
    foreach (affix, affixlist){
        this_StringSet.insert(affix);
    }


}

QString CSignature::display()
{
    return m_Signature;
}

QString CSignature::display_stems()
{
    QString outstring;
    CStem *qStem;
    for (int stem_no = 0; stem_no < m_Stems->size(); stem_no ++ ){
        outstring += m_Stems->at(stem_no)->display();
    }
    return outstring;
}
int CSignature::get_robustness() const
{
    int robustness = 0;
    int stem_letters = 0;
    int suffix_letters = 0;
    if (m_SuffixFlag){
        for (int suffix_no=0; suffix_no < m_Suffixes->size(); suffix_no++){
            if (m_Suffixes->at(suffix_no)->get_key() == "NULL"){
                suffix_letters += 1;
            } else{
                suffix_letters += m_Suffixes->at(suffix_no)->get_key().length();
            }
        }
    }
    for (int stem_no=0;stem_no<m_Stems->size();stem_no++){
        stem_letters += m_Stems->at(stem_no)->get_key().length();
    }
    robustness = stem_letters + suffix_letters;
    return robustness;
}
 int CSignature::get_number_of_affixes() const
{    if (m_SuffixFlag){
         if (m_Suffixes->count() > 0)
         {return m_Suffixes->count();}
         else {return m_Signature.count("=") + 1; }
     }
     else{
         if (m_Prefixes->count() > 0)
         {return m_Prefixes->count();}
         else {return m_Signature.count("=") + 1; }
     }
}

 bool CSignature::contains(CSignature *other) {
    if (m_SuffixFlag){
        for (int i  = 0; i < other->get_number_of_affixes(); i++){
            CSuffix* pSuffix = other->get_suffix_list()->at(i);
            if (! get_suffix_list()->contains (pSuffix)){
                return false;
            }
        }

    return true;
    } else {            // -->  prefixes <-- //
    for (int i  = 0; i < other->get_number_of_affixes(); i++){
            CPrefix* pPrefix = other->get_prefix_list()->at(i);
            if (! get_prefix_list()->contains (pPrefix)){
                return false;
            }
    }
    return true;
    }
}

word_and_count_list * CSignature::get_word_and_count_vectors(word_and_count_list * this_vector)
// this produces a list of word_and_count's: each is a word and its corpus count, one for each affix in the signature.
{   QString this_word;
    int word_count =0;
    if (m_SuffixFlag){
        for (int stem_no = 0; stem_no < m_Stems->size(); stem_no++){
            stem_t this_stem = m_Stems->at(stem_no)->get_key();
            for (int suff_no = 0; suff_no < m_Suffixes->size();suff_no++){
                suffix_t this_suffix = m_Suffixes->at(suff_no)->get_key();
                if (this_suffix == "NULL"){
                    this_word = this_stem;
                } else{
                    this_word = this_stem + this_suffix;
                }
                CWord* pWord = get_signatures()->get_lexicon()->get_word_collection()->find_or_fail(this_word);
                if (pWord) {
                    word_and_count* pWord_and_count = new word_and_count(pWord->get_key(),pWord->get_word_count());
                    this_vector->append(pWord_and_count);
                }else{
                    //  FAILURE --
                }
            }
        }
    }
    return this_vector;
}

double log_base_2(double x)
{   //qDebug() << x << qLn(x)/qLn(2) << "signature.cpp 162";
    return qLn(x) / qLn(2.0);
}

double CSignature::calculate_stem_entropy()
{
    QMap<QChar,double> counts;
    double     total_count (0);
    CStem* pStem;
    QChar  letter;
    double entropy =0.0;
    foreach (pStem, *m_Stems){
        stem_t this_stem = pStem->get_key();
        m_SuffixFlag?
            letter = this_stem.at(this_stem.length()-1):
            letter = this_stem.at(0);
        if (counts.contains(letter)){
                counts[letter]++;
        } else{
            counts[letter] = 1;
        }
        qDebug() << this_stem << letter << counts[letter];
        total_count++;
    }
    if (counts.size() == 1){
        m_stem_entropy = 0.0;
        qDebug() << "zero entropy" << this->get_key();
        return 0.0;
    }
    QMapIterator<QChar,double> this_iter (counts);
    while (this_iter.hasNext()){
        letter = this_iter.next().key();
        double freq = counts[letter]/total_count;
        entropy += -1.0 * freq * log_base_2 (freq);
    }
    m_stem_entropy = entropy;
    qDebug() << "Stem entropy of signature"<<entropy << 195;
    return entropy;
}
/////////////////////////////////////////////////////////////////////////
//
//      non-class functions dealing with signatures
//
/////////////////////////////////////////////////////////////////////////

sigstring_t restructure_signature(sigstring_t sig, QString morph, QStringList new_affixes)
{
    QStringList affixes = sig.split("=");
    QStringList new_sig;
    affix_t     affix;
    sigstring_t new_signature;

    qDebug() << "restructuring 1: old signature" << sig << "morph:" << morph << "new affixes:" << new_affixes.join("=");
    for (int i = 0; i < affixes.count(); i++){
        affix = affixes[i];
        if (! new_affixes.contains(affix)){
            new_sig.append(affix);
        }
    }
    new_sig.append(morph);
    new_sig.sort();
    new_signature = new_sig.join("=");
    qDebug() <<"restructuring 2 old sig:" << sig << "new signature:"<< new_signature << morph;
    return new_signature;
}

