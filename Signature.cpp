#include "Signature.h"
#include "SignatureCollection.h"
#include "Suffix.h"
#include <QDebug>
#include <QChar>
#include <QMap>
#include <QtMath>
#include <algorithm>
#include "Lexicon.h"
#include "WordCollection.h"
#include "Word.h"


extern QString QStringList2QString(QString);


CSignature::CSignature(QString signature_string, bool suffix_flag, QObject * parent): QObject(parent)
{
  m_Signature = signature_string;
  m_SuffixFlag = suffix_flag;
  /*
  m_Stems = new CStem_ptr_list();
  m_Suffixes = new CSuffix_ptr_list();
  m_Prefixes = new CPrefix_ptr_list();
  m_SuffixFlag = suffix_flag;
  */

  // oct 2021 removed the next line, made it the parent with qobject:
  //m_SignatureCollection = Signatures;
  m_stem_entropy = -1;
  m_robustness = 0;
  m_secondary_stem_count = 0;
}

CSignature::CSignature(CSignature& signature) {
    m_Signature = signature.get_key();
    if (signature.get_prefix_list()->count() > 0){
        foreach (CPrefix* pPrefix, *signature.get_prefix_list()){
            m_Prefixes.append(pPrefix);
        }
    }
    if (signature.get_suffix_list()->count() > 0){
        foreach (CSuffix* pSuffix, *signature.get_suffix_list()){
            m_Suffixes.append(pSuffix);
        }
    }
    if (signature.get_stems()->count() > 0){
        foreach (CStem* pStem, *signature.get_stems()){
            m_Stems.append(pStem);
        }
    }
    m_SuffixFlag = signature.get_suffix_flag();
    m_stem_entropy = signature.get_stem_entropy();
    m_secondary_stem_count = signature.get_secondary_stem_count();
    m_robustness = signature.get_robustness();
}
CSignature::~CSignature()
{
 // nothing needs to be deleted.
};
QStringList CSignature::get_affix_string_list(){
    return m_Signature.split("=");
}


QStringList& CSignature::get_stem_strings(QStringList& stem_list)
{   stem_list.clear();
    for (int stemno = 0; stemno < m_Stems.size(); stemno++){
        stem_list.append(m_Stems[stemno]->get_key());
    }
    return stem_list;
}
int CSignature::get_letter_count_in_stems(){
    int count = 0;
    for (int stemno = 0; stemno < m_Stems.size(); stemno++){
        count += m_Stems[stemno]->get_key().length();
    }
    return count;
}
int CSignature::get_letter_count_in_affixes(){
    int count = 0;
    if (m_SuffixFlag){
        for (int suffixno = 0; suffixno < m_Suffixes.size(); suffixno++){
         count += m_Suffixes[suffixno]->get_key().length();
        }
    } else{
        for (int prefixno = 0; prefixno < m_Prefixes.size(); prefixno++){
         count += m_Prefixes[prefixno]->get_key().length();
        }
    }
    return count;
}
int CSignature::get_letter_count_in_stems_and_affixes(){
    return get_letter_count_in_stems() + get_letter_count_in_affixes();
}
// =============================================>  Functions involving string containment <===========================//
bool CSignature::contains(CSignature *other) {
    if (m_SuffixFlag){
       for (int i = 0; i < other->get_number_of_affixes(); i++){
           CSuffix* pSuffix = other->get_suffix_list()->at(i);
           if (! get_suffix_list()->contains (pSuffix)){
               return false;
           }
       }
       return true;
    } else {            // -->  prefixes <-- //
        for (int i = 0; i < other->get_number_of_affixes(); i++){
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

int CSignature::intersection_with(Affix_list &in, Affix_list &intersection)
{
   intersection.clear();
    Affix_list my_affixes = m_Signature.split("=");
   for (int affix_no = 0; affix_no < in.size(); affix_no++){
       affix_t affix = in.at(affix_no);
       if (my_affixes.contains(affix)) {
           intersection.append(affix);
       }
   }
   intersection.sort();
   return intersection.length();
}

int  CSignature::get_size_of_intersection(CSignature* othersig){
   int count = 0;
   QStringList my_affixes = m_Signature.split("=");
   QStringList other_affixes;
   other_affixes = othersig->get_affix_string_list();
   for (int affix_no = 0; affix_no < my_affixes.size(); affix_no ++ ){
       if (other_affixes.contains( my_affixes[affix_no] ) ){
               count++;
        }
   }
   return count;
}
int  CSignature::get_size_of_intersection(QList<CSuffix*>* other_affixes){
   int count = 0;  
   foreach(CSuffix* pSuffix, m_Suffixes){
       if (other_affixes->contains( pSuffix) ){
               count++;
        }
   }
   return count;
}

// =============================================>  Functions involving string containment <===========================//

void CSignature::add_affix_string(QString this_affix){
    QStringList affixes;
    if (m_Signature.length() > 0){
        affixes = m_Signature.split("=");
    }
    affixes.append(this_affix);
    affixes.sort();
    m_Signature = affixes.join("=");
    //if (affixes.count()==1) qDebug() << m_Signature << "only one element though";
}

void CSignature::add_stem_pointer(CStem* pStem)
{
    if (m_Stems.contains(pStem)){
        return;
    }
    m_Stems.append(pStem);
}

void CSignature::add_affix_ptr(CPrefix *pPrefix) {
    if (m_Prefixes.contains(pPrefix)){
        return;
    }
    m_Prefixes.append(pPrefix);
}
void CSignature::add_affix_ptr(CSuffix *pSuffix) {
    if (m_Suffixes.contains(pSuffix)){
        return;
    }
    m_Suffixes.append(pSuffix);
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
    //qSort(m_Stems->begin(), m_Stems->end(), compare_stems_of_sig);
     std::sort(m_Stems.begin(), m_Stems.end(), compare_stems_of_sig);
}
void CSignature::sort_stems_by_count(){
    //qSort(m_Stems->begin(), m_Stems->end(), compare_stems_of_sig_by_count);
    std::sort(m_Stems.begin(), m_Stems.end(), compare_stems_of_sig_by_count);
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
    for (int stem_no = 0; stem_no < m_Stems.size(); stem_no ++ ){
        outstring += m_Stems[stem_no]->display();
    }
    return outstring;
}


int CSignature::calculate_secondary_robustness(){
    int affix_letters = 0;
    if (m_SuffixFlag){
        foreach (CSuffix* p_suffix, m_Suffixes) {
            const QString& str_suffix = p_suffix->get_key();
            if (str_suffix == "NULL") {
                affix_letters += 1;
            } else {
                affix_letters += str_suffix.length();
            }
        }
    } else {
        foreach (CPrefix* p_prefix, m_Prefixes) {
            const QString& str_prefix = p_prefix->get_key();
            if (str_prefix == "NULL") {
                affix_letters += 1;
            } else {
                affix_letters += str_prefix.length();
            }
        }
    }
    qDebug() << 238 << "sig.cpp"<< display() << m_secondary_stem_count << affix_letters;
    return (m_secondary_stem_count-1) * affix_letters;
}

double CSignature::calculate_robustness()   {
    int stem_letters = 0;
    int affix_letters = 0;
    if (m_SuffixFlag){
        foreach (CSuffix* p_suffix, m_Suffixes) {
            const QString& str_suffix = p_suffix->get_key();
            if (str_suffix == "NULL") {
                affix_letters += 1;
            } else {
                affix_letters += str_suffix.length();
            }
        }
    } else {
        foreach (CPrefix* p_prefix, m_Prefixes) {
            const QString& str_prefix = p_prefix->get_key();
            if (str_prefix == "NULL") {
                affix_letters += 1;
            } else {
                affix_letters += str_prefix.length();
            }
        }
    }

    foreach (CStem* p_stem, m_Stems) {
        const QString& str_stem = p_stem->get_key();
        stem_letters += str_stem.length();
    }
    /*
    for (int stem_no=0;stem_no<m_Stems->size();stem_no++){
        stem_letters += m_Stems->at(stem_no)->get_key().length();
    }*/
    m_robustness = stem_letters * (get_number_of_affixes()-1)  +  affix_letters * (get_number_of_stems()-1);
}
int CSignature::get_robustness()   {
    if (m_robustness == 0)  {
        m_robustness = calculate_robustness();
    }
    return m_robustness;
}
 int CSignature::get_number_of_affixes() const
{    if (m_SuffixFlag){
         if (m_Suffixes.count() > 0)
         {return m_Suffixes.count();}
         else {return m_Signature.count("=") + 1; }
     }
     else{
         if (m_Prefixes.count() > 0)
         {return m_Prefixes.count();}
         else {return m_Signature.count("=") + 1; }
     }
}


word_and_count_list * CSignature::get_word_and_count_vectors(word_and_count_list * this_vector)
// this produces a list of word_and_count's: each is a word and its corpus count, one for each affix in the signature.
{   QString this_word;
    if (m_SuffixFlag){
        for (int stem_no = 0; stem_no < m_Stems.size(); stem_no++){
            stem_t this_stem = m_Stems[stem_no]->get_key();
            for (int suff_no = 0; suff_no < m_Suffixes.size();suff_no++){
                suffix_t this_suffix = m_Suffixes[suff_no]->get_key();
                if (this_suffix == "NULL"){
                    this_word = this_stem;
                } else{
                    this_word = this_stem + this_suffix;
                }
                CWord* pWord = get_signatures()->get_lexicon()->get_word_collection()->find_or_fail(this_word);
                if (pWord) {
                    word_and_count* pWord_and_count = new word_and_count(pWord->get_key(),pWord->count());
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
{   QMap<QChar,double> counts;
    double     total_count (0);
    CStem* pStem;
    QChar  letter;
    double entropy =0.0;
    foreach (pStem, m_Stems){
        stem_t this_stem = pStem->get_key();
        m_SuffixFlag?
            letter = this_stem.at(this_stem.length()-1):
            letter = this_stem.at(0);
        counts.contains(letter)?
            counts[letter]++:
            counts[letter] = 1;
        total_count++;
    }
    if (counts.size() == 1){
        m_stem_entropy = 0.0;
        return 0.0;
    }
    foreach (QChar letter, counts.keys()){
        double freq = counts[letter]/total_count;
        entropy += -1.0 * freq * log_base_2 (freq);
    }
    m_stem_entropy = entropy;
    return entropy;
}

void CSignature::remove_suffix(suffix_t this_suffix){
    QStringList old_sig = m_Signature.split("=");
    old_sig.removeOne(this_suffix);
    m_Signature = old_sig.join("=");
    foreach (CSuffix * psuffix, m_Suffixes){
        if (psuffix->get_key()==this_suffix){
            m_Suffixes.removeOne(psuffix);
            break;
        }
    }
}
void CSignature::remove_prefix(prefix_t this_prefix){
    QStringList old_sig = m_Signature.split("=");
    old_sig.removeOne(this_prefix);
    m_Signature = old_sig.join("=");
    foreach (CPrefix * pprefix, m_Prefixes){
        if (pprefix->get_key()==this_prefix){
            m_Prefixes.removeOne(pprefix);
            break;
        }
    }
}

QString CSignature::get_highfreq_edge_letters(float frequency_threshold){
    QMap<QString, int> counts;
    QString letter, winner;
    float totalcount =0.0, winner_count(0);
    foreach (CStem* stem, m_Stems){
        totalcount += 1.0;
        m_SuffixFlag?
            letter = stem->get_key().right(1):
            letter = stem->get_key().left(1);
        if ( counts.contains(letter)){
            counts[letter] += 1;
        } else{
            counts[letter] = 1;
        }
    }
    QMapIterator<QString, int> i(counts);
    while (i.hasNext()) {
        i.next();
        if (winner.length() == 0){
                winner = i.key();
                winner_count = i.value();
        } else{
            if (i.value() > winner_count){
                    winner = i.key();
                    winner_count = i.value();
            }
        }
    }
    if (winner_count / totalcount > frequency_threshold)
        return winner;
    else
        return QString();
}

void CSignature::increment_robustness(int increment) {
    m_robustness += increment;
}

void CSignature::compute_signature_transforms(bool suffix_flag, QTextStream & sig_transforms){
    for (int n=0; n<m_Stems.size();n++ )
    {   QString stem, word;
        if (suffix_flag){
            stem = m_Stems[n]->get_stem();
            for(int s = 0; s < get_suffix_list()->length(); s++){
                QString suffix = get_suffix_list()->at(s)->get_key();
                if (suffix == "NULL")
                    word = stem;
                else
                    word = stem + suffix;
                QString transform (m_Signature + "_" + suffix);
                sig_transforms << word + " " + transform << Qt::endl;

            }
        }
    }
}
QStringList CSignature::signature2latex_string_list(){
    QStringList result;
    result << get_key();
    result << QString::number(m_Stems.count());
    result << QString::number(m_stem_entropy);
    result << QString::number(m_robustness);
    return result;
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


