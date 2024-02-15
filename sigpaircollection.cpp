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

void CLexicon::find_all_suffixal_sigpairs(){  // currently not used
    foreach (CWord* word, *m_Words->get_word_list()){
        qDebug() << 24 << word->display();
        qDebug() << 25 << word->get_suffixal_sigpairs();
        for (int n = 0; n < word->get_suffixal_sigpairs()->length(); n++){
            m_suffixal_sig_pairs->add( word->get_suffixal_sigpairs()->at(n) );
        }
    }
}
