#include <QList>
#include <QMap>
#include <QMapIterator>
#include "Lexicon.h"
#include "Stem.h"
#include "StemCollection.h"
#include "WordCollection.h"


CLexicon* CLexicon::build_sublexicon()
{
    if (m_SuffixesFlag == false &&  m_PrefixSignatures->get_count() < 1) {
        return NULL;
    }
    if (m_SuffixesFlag == true &&  m_Signatures->get_count() < 1) {
        return NULL;
    }
    CLexicon sublexicon(this);
    sublexicon.set_progress_bar(m_ProgressBar);
    sublexicon.set_status_bar(m_StatusBar);

    if (m_SuffixesFlag){
        QMapIterator<QString, CStem*> stem_iter (*m_suffixal_stems->get_stem_map());
        while (stem_iter.hasNext()){
            CStem* pStem = stem_iter.next().value();
            stem_t  stem = stem_iter.key();
            sublexicon.get_words()->add(stem);
        }
    }
    sublexicon.Crab_1();


    return this;
}
