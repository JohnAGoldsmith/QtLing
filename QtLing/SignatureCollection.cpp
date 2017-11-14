#include "SignatureCollection.h"
#include <QDebug>
#include "Typedefs.h"

CSignatureCollection::CSignatureCollection()
{
    //m_SignatureList         = QList<CSignature*>();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
    m_MapIterator           = new map_sigstring_to_sig_ptr_iter (m_SignatureMap);
    m_SortedListIterator    = new     QListIterator<CSignature*> (m_SortList);
}

CSignatureCollection::~CSignatureCollection()
{
    delete m_MapIterator;
    delete m_SortedListIterator;
}

void CSignatureCollection::clear(){
    m_SignatureMap.clear();
    m_SortList.clear();

}
map_sigstring_to_sig_ptr_iter * CSignatureCollection::get_map_iterator()
{   qDebug() << "In signature collection, getting the map iterator.";
    m_MapIterator->toFront();
    return m_MapIterator ;
}
QListIterator<CSignature*> * CSignatureCollection::get_sorted_list_iterator()
{

    m_SortedListIterator->toFront();
    return m_SortedListIterator;
}


// -->   Accssing  <--     //



bool CSignatureCollection::contains(sigstring_t this_sigstring){
    return m_SignatureMap.contains(this_sigstring);
}
CSignature* CSignatureCollection::operator <<(QString szSignature)
{
    CSignature* pSig = new CSignature(szSignature);
    m_SignatureMap.insert(szSignature, pSig);
    return pSig;
}

void CSignatureCollection::operator<< (CSignature * pSig)
{
    m_SignatureMap.insert(pSig->display(), pSig);
}

CSignature* CSignatureCollection::operator ^=(QString szSignature)
{
    return this->find_or_add(szSignature);
}
CSignature* CSignatureCollection::find_or_add (QString sigstring )
{   if (m_SignatureMap.contains(sigstring)){
        return m_SignatureMap[sigstring];
    }
    CSignature* pSig = new CSignature(sigstring);
    m_SignatureMap[sigstring] = pSig;
    return pSig;

}

// -->   Sorting  <--     //

bool compare_stem_count(const CSignature* pSig1, const CSignature* pSig2)
{
 return  pSig1->get_number_of_stems() > pSig2->get_number_of_stems();
}
void CSignatureCollection::sort()
{
    m_SortList.clear();
    map_sigstring_to_sig_ptr_iter sig_iter (m_SignatureMap);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        m_SortList.append(sig_iter.value());
    }
   qSort(m_SortList.begin(), m_SortList.end(),  compare_stem_count);
}

// ------->                                   <---------------------//
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


void CSignatureCollection::compute_containment_list()
{   CSignature* pSig, *qSig;
    sort_signatures_by_affix_count();
    for (int i = 0; i < m_SortList.size(); i++){
        pSig = m_SortList[i];
        if (pSig->get_number_of_affixes() < 3) {break;}
        m_ContainmentMap[pSig] = new QList<CSignature*>;
        for (int j = i+1; j < m_SortList.size(); j++){
            qSig = m_SortList[j];
            if (qSig->get_number_of_affixes() < 2) {break;}
            if (pSig->contains(qSig)){
                m_ContainmentMap[pSig]->append(qSig);
                //qDebug() << pSig->get_key() << qSig->get_key();
            }
        }
    }
    // Sort each row of signatures by stem-count or robustness.

}
