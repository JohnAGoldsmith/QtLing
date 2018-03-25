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
  m_stem_entropy = -1;
}

CSignature::CSignature(CSignature& signature) {
    m_Signature = signature.GetSignature();
    m_Prefixes = signature.get_prefix_list();
    m_Suffixes = signature.get_suffix_list();
    m_Stems = new CStem_ptr_list();
    m_SuffixFlag = signature.get_suffix_flag();
    m_stem_entropy = signature.get_stem_entropy();
}
CSignature::~CSignature()
{
  delete m_Stems;
  delete m_Prefixes;
  delete m_Suffixes;
}
QStringList& CSignature::get_string_list(QStringList& affix_string_list){
    affix_string_list.clear();
    affix_string_list = m_Signature.split("=");
    return affix_string_list;
}

QStringList& CSignature::get_stem_strings(QStringList & stem_list)
{
    for (int stemno = 0; stemno < m_Stems->size(); stemno++){
        stem_list.append(m_Stems->at(stemno)->get_key());
    }
    return stem_list;
}

// =============================================>  Functions involving string containment <===========================//
bool CSignature::contains(CSignature *other) {
   if (m_SuffixFlag){
       for (int i  = 0; i < other->get_number_of_affixes(); i++){
           CSuffix* pSuffix = other->get_suffix_list()->at(i);
           if (! get_suffix_list()->contains (pSuffix)){
               return false;
           }
       }
   return true; }
   else {            // -->  prefixes <-- //
   for (int i  = 0; i < other->get_number_of_affixes(); i++){
           CPrefix* pPrefix = other->get_prefix_list()->at(i);
           if (! get_prefix_list()->contains (pPrefix)){
               return false;
           }
   }
   return true;
   }
}
bool CSignature::contains_affix_string(affix_t affix)
{
    QStringList affixes = m_Signature.split("=");
    if (affixes.contains(affix) ){
        return true;
    }
    return false;
}

void CSignature::intersection_with(affix_list &in, affix_list &intersection)
{
   intersection.clear();
   affix_list my_affixes = m_Signature.split("=");
   for (int affix_no = 0; affix_no < in.size(); affix_no++){
       affix_t affix = in.at(affix_no);
       if (my_affixes.contains(affix)) {
           intersection.append(affix);
       }
   }
   intersection.sort();
   return;
}



// =============================================>  Functions involving string containment <===========================//

void CSignature::add_stem_pointer(CStem* pStem)
{
    if (m_Stems->contains(pStem)){
        return;
    }
    m_Stems->append(pStem);
}

void CSignature::add_affix_ptr(CPrefix *pPrefix) {
    if (m_Prefixes->contains(pPrefix)){
        return;
    } else{
        m_Prefixes->append(pPrefix);
    }
    return;
}
void CSignature::add_affix_ptr(CSuffix *pSuffix) {
    if (m_Suffixes->contains(pSuffix)){
        return;
    } else{
        m_Suffixes->append(pSuffix);
    }
    return;
}

bool compare_stems_of_sig(const CStem* pStem1, const CStem* pStem2)
{
 return  pStem1->get_key() < pStem2->get_key();
}
bool compare_stems_of_sig_by_count(const CStem* pStem1, const CStem* pStem2)
{
 return  pStem1->get_count() >pStem2->get_count();
}
void CSignature::sort_stems(){
    qSort(m_Stems->begin(), m_Stems->end(), compare_stems_of_sig);
}
void CSignature::sort_stems_by_count(){
    qSort(m_Stems->begin(), m_Stems->end(), compare_stems_of_sig_by_count);
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
    robustness = stem_letters * (get_number_of_affixes()-1)  +  suffix_letters * (get_number_of_stems()-1);
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


word_and_count_list * CSignature::get_word_and_count_vectors(word_and_count_list * this_vector)
// this produces a list of word_and_count's: each is a word and its corpus count, one for each affix in the signature.
{   QString this_word;
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

double CSignature::get_stem_entropy()
{
    if (m_stem_entropy >= 0){
        return m_stem_entropy;
    }
    m_stem_entropy = calculate_stem_entropy();
    return m_stem_entropy;
}

double log_base_2(double x)
{
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
        total_count++;
    }
    if (counts.size() == 1){
        m_stem_entropy = 0.0;
        return 0.0;
    }
    QMapIterator<QChar,double> this_iter (counts);
    while (this_iter.hasNext()){
        letter = this_iter.next().key();
        double freq = counts[letter]/total_count;
        entropy += -1.0 * freq * log_base_2 (freq);
    }
    m_stem_entropy = entropy;
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
    for (int i = 0; i < affixes.count(); i++){
        affix = affixes[i];
        if (! new_affixes.contains(affix)){
            new_sig.append(affix);
        }
    }
    new_sig.append(morph);
    new_sig.sort();
    new_signature = new_sig.join("=");
    return new_signature;
}

