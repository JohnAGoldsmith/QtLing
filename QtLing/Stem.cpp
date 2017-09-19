#include "Stem.h"
#include "Signature.h"

CStem::CStem(CStringSurrogate ssWord) : CParse(ssWord), m_key(ssWord.GetKey()), m_Signatures() {}

CStem::CStem(CStem& stem) {
    m_key = stem.GetStem();
    QListIterator<QString> sig_iter(*stem.GetSignatures());
    while (sig_iter.hasNext()){
        m_Signatures.append(sig_iter.next());
    }

}

CStem::CStem(QString stem){
    m_key = stem;
}

QString CStem::display(){
    return m_key;
}
