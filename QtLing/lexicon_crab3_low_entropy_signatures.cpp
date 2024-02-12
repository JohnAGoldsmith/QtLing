#include <QApplication>
#include "Lexicon.h"
#include "WordCollection.h"

/**
 * @brief CLexicon::Crab_3
 *
 */
void CLexicon::Crab_3()
{
    find_all_signature_spines();

    //repair_low_entropy_signatures();

    m_suffix_flag?
        m_Signatures->calculate_sig_robustness():
        m_PrefixSignatures->calculate_sig_robustness();

    test_json_functionality();
}

void CLexicon::repair_low_entropy_signatures()
{
    /*  1. iterate through signatures.
        a. if the edge has zero-entropy, make a shorter stem, add the stem-affix pair to Parses, for all stems in sig.
        b. keep pairs of (new stem, whole word) and see if the set of new stems has zero-entropy. if so, repeat. If not, exit.
    2. Call AssignSuffixesToStems.
*/
    int progress_bar_count = 0;

    qApp->processEvents();
    CStemCollection * stems;
    CSignature * pSig;
    QString letter, affix, stem, stem2, this_morphemic_split;
    QStringList affix_list, stem_list;
    CSignatureCollection * signatures;
    m_suffix_flag ?
        stems = m_suffixal_stems:
        stems = m_prefixal_stems;
    signatures = get_active_signature_collection();
    initialize_progress(signatures->get_count());


    replace_parse_pairs_from_current_signature_structure();

    QMapIterator<sigstring_t,CSignature*> * sig_iter = new QMapIterator<sigstring_t,CSignature*> (* signatures->get_map() );
    while (sig_iter->hasNext()){
        if (++progress_bar_count % 1000 == 0){ mark_progress(progress_bar_count);}

        pSig = sig_iter->next().value();

        // if this sig has low entropy ( == is not left-definite ), then consider each stem which ends in that high-freq letter L.
        // For each stem, consider each word generated by each suffix of this signature.
        // For each word, is there another analysis with the stem one-letter shorter? (We can find that by looking at the parses, actually)
        // If so, drop the parse that comes from this signature.
        // Keep track of this erasure of (part of) this signature.
        // put message in the word's biography.

        if (pSig->get_stem_entropy() > m_entropy_threshold_for_stems ){
            continue;
        }
        // this has a problem: e.g. with abort: it shifts (wrongly) the split to the left of the t for abort and aborted, and then deletes the analyses of the other abort-stem words. TODO
        // the main reason it did badly was that there was only one stem in the signature! so the test for whether it was left-definite was no test at all.
        // otherwise, the signature has lots of great suffixes: NULL ed ion ive.
        float EdgeLetterPredominanceThreshold = 0.8;
        letter = pSig->get_highfreq_edge_letters(EdgeLetterPredominanceThreshold);
        foreach (stem, pSig->get_stem_strings(stem_list))
        { if (m_suffix_flag)
            {      stem2 = stem.left(stem.length()-1);
                foreach (QString affix, pSig->get_affix_string_list() )
                {
                    CParse parse (stem,affix,get_suffix_flag());
                    QString word = parse.display();
                    QString stem2 = stem.left(stem.length()-1);
                    CWord* pWord = get_words()->find_or_fail(word);
                    if (pWord->contains_this_stem_among_parses(stem2))
                    {
                        this_morphemic_split = parse.display_with_gap();
                        pWord->remove_morphemic_split(this_morphemic_split);
                        remove_parse(&parse);
                    }
                } //end of affixes in this sig
            } // end of suffix treatment
            else
            { // prefixes
                stem2 = stem.right(stem.length()-1);
                foreach (QString affix, pSig->get_affix_string_list() )
                {
                    CParse parse (stem,affix,get_suffix_flag());
                    QString word = parse.display();
                    QString stem2 = stem.right(stem.length()-1);
                    CWord* pWord = get_words()->find_or_fail(word);
                    if (pWord && pWord->contains_this_stem_among_parses(stem2))
                    {
                        this_morphemic_split = parse.display_with_gap();
                        pWord->remove_morphemic_split(this_morphemic_split);
                        remove_parse(&parse);
                    }
                } //end of affixes in this sig
            }
        } // end of stems in this sig;
    } // end of signatures loops
    
    
    step3_from_parses_to_stem2sig_maps(QString("Shift morpheme boundary leftward"));
    step4_create_signatures(QString("Shift morpheme boundary leftward"));


}

