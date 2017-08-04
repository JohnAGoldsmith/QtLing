#include "SignatureCollection.h"
#include <QDebug>
#include "Parse.h"
//#include "Signature.h"

CSignatureCollection::CSignatureCollection()
{
    m_SignatureList         = QList<CSignature*>();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    m_SortArray				= NULL;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
}

//CSignatureCollection::CSignatureCollection(CLexicon* Lex, QString MemberName)
//{
//    m_SignatureList              = QList<CSignature>();
//    m_CorpusCount			= 0;
//    m_MemberName			= QString::null;
//    m_SortArray				= NULL;
//    m_SortValidFlag			= 0;
//    m_SortStyle				= KEY;
//}
CSignatureCollection::~CSignatureCollection()
{
    if ( m_SortArray )         { delete [] m_SortArray; m_SortArray = NULL;  }
}
CSignature* CSignatureCollection::operator<<( CParse* pParse )
{
    CStringSurrogate  cssKey;
    cssKey = pParse->GetKey();

    CSignature* Signature = new CSignature(cssKey);
//    Signature->IncrementSignatureCount();
    m_SignatureList << Signature;
    m_SignatureMap[cssKey] = Signature;
    return Signature;
}
CSignature* CSignatureCollection::operator <<(CStringSurrogate SS)
{
    CSignature* Signature = new CSignature(SS);
//    Signature->IncrementSignatureCount();
    m_SignatureList << Signature;
    m_SignatureMap[SS] = Signature;
    return Signature;
}
CSignature* CSignatureCollection::operator <<(QString szSignature)
{
    const QChar* key = szSignature.constData();
    CStringSurrogate cssKey = CStringSurrogate(key, szSignature.length());
    CSignature* Signature = new CSignature(cssKey);
    m_SignatureList << Signature;
    m_SignatureMap[cssKey] = Signature;
    return Signature;
}
CSignature* CSignatureCollection::operator ^=(CParse* string)
{
    CStringSurrogate cssKey = string->GetKey();

    CSignature* pSignature = new CSignature(cssKey);
    int length = m_SignatureList.length();
    for (int i = 0 ; i < length; i++) {
        if (pSignature->GetSignature() == m_SignatureList.at(i)->GetSignature()) {
//            pSignature->IncrementSignatureCount();
            return pSignature;
        }
    }
    return 0;
}
CSignature* CSignatureCollection::operator ^=(CStringSurrogate string)
{
    CSignature* pSignature = new CSignature(string);
    int length = m_SignatureList.length();
    for (int i = 0 ; i < length; i++) {
        if (pSignature->GetSignature() == m_SignatureList.at(i)->GetSignature()) {
//            pSignature->IncrementSignatureCount();
            return pSignature;
        }
    }
    return 0;
}
CSignature* CSignatureCollection::operator ^=(QString szSignature)
{
    CStringSurrogate cssKey = CStringSurrogate(szSignature, szSignature.length());
    CSignature* pSignature = new CSignature(cssKey);

    int length = m_SignatureList.length();
    for (int i = 0 ; i < length; i++) {
        if (pSignature->GetSignature() == m_SignatureList.at(i)->GetSignature()) {
//            pSignature->IncrementSignatureCount();
            return pSignature;
        }
    }
    return 0;
}
CSignature* CSignatureCollection::GetAt( uint n )
{
    return m_SignatureList.at(n);
}
