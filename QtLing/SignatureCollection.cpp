#include "SignatureCollection.h"
#include <QDebug>


CSignatureCollection::CSignatureCollection()
{
    //m_SignatureList         = QList<CSignature*>();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
}

QMapIterator<QString, CSignature*> * CSignatureCollection::get_map_iterator()
{
    map_sigstring_to_sigptr_iter * sig_iter = new map_sigstring_to_sigptr_iter (m_SignatureMap);
    return sig_iter ;
}
QListIterator<CSignature*> * CSignatureCollection::get_sorted_list_iterator()
{
    QListIterator<CSignature*>  * sig_iter = new QListIterator<CSignature*> (m_SortList);
    return sig_iter ;
}

CSignatureCollection::~CSignatureCollection()
{

}

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
CSignature* CSignatureCollection::find_or_add (QString szSignature)
{    if (! m_SignatureMap.contains(szSignature) ) {
    CSignature* pSig = new CSignature(szSignature);
    return pSig;
    }
    return m_SignatureMap.value(szSignature);
 
}

bool compare_stem_count(const CSignature* pSig1, const CSignature* pSig2)
{
 return  pSig1->get_number_of_stems() > pSig2->get_number_of_stems();
}
void CSignatureCollection::sort()
{   qDebug() << "sorting signatures.";
    //QMap<QString,CSignature*>::iterator sig_iter;

    map_sigstring_to_sigptr_iter sig_iter (m_SignatureMap);
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
{
    map_sigstring_to_sigptr_iter sig_iter (m_SignatureMap);
    while (sig_iter.hasNext())
    {
        sig_iter.next();
        m_SortList.append(sig_iter.value());
    }
    qSort(m_SortList.begin(), m_SortList.end(),  compare_affix_count);
}
