#include "Lexicon.h"
#include "Stem.h"
#include "Suffix.h"
#include "Signature.h"


CStem::CStem(QString stem) :  m_key(stem), m_Signatures(),m_count(0) {
    m_count = 0;
    m_json_id = 0;
    m_compound_component = 0;
    m_word_list_end_point = -1;
    m_word_list_start_point= -1;
}

CStem::CStem(CStem& stem) {
    m_key = stem.get_key();
    m_count = stem.get_count();
    QListIterator<CSignature*> sig_iter(*stem.GetSignatures());
    while (sig_iter.hasNext()){
        m_Signatures.append(sig_iter.next());
    }
    m_count = 0;
    m_json_id = 0;
}
CStem::~CStem(){

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
void CStem::make_compound(QString stem1, QString stem2){
    m_compound.first = stem1;
    m_compound.second = stem2;
}
bool CStem::is_compound(){
    if (m_compound.first.length() > 0 && m_compound.second.length()> 0) {
        return true;
    }
    return false;
}
