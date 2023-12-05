#include <QList>
#include <QMap>
#include <QDebug>
#include <QMapIterator>
#include "mainwindow.h"
#include "Lexicon.h"
#include "Stem.h"
#include "StemCollection.h"
#include "WordCollection.h"


CLexicon* CLexicon::build_sublexicon(MainWindow* my_window)
{
    CSignature* pSig;

    CLexicon*  sublexicon = new CLexicon(this, get_suffix_flag());
    my_window->set_lexicon(sublexicon);
    set_window(my_window);

    sublexicon->set_progress_bar(my_window->m_ProgressBar);
    sublexicon->set_status_bar(my_window->statusBar());
    
    if (m_suffix_flag == false &&  m_PrefixSignatures->get_count() < 1) {
        return NULL;
    }
    if (m_suffix_flag == true &&  m_Signatures->get_count() < 1) {
        return NULL;
    }

    // Get a map of the full signatures (non-hollow), and then take only the stems
    // associated with full signatures.
    QMap<CSignature*,int> full_signatures;
    if (m_suffix_flag){
        QMapIterator<sigstring_t, CSignature*> sig_iter (*m_Signatures->get_map());
        while (sig_iter.hasNext()){
            pSig = sig_iter.next().value();
            if (pSig->get_stem_entropy() > get_entropy_threshold_for_positive_signatures()   ){
                full_signatures[pSig] = 1;
            }
        }
        QMapIterator<QString, CStem*> stem_iter (*m_suffixal_stems->get_stem_map());
        while (stem_iter.hasNext()){
            CStem* pStem = stem_iter.next().value();
            CSignature* pSig = pStem->get_last_signature();
            if (pSig->get_stem_entropy() < m_entropy_threshold_for_stems){
                continue;
            }
            stem_t  stem = stem_iter.key();
            sublexicon->add_word(stem);
        }
    }else{   // prefixal signatures
        QMapIterator<sigstring_t, CSignature*> sig_iter (*m_PrefixSignatures->get_map());
        while (sig_iter.hasNext()){
            pSig = sig_iter.next().value();
            if (pSig->get_stem_entropy() > get_entropy_threshold_for_positive_signatures()   ){
                full_signatures[pSig] = 1;
            }
        }
        QMapIterator<QString, CStem*> stem_iter (*m_prefixal_stems->get_stem_map());
        while (stem_iter.hasNext()){
            CStem* pStem = stem_iter.next().value();
            CSignature* pSig = pStem->get_last_signature();
            if (pSig->get_stem_entropy() < m_entropy_threshold_for_stems){
                continue;
            }
            stem_t  stem = stem_iter.key();
            sublexicon->add_word(stem);
        }
    }
    sublexicon->get_word_collection()->sort_word_lists(); 

    return sublexicon;
}
