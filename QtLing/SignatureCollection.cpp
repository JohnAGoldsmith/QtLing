#include "SignatureCollection.h"
#include <QDebug>
#include "Typedefs.h"
#include "Lexicon.h"

CSignatureCollection::CSignatureCollection(CLexicon* p_lexicon, bool suffix_flag)
{
    //m_SignatureList         = QList<CSignature*>();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
    m_SortedListIterator    = new     QListIterator<CSignature*> (m_SortList);
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
    m_SignatureMap.clear();
    m_SortList.clear();
}

QListIterator<CSignature*> * CSignatureCollection::get_sorted_list_iterator()
{

    m_SortedListIterator->toFront();
    return m_SortedListIterator;
}


// -->   Accessing  <--     //


bool CSignatureCollection::contains(sigstring_t this_sigstring){
    return m_SignatureMap.contains(this_sigstring);
}
CSignature* CSignatureCollection::operator <<(QString this_sigstring)
{
    CSignature* pSig;

    Q_ASSERT(this_sigstring.length() > 0);
    if (! m_SignatureMap.contains(this_sigstring)){
        pSig = new CSignature(this_sigstring);
        m_signature_list.append(pSig);
        m_SignatureMap.insert(this_sigstring, pSig);
        m_suffix_flag?
            pSig->set_suffix_flag(true):
            pSig->set_suffix_flag(false);
    } else
    {
        pSig = m_SignatureMap[this_sigstring];
    }

    return pSig;
}

void CSignatureCollection::operator<< (CSignature * pSig)
{
    if (! m_SignatureMap.contains(pSig->get_key())){
        m_signature_list.append(pSig);
        m_SignatureMap.insert(pSig->get_key(), pSig);
        m_suffix_flag?
            pSig->set_suffix_flag(true):
            pSig->set_suffix_flag(false);
    }
}

CSignature* CSignatureCollection::operator ^=(QString szSignature)
{
    return this->find_or_add(szSignature);
}
CSignature* CSignatureCollection::find_or_add (QString sigstring )
{   if (m_SignatureMap.contains(sigstring))
    {
        return m_SignatureMap[sigstring];
    } else
    {
        CSignature* pSig = new CSignature(sigstring);
        m_SignatureMap[sigstring] = pSig;
        m_signature_list.append(pSig);
        return pSig;
    }
}
CSignature* CSignatureCollection::find_or_fail(QString this_sig_string){

    if (m_SignatureMap.contains(this_sig_string)){
        return m_SignatureMap[this_sig_string];
    } else{
        return NULL;
    }
}


// -->   Sorting  <--     //

bool compare_stem_count(const CSignature* pSig1, const CSignature* pSig2)
{ return  pSig1->get_number_of_stems() > pSig2->get_number_of_stems();}

bool compare_affix_count(const CSignature* pSig1, const CSignature* pSig2)
{   if (pSig1->get_number_of_affixes() == pSig2->get_number_of_affixes())
     {
        return pSig1->get_key() < pSig2->get_key();
     }
     return pSig1->get_number_of_affixes() > pSig2->get_number_of_affixes();
}


void CSignatureCollection::sort(eSortStyle sort_style)
{
    m_SortList.clear();
    map_sigstring_to_sig_ptr_iter sig_iter (m_SignatureMap);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        m_SortList.append(sig_iter.value());
    }
    if (sort_style == SIG_BY_AFFIX_COUNT) {
          qSort(m_SortList.begin(), m_SortList.end(),  compare_affix_count);
    } else{
          qSort(m_SortList.begin(), m_SortList.end(),  compare_stem_count);
    }

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

void CSignatureCollection::compute_containment_list()
{   CSignature* pSig, *qSig;
    sort(SIG_BY_AFFIX_COUNT);
    for (int i = 0; i < m_SortList.size(); i++){
        pSig = m_SortList[i];
        if (pSig->get_number_of_affixes() < 3) {break;}
        m_ContainmentMap[pSig] = new QList<CSignature*>;
        for (int j = i+1; j < m_SortList.size(); j++){
            qSig = m_SortList[j];
            if (qSig->get_number_of_affixes() < 2) {break;}
            if (pSig->contains(qSig)){
                m_ContainmentMap[pSig]->append(qSig);
            }
        }
    }
    // Sort each row of signatures by stem-count or robustness.

}
void CSignatureCollection::sort_each_signatures_stems_alphabetically()
{
    map_sigstring_to_sig_ptr_iter sig_iter (m_SignatureMap);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        sig_iter.value()->sort_stems();
    }
}

QList<word_and_count_list*> *   CSignatureCollection::get_count_vectors(QList<word_and_count_list*> * count_vectors){
    count_vectors->clear();
    for (int signo = 0; signo<m_SortList.size(); signo++)
    {
        CSignature* pSig = m_SortList[signo];
        word_and_count_list * pVector = new word_and_count_list;
        count_vectors->append(pSig->get_word_and_count_vectors(pVector));
    }
    return count_vectors;
}
void CSignatureCollection::calculate_stem_entropy()
{
    map_sigstring_to_sig_ptr_iter sig_iter (m_SignatureMap);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        sig_iter.value()->calculate_stem_entropy();
    }
}

int CSignatureCollection::get_number_of_epositive_signatures()
{   int count = 0;
    map_sigstring_to_sig_ptr_iter sig_iter (m_SignatureMap);
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
    map_sigstring_to_sig_ptr_iter sig_iter (m_SignatureMap);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        if (sig_iter.value()->get_stem_entropy() > get_lexicon()->get_entropy_threshold_for_positive_signatures()){
            sig_map.insert( sig_iter.value(), 1);
        }
    }
}

//This function find a small subset of the signatures with the property that each signature in the collection
// (whose stem count is greater or equal to threshold) is a subsignature of one of the minimal covering signatures.

void CSignatureCollection::find_minimal_cover()
{
    int STEM_THRESHOLD = 5;

    sort(SIG_BY_AFFIX_COUNT);

    QList<CSignature*> temporary_sig_list;
    QList<CSignature*> minimal_sig_cover;
    QListIterator<CSignature*> sig_iter_1(m_SortList);

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
