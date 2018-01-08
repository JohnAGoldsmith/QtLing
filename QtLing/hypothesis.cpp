#include <QDebug>
#include "hypothesis.h"
#include "Lexicon.h"

CHypothesis* CLexicon::get_hypothesis(QString hypothesis)
{
    return m_Hypothesis_map->value( hypothesis );
}
void CLexicon::generate_hypotheses()
{   sig_graph_edge * p_edge;
    lxa_sig_graph_edge_map_iter edge_iter (m_SigGraphEdgeMap);
    QString affix, new_affix;
    QStringList affixes1, affixes2, new_affixes, new_sig_2;
    int MINIMUM_AFFIX_OVERLAP = 10;
    //qDebug() << "entering generate hypotheses";
    while    (edge_iter.hasNext()){
        p_edge = edge_iter.next().value();
        QString this_morph = p_edge->get_morph();
        CSignature* pSig1 = p_edge->get_sig_1();
        CSignature* pSig2 = p_edge->get_sig_2();


        if (this_morph.length() < 2){
            continue;
        }
        //qDebug() << this_morph << 19;

        if (pSig1->get_stem_entropy() < m_entropy_threshold_for_stems ||
            pSig2->get_stem_entropy() < m_entropy_threshold_for_stems  ){
            //qDebug() << pSig1->get_key() << pSig1->get_stem_entropy() << pSig2->get_key() << pSig2->get_stem_entropy();
            continue;
        }

        affixes1.clear();
        affixes2.clear();
        new_sig_2.clear();
        new_affixes.clear();
        pSig1->get_string_list(affixes1);
        pSig2->get_string_list(affixes2);
       // qDebug() << "number of words" << p_edge->get_number_of_words() << 37;
        if (p_edge->get_number_of_words() < 6 ){continue;}

        //qDebug() << p_edge->get_number_of_words() << 34;
        //--> new_affixes is the set of affixes that sig1 proposes to sig2  for retirement
        for (int affixno = 0; affixno < affixes1.count(); affixno++){
            affix = affixes1[affixno];
            if (affix == "NULL"){
                new_affix = this_morph;
            } else{
                new_affix = this_morph + affix;
            }
            new_affixes.append(new_affix);
        }
        for (int affixno = 0; affixno < affixes2.count(); affixno++){
            affix = affixes2[affixno];
            if (!new_affixes.contains(affix)){
                new_sig_2.append(affix);
            }
        }
        new_sig_2.append(this_morph);
        new_sig_2.sort();

        // we remove all of the newaffixes from pSig2, and replace them with this_morph, and
        // this_morph points directly to pSig1.
        // Then pSig2 loses all of the stems that created the shared words here.
        //newaffixes2 = replace(affixes2, newaffixes2);

        eHypothesisType this_hypothesis_type = HT_affix_goes_to_signature;
        CHypothesis (this_hypothesis_type, p_edge);
        CHypothesis * this_hypothesis = new CHypothesis( this_hypothesis_type, this_morph,
                                                         pSig1, pSig2, new_sig_2.join("="), new_affixes,
                                                         p_edge->get_number_of_words());
        m_Hypotheses->append(this_hypothesis);
        m_Hypothesis_map->insert (this_hypothesis->express_as_string(),  this_hypothesis);


    }
  //  qDebug() << "hypothesis count " <<m_Hypotheses->count() << 72;

}
CHypothesis::CHypothesis(eHypothesisType HypothesisT,   sig_graph_edge*  p_edge)
{
    m_hypothesis_type  = HypothesisT;
    m_number_of_words_saved = 0;
    m_signature_1       = p_edge->get_sig_1()->get_key();
    m_signature_2       = p_edge->get_sig_2()->get_key();
    m_morpheme          = p_edge->get_morph();

    affix_t             new_affix, affix;
    QStringList         affixes1;
    QStringList         affixes2;
    QStringList         new_affixes;
    QStringList         new_sig_2;
    p_edge->get_sig_1()->get_string_list(affixes1);
    p_edge->get_sig_2()->get_string_list(affixes2);
    m_new_edge          = new QPair<affix_t, sigstring_t>(new_affix,m_signature_1);


    //--> new_affixes is the set of affixes that sig1 proposes to sig2  for retirement
    for (int affixno = 0; affixno < affixes1.count(); affixno++){
        affix = affixes1[affixno];
        if (affix == "NULL"){
            new_affix = m_morpheme;
        } else{
            new_affix = m_morpheme + affix;
        }
        new_affixes.append(new_affix);
    }
    for (int affixno = 0; affixno < affixes2.count(); affixno++){
        affix = affixes2[affixno];
        if (!new_affixes.contains(affix)){
            new_sig_2.append(affix);
        }
    }
    new_sig_2.append(m_morpheme);
    new_sig_2.sort();


}


CHypothesis::CHypothesis (eHypothesisType HypothesisT, morph_t this_morph,
                          CSignature* pSig1, CSignature* pSig2, sigstring_t new_sig,
                          QStringList new_affixes, int number_of_words_saved)
{
    if (HypothesisT == HT_affix_goes_to_signature){
        m_hypothesis_type   == HypothesisT;
        m_number_of_words_saved = 0;
        m_signature_1       = pSig1->get_key();
        m_signature_2       = pSig2->get_key();
        m_new_signature_2   = new_sig;
        m_morpheme          = this_morph;
        m_new_edge          = new QPair<QString, sigstring_t>(m_morpheme, m_signature_1);
        m_number_of_words_saved = number_of_words_saved;
        //m_number_of_stems   =
    }
}

QStringList CHypothesis::express(){
    QStringList output;
    output <<m_morpheme << m_signature_1 << m_signature_2 << m_new_signature_2
          <<  m_morpheme << "--> " << m_signature_1<< QString::number( get_number_of_words_saved());
    return output;
}

QString CHypothesis::express_as_string(){
    return express().join("@");
}
bool compare_number_of_words(CHypothesis* hyp1, CHypothesis* hyp2){
    return hyp1->get_number_of_words_saved()< hyp2->get_number_of_words_saved();
}
void sort_hypotheses(QStringList hypotheses)
{
 //   std::sort(hypotheses.begin(), hypotheses.end(),compare_number_of_words);
}
int CHypothesis::get_number_of_words_saved()
{
    return m_number_of_words_saved;
}
QString CHypothesis::get_key(){
    return m_morpheme + "@" + m_signature_1;
}
