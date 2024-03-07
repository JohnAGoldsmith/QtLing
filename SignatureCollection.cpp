#include "SignatureCollection.h"
#include <QDebug>
#include <QtMath>
#include <QFile>
#include "Typedefs.h"
#include "Lexicon.h"
#include "latex.h"

CSignatureCollection::CSignatureCollection(CLexicon* p_lexicon, bool suffix_flag)
{
    //m_SignatureList         = QList<CSignature*>();
    m_CorpusCount			= 0;
    m_MemberName			= QString();
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
    m_SortedListIterator    = new     QListIterator<CSignature*> (m_sort_list);
    m_suffix_flag           = suffix_flag;
    m_Lexicon               = p_lexicon;
    m_minimum_number_of_stems_for_minimal_cover = 5;
}

CSignatureCollection::~CSignatureCollection()
{
    foreach(CSignature* pSig, m_signature_list){
        delete pSig;
    }
}

void CSignatureCollection::clear(){
    foreach (CSignature * sig, m_signature_list){
        delete sig;
    }
    m_signature_list.clear();
    m_signature_map.clear();
    m_sort_list.clear();
    m_ContainmentMap.clear();
    m_minimal_cover.clear();
}

QListIterator<CSignature*> * CSignatureCollection::get_sorted_list_iterator()
{

    m_SortedListIterator->toFront();
    return m_SortedListIterator;
}


// -->   Accessing  <--     //


bool CSignatureCollection::contains(sigstring_t this_sigstring){
    return m_signature_map.contains(this_sigstring);
}
CSignature* CSignatureCollection::operator <<(const QString& this_sigstring)
{   return find_or_add(this_sigstring);
}
void CSignatureCollection::operator<< (CSignature * pSig)
{
    if (! m_signature_map.contains(pSig->get_key())){
        m_signature_list.append(pSig);
        m_signature_map.insert(pSig->get_key(), pSig);
        m_suffix_flag?
            pSig->set_suffix_flag(true):
            pSig->set_suffix_flag(false);
        pSig->setParent(this);
    }
}
CSignature* CSignatureCollection::operator ^=(const QString& szSignature)
{    return this->find_or_add(szSignature);
}
CSignature* CSignatureCollection::find_or_add (const QString& sigstring )
{   Q_ASSERT(sigstring.length() > 0);
    if (m_signature_map.contains(sigstring))
    {
        return m_signature_map[sigstring];
    }
    CSignature* pSig = new CSignature(sigstring, m_suffix_flag, this);
    m_signature_map[sigstring] = pSig;
    m_signature_list.append(pSig);
    return pSig;

}
CSignature* CSignatureCollection::find_or_fail(const QString& this_sig_string){

    if (m_signature_map.contains(this_sig_string)){
        return m_signature_map[this_sig_string];
    } else{
        return NULL;
    }
}

int CSignatureCollection::get_total_count_of_letters_in_all_signatures(){
    int letter_count(0);
    map_sigstring_to_sig_ptr_iter sig_iter (m_signature_map);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        letter_count += sig_iter.value()->get_letter_count_in_stems_and_affixes();
    }
    return letter_count;
}

double CSignatureCollection::get_description_length(){
    double DL = 0;
    double cost_of_letter = qLn(26) / qLn(2.0);
    double cost_of_end_marker = qLn(26) / qLn(2.0);
    double number_of_stems_and_affixes(0);

    DL = get_total_count_of_letters_in_all_signatures() * cost_of_letter;

    // add cost of marking the end of each stem and affix:
    map_sigstring_to_sig_ptr_iter sig_iter (m_signature_map);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        number_of_stems_and_affixes += sig_iter.value()->get_number_of_stems() +
                                        sig_iter.value()->get_number_of_affixes();
    }
    DL += number_of_stems_and_affixes * cost_of_end_marker;
    return DL;
}

// -->   Sorting  <--     //

bool compare_stem_count(const CSignature* pSig1, const CSignature* pSig2)
{ return  pSig1->get_number_of_stems() > pSig2->get_number_of_stems();}

bool compare_affix_count(  CSignature* pSig1,   CSignature* pSig2)
{   if (pSig1->get_number_of_affixes() == pSig2->get_number_of_affixes())
     {
        return pSig1->get_robustness() > pSig2->get_robustness();
     }
     return pSig1->get_number_of_affixes() > pSig2->get_number_of_affixes();
}
bool compare_robustness_reversed(  CSignature* pSig1,   CSignature* pSig2)
{
    return pSig1->get_robustness() >pSig2->get_robustness();
}
bool compare_secondary_robustness(CSignature* pSig1, CSignature* pSig2)
{
     qDebug() << 121 << pSig1->calculate_secondary_robustness() << pSig2->calculate_secondary_robustness();
     return pSig1->calculate_secondary_robustness() >pSig2->calculate_secondary_robustness();
}


void CSignatureCollection::sort(eSortStyle sort_style)
{
    m_sort_list.clear();
    map_sigstring_to_sig_ptr_iter sig_iter (m_signature_map);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        m_sort_list.append(sig_iter.value());
    }
    switch (sort_style){
    case SIG_BY_AFFIX_COUNT:
         std::sort(m_sort_list.begin(), m_sort_list.end(),  compare_affix_count);
         break;
    case SIG_BY_REVERSE_ROBUSTNESS:
        qDebug() << 176 << m_sort_list.length();
          std::sort(m_sort_list.begin(), m_sort_list.end(),  compare_robustness_reversed);
        qDebug() << 178;
          break;
    case SIG_BY_SECONDARY_ROBUSTNESS:
          std::sort(m_sort_list.begin(), m_sort_list.end(),  compare_secondary_robustness);
          break;
    case SIG_BY_AFFIX_COUNT_FOR_TREE:
        sort_signatures_by_subsets();
        break;
    default:
        std::sort(m_sort_list.begin(), m_sort_list.end(),  compare_stem_count);
    }
}
void CSignatureCollection::sort_signatures_by_subsets(){
    sort(SIG_BY_AFFIX_COUNT);
    int n = 0;
    while (n < m_sort_list.length()){
        QList<CSignature*> * sig_list;
        sig_list = new QList<CSignature*>;
        CSignature* top_sig = m_sort_list.at(0);
        sig_list->append(m_sort_list.takeAt(0));
        int m = n;
        while (m < m_sort_list.length()){
            if (top_sig->contains(m_sort_list.at(m))){
                sig_list->append(m_sort_list.takeAt(m));
                continue;
            }
            m++;
        }
        m_sort_list_by_subsets.append(sig_list);
    }
    sort(SIG_BY_REVERSE_ROBUSTNESS);





}
// ------->                                   <---------------------//
/*
bool compare_affix_count(const CSignature* pSig1, const CSignature* pSig2)
{
 return  pSig1->get_number_of_affixes() > pSig2->get_number_of_affixes();
}
void CSignatureCollection::sort_signatures_by_affix_count()
{   m_SortList.clear();
    map_sigstring_to_sig_ptr_iter sig_iter (m_SignatureMap);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        m_SortList.append(sig_iter.value());
    }
    qSort(m_SortList.begin(), m_SortList.end(),  compare_affix_count);
}
*/
bool compare_secondary_stem_count(CSignature* pSig1,CSignature* pSig2)
{
 return  pSig1->get_secondary_stem_count() < pSig2->get_secondary_stem_count();
}

// not currently used:
void CSignatureCollection::sort_signatures_by_secondary_stem_count(){
      m_sort_list.clear();
    map_sigstring_to_sig_ptr_iter sig_iter (m_signature_map);
       while (sig_iter.hasNext())
       {
           sig_iter.next();
           m_sort_list.append(sig_iter.value());
       }
       std::sort(m_sort_list.begin(), m_sort_list.end(),  compare_secondary_stem_count);

}

// I don´t think the containmentmap  is being used:
void CSignatureCollection::compute_containment_list()
{   CSignature* pSig, *qSig;
    sort(SIG_BY_AFFIX_COUNT);
    for (int i = 0; i < m_sort_list.size(); i++){
        pSig = m_sort_list[i];
        if (pSig->get_number_of_affixes() < 3) {break;}
        m_ContainmentMap[pSig] = new QList<CSignature*>;
        for (int j = i+1; j < m_sort_list.size(); j++){
            qSig = m_sort_list[j];
            if (qSig->get_number_of_affixes() < 2) {break;}
            if (pSig->contains(qSig)){
                m_ContainmentMap[pSig]->append(qSig);
            }
        }
    }
    // Sort each row of signatures by stem-count or robustness.

}

// not currently used:
void CSignatureCollection::sort_each_signatures_stems_alphabetically()
{
    map_sigstring_to_sig_ptr_iter sig_iter (m_signature_map);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        sig_iter.value()->sort_stems();
    }
}

QList<word_and_count_list*> *   CSignatureCollection::get_count_vectors(QList<word_and_count_list*> * count_vectors){
    count_vectors->clear();
    for (int signo = 0; signo<m_sort_list.size(); signo++)
    {
        CSignature* pSig = m_sort_list[signo];
        word_and_count_list * pVector = new word_and_count_list;
        count_vectors->append(pSig->get_word_and_count_vectors(pVector));
    }
    return count_vectors;
}
void CSignatureCollection::calculate_stem_entropy()
{
    map_sigstring_to_sig_ptr_iter sig_iter (m_signature_map);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        sig_iter.value()->calculate_stem_entropy();
    }
}

int CSignatureCollection::get_number_of_epositive_signatures()
{   int count = 0;
    map_sigstring_to_sig_ptr_iter sig_iter (m_signature_map);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        if (sig_iter.value()->get_stem_entropy() > get_lexicon()->get_entropy_threshold_for_positive_signatures()){
            count++;
        }
    }
    return count;
}

void CSignatureCollection::get_epositive_signatures(QMap<CSignature*, int> sig_map)
{
    map_sigstring_to_sig_ptr_iter sig_iter (m_signature_map);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        if (sig_iter.value()->get_stem_entropy() > get_lexicon()->get_entropy_threshold_for_positive_signatures()){
            sig_map.insert( sig_iter.value(), 1);
        }
    }
}

// this is not used, I think, and should be removed.
/*!
 * \brief CSignatureCollection::find_minimal_cover
 *
 * 1. Find all signatures that have more than 5 stems corresponding to each of
 *    them.
 * 2. Among these signatures, make a comparison between each two signatures.
 *    If a signature A "contains" signature B, signature B is removed from the
 *    list.
 */
void CSignatureCollection::find_minimal_cover()
{
    int STEM_THRESHOLD = 5;

    sort(SIG_BY_AFFIX_COUNT);
    // important!! Sort signatures by descending affix count

    QList<CSignature*> temporary_sig_list;
    QList<CSignature*> minimal_sig_cover;
    QListIterator<CSignature*> sig_iter_1(m_sort_list);

    //--> make a sorted copy of signatures with enough stems <---
    while (sig_iter_1.hasNext()){
        CSignature* pSig = sig_iter_1.next();
        if (pSig->get_number_of_stems() >= STEM_THRESHOLD){
            temporary_sig_list.append(pSig);
        }
    }


    CSignature* pSig;
    QMutableListIterator<CSignature*> sig_iter_2(temporary_sig_list);
    while (! temporary_sig_list.isEmpty()){
        pSig = temporary_sig_list.takeFirst();
        m_minimal_cover.append(pSig);
        //qDebug() << 251 << pSig->display();
        sig_iter_2.toFront();
        while(sig_iter_2.hasNext()){
            CSignature* qSig = sig_iter_2.next();
            if (pSig->contains(qSig)){
                    sig_iter_2.remove();
                    //qDebug() << 256 << pSig->display() << qSig->display();
            }
        }
    }
}

void CSignatureCollection::check_singleton_signatures(const QString &message)
{
    QMap<QString, CSignature*>::ConstIterator sig_map_iter;
    qDebug() << message << "Checking for singleton signatures";
    for (sig_map_iter = m_signature_map.constBegin();
         sig_map_iter != m_signature_map.constEnd();
         sig_map_iter++) {
        const QString& str_sig = sig_map_iter.key();
        if (!str_sig.contains('='))
            qDebug() << message << "found singleton signature in key:" << str_sig;
        CSignature* p_sig = sig_map_iter.value();
        if (!p_sig->get_key().contains('='))
            qDebug() << message << "found singleton signature in CSignature object:" << str_sig;
        if (p_sig->get_number_of_affixes() == 1)
            qDebug() << message << "found singleton signature in CSignature object:" << str_sig;
    }
}

void CSignatureCollection::calculate_sig_robustness()
{
    foreach (CSignature* p_sig, m_signature_map) {
        p_sig->calculate_robustness();
    }
    qDebug() << "Calculated signature robustness";
}

void CSignatureCollection::add_this_and_all_subsignatures(QString this_sig_string, int robustness, QStringList & signature_check_list){ // this is only used when creating "virtual signatures"
    if (signature_check_list.contains(this_sig_string)){
          return;
    }
    QStringList affixes = this_sig_string.split("=");
    if (affixes.length() == 1) {
        return;
    }
    signature_check_list.append(this_sig_string);
    CSignature * pSig = find_or_add(this_sig_string);
    pSig->increment_robustness(robustness);
    if (affixes.length() < 3){
        return;
    }
    for (int affix_no = 0; affix_no < affixes.length(); affix_no++){
        QStringList smaller_sig (affixes);
        smaller_sig.removeAt(affix_no);
        QString sig_string_2 = smaller_sig.join("=");
        add_this_and_all_subsignatures(sig_string_2, robustness, signature_check_list);
    }
}

void CSignatureCollection::compute_signature_transforms(bool suffix_flag, QTextStream & sig_transforms){
    map_sigstring_to_sig_ptr_iter sig_iter (m_signature_map);
    while (sig_iter.hasNext())
    {   sig_iter.next();
        sig_iter.value()->compute_signature_transforms(suffix_flag, sig_transforms);
    }
}
void CSignatureCollection::produce_latex(){

    QString file_name;
    file_name = "../../../../Dropbox/data/english/lxa/signatures.txt";
    QFile out_file (file_name);
    if (! out_file.open(QFile::WriteOnly | QFile::Text))
    {   qDebug() << "************  no file opened for writing output, file output.cpp";
        return;
    }
    QTextStream textstream (&out_file);
    textstream.setFieldAlignment(QTextStream::AlignRight);
    QMap<int, int> max_column_width;
    int number_of_columns = 5;
    for (int n = 0; n < number_of_columns; n++){ max_column_width[n] = 0;}
    QStringList result, temp_stringlist;
    double sum=0;
    QString alignments = "l l l l l";
    latex Latex;
    sort();
    result = Latex.tabular_beginning(alignments);
    foreach (CSignature* sig, m_sort_list){
        sum += sig->get_robustness();
        qDebug() << sum;
    }
    foreach (CSignature* sig, m_sort_list){
        temp_stringlist = sig->signature2latex_string_list();
        if (temp_stringlist[0].length() > max_column_width[0]) {
            max_column_width[0] = temp_stringlist[0].length();
        }
        if (temp_stringlist[1].length() > max_column_width[1]) {
            max_column_width[1] = temp_stringlist[1].length();
        }
        if (temp_stringlist[2].length() > max_column_width[2]) {
            max_column_width[2] = temp_stringlist[0].length();
        }
        if (temp_stringlist[3].length() > max_column_width[3]) {
            max_column_width[3] = temp_stringlist[3].length();
        }
        QString robustness_ratio;
        robustness_ratio = QString::number(sig->get_robustness()/sum);
        if (robustness_ratio.length() > max_column_width[4]) {
            max_column_width[4] = robustness_ratio.length();
        }
    }
    double running_robustness_sum (0.0);
    int n = 0;
    foreach (CSignature* sig, m_sort_list){
        textstream << QString::number(n++) << " & ";
        temp_stringlist = sig->signature2latex_string_list();
        for (int n = 0; n < 4; n++){
            textstream << temp_stringlist[n] + " & ";
        }
        QString robustness_ratio_string;
        double robustness_ratio;
        robustness_ratio =  sig->get_robustness()/sum;
        robustness_ratio_string = QString::number(robustness_ratio);
        textstream << robustness_ratio_string  << " & ";
        running_robustness_sum += robustness_ratio;
        textstream << QString::number(running_robustness_sum * 100.0);
        textstream << "\\\\";
        textstream << Qt::endl;
    }


    result.append(Latex.tabular_ending());
    out_file.close();

}
