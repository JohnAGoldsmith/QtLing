#ifndef HYPOTHESIS_H
#define HYPOTHESIS_H

#include "QString"
#include "generaldefinitions.h"
#include "Lexicon.h"
#include "graphics.h"


class CHypothesis
{
    friend class lxa_graphics_scene;

    QString                         m_key;
    eHypothesisType                 m_hypothesis_type;
    sigstring_t                     m_signature_1_longer_stem;
    sigstring_t                     m_signature_2_shorter_stem;
    sigstring_t                     m_new_signature_2;
    affix_t                         m_morpheme;
    QPair<affix_t, sigstring_t>*    m_new_edge; // an affix plus the signature it points to.
    double                          m_minimum_saving;
    double                          m_maximum_saving;
    int                             m_number_of_words_saved;

public:
    CHypothesis (eHypothesisType  );
    CHypothesis (eHypothesisType, sig_graph_edge * );
    CHypothesis (eHypothesisType,
                 const morph_t& this_morph,
                 const sigstring_t& sigstring_1,
                 const sigstring_t& sigstring_2,
                 const sigstring_t& new_sig,
                 const QStringList& new_affixes,
                 const int number_of_words_saved);
    QString                         get_key();
    QStringList                     express();
    QString                         express_as_string();
    int                             get_number_of_words_saved();



};
#endif // HYPOTHESIS_H
