#include "Stem.h"
#include "Signature.h"

CStem::CStem(CStringSurrogate ssWord) : CParse(ssWord), m_Word(ssWord.GetKey()), m_Signatures() {}

CStem::CStem(CStem& stem) {
    m_key = stem.GetStem();
    m_Signatures = stem.GetSignatures();
}

CStem::CStem(QString stem){
    m_key = stem;
}

QString CStem::display(){
    return m_key;
}
