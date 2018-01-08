#ifndef HYPOTHESIS_H
#define HYPOTHESIS_H

#include "QString"
#include "generaldefinitions.h"
#include "Lexicon.h"
#include "graphics.h"


class CHypothesis
{   friend lxa_graphics_scene;

    QString                         m_key;
    eHypothesisType                 m_hypothesis_type;
    sigstring_t                     m_signature_1;
    sigstring_t                     m_signature_2;
    sigstring_t                     m_new_signature_2;
    affix_t                         m_morpheme;
    QPair<affix_t, sigstring_t>*    m_new_edge; // an affix plus the signature it points to.
    double                          m_minimum_saving;
    double                          m_maximum_saving;
    int                             m_number_of_words_saved;

public:
    CHypothesis (eHypothesisType  );
    CHypothesis (eHypothesisType, sig_graph_edge * );
    CHypothesis (eHypothesisType, morph_t this_morph, CSignature* pSig1, CSignature* pSig2,
                 sigstring_t new_sig, QStringList new_affixes, int number_of_words_saved);
    QString                         get_key();
    QStringList                     express();
    QString                         express_as_string();
    int                             get_number_of_words_saved();

};
#endif // HYPOTHESIS_H
