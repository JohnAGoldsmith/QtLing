#include "Lexicon.h"




/**
 * @brief CLexicon::Crab_4
 * Most of these functions are in the signature_graph.cpp file.
 */
void CLexicon::Crab_4()
{   m_StatusBar->showMessage("Crab 4: split complex morphemes...");

    replace_parse_pairs_from_current_signature_structure();

    step8a_compute_word_sig_pairs();

    step8c_from_sig_pairs_to_parses_Create_hypotheses();

    m_suffix_flag?
        m_Signatures->calculate_sig_robustness():
        m_PrefixSignatures->calculate_sig_robustness();
    test_json_functionality();

}


void CLexicon::replace_parse_pairs_from_current_signature_structure()
{
    //m_Raw_parses = new QMap<QString, CParse*> (m_ParseMap);
    m_ParseMap.clear();
    QList<CSignature*> *           these_signatures;
    m_suffix_flag?
        these_signatures = m_Signatures->get_signature_list():
        these_signatures = m_PrefixSignatures->get_signature_list();
    CParse*  this_parse;
    QStringList affix_string_list;
    foreach (CSignature* pSig, *these_signatures ){
        affix_string_list = pSig->get_affix_string_list();
        foreach (CStem* pStem, *pSig->get_stems()){
            const QString& this_stem = pStem->display();
            foreach (QString this_affix, affix_string_list){
                this_parse = new CParse(this_stem, this_affix, m_suffix_flag);
                add_parse(this_parse);
            }
        }
    }
}



/*!
 * Replace parse pairs from current signature structure. This allows us to
 * delete the old signature structure, and replace them with the set of
 * parse-pairs that exactly describe the current signature structure.
 */
/*
void CLexicon::replace_parse_pairs_from_current_signature_structure()
{
    m_Parses->clear();

    QList<CSignature*> *           these_signatures;
    m_SuffixesFlag?
            these_signatures = m_Signatures->get_signature_list():
            these_signatures = m_PrefixSignatures->get_signature_list();
    CSignature*                     pSig;
    foreach (pSig, *these_signatures ){
        const QStringList affixes = pSig->display().split("=");
        QList<CStem*>* stem_list = pSig->get_stems();
        CStem* pStem;
        foreach (pStem, *stem_list){
            const QString& this_stem = pStem->display();
            QString this_affix;
            foreach (this_affix, affixes){
                //if (this_affix == "NULL") this_affix = "";
                CParse*                         this_parse;
                m_SuffixesFlag?
                        this_parse = new CParse(this_stem, this_affix, true):
                        this_parse = new CParse(this_affix, this_stem, false);
                m_Parses->append(this_parse);

            }
        }

    }

   // while (sig_iter->hasNext()){
   //     pSig = sig_iter->next().value();
   //     affixes = pSig->display().split("=");
//
  //      stem_list =  pSig->get_stems();
  //      foreach (CStem* p_this_stem, *stem_list){
  //          foreach (this_affix, affixes){
  //              //if (this_affix == "NULL") this_affix = "";
  //              m_SuffixesFlag?
  //                      this_parse = new CParse(this_stem, this_affix, true):
  //                      this_parse = new CParse(this_affix, this_stem, false);
  //          }
  //      }
  //
}
 */


