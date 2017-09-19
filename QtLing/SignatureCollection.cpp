#include "SignatureCollection.h"
#include <QDebug>
#include "Parse.h"
//#include "Signature.h"

CSignatureCollection::CSignatureCollection()
{
    //m_SignatureList         = QList<CSignature*>();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
}


CSignatureCollection::~CSignatureCollection()
{

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
    return this->find(szSignature);
}
CSignature* CSignatureCollection::find (QString szSignature)
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
    QMap<QString,CSignature*>::iterator sig_iter;
    foreach (CSignature* pSig, m_SignatureMap){
        m_SortList.append(pSig);
        //qDebug() << pSig->GetSignature();
    }
    qSort(m_SortList.begin(), m_SortList.end(),  compare_stem_count);
}
