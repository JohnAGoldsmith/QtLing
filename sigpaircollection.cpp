#include "sigpaircollection.h"
#include "Lexicon.h"
#include "WordCollection.h"

SigPairCollection::SigPairCollection() {


}
void SigPairCollection::add(sig_pair* sigpair){
    if (m_SigPairMap.contains(sigpair->label())){
        sig_pair* my_sigpair = m_SigPairMap[sigpair->label()];
        foreach (QString stem, sigpair->m_stems){
            my_sigpair->add_stem(stem);
        }
        return;
    }
    sig_pair*  my_sigpair = new sig_pair(sigpair);
    m_SigPairMap[my_sigpair->label()] = my_sigpair;
    return;
}

void CLexicon::find_all_suffixal_sigpairs(){
    foreach (CWord* word, *m_Words->get_word_list()){
        for (int n = 0; n < word->get_suffixal_sigpairs()->length(); n++){
            sig_pair* sigpair;
            sigpair = word->get_suffixal_sigpairs()->at(n);
            sigpair->m_sig_1 = m_Signatures->find_or_fail(sigpair->get_sig1_string());
            sigpair->m_sig_2 = m_Signatures->find_or_fail(sigpair->get_sig2_string());
            sigpair->m_sig_2_entropy = sigpair->m_sig_2->get_stem_entropy();
            m_suffixal_sig_pairs->add( sigpair );
            if (!sigpair->m_sig_1 || ! sigpair->m_sig_2){
                qDebug() << 31 << "Sig did not exist" << sigpair->m_sig_1->display() << sigpair->m_sig_2->display();
            }
        }
    }
}
