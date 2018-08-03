#include "Stem.h"
#include "Signature.h"

CStem::CStem(QString stem) :  m_key(stem), m_Signatures(),m_count(0) {}

CStem::CStem(CStem& stem) {
    m_key = stem.get_key();
    m_count = stem.get_count();
    QListIterator<CSignature*> sig_iter(*stem.GetSignatures());
    while (sig_iter.hasNext()){
        m_Signatures.append(sig_iter.next());
    }

}

void CStem::add_signature(CSignature *p_new_sig)
{
    foreach (CSignature* p_sig, m_Signatures) {
        if (p_sig->get_key() == p_new_sig->get_key())
            return;
    }
    m_Signatures.append(p_new_sig);
}

QString CStem::display(){
    return m_key;
}
