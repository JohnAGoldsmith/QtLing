#include "Stem.h"
#include "Signature.h"

CStem::CStem(QString stem) : CParse(stem), m_key(stem), m_Signatures() {}

CStem::CStem(CStem& stem) {
    m_key = stem.get_key();
    QListIterator<QString> sig_iter(*stem.GetSignatures());
    while (sig_iter.hasNext()){
        m_Signatures.append(sig_iter.next());
    }

}



QString CStem::display(){
    return m_key;
}
