#include "Lexicon.h"

#include "WordCollection.h"
#include "Word.h"



/*!
 * We can build a graph whose nodes are the signatures, where an edge connects
 * any pair of signatures if there exists a word that they both analyze.
 * A sig_graph_edge has two flavors: this function uses the flavor that
 * contains the two signatures, the word, and the string-difference between
 * the stems of the word at the two signatures.
 * The edge has: signature1, signature2, a morph, and a pair of stems.
 *
 * This is Part 1 of the 3rd major function of Crab 2.
 */
void CLexicon::compute_sig_graph_edges()
{
    map_string_to_word *            WordMap = m_Words->GetMap();
    map_string_to_word_ptr_iter     word_iter(*WordMap);
    simple_sig_graph_edge *          p_SigGraphEdge;
    CSignature *                    pSig, * this_sig_ptr, *the_other_sig_ptr;
    CWord*                          pWord;
    morph_t                         difference;
    stem_t                          this_stem, the_other_stem;

    while (word_iter.hasNext())   {
        pWord = word_iter.next().value();
        QMapIterator<stem_t, Parse_triple*> iter_triple_1 (*pWord->get_parse_triple_map());
        while (iter_triple_1.hasNext())
        {
            this_stem = iter_triple_1.next().key();
            int this_stem_length = this_stem.length();
            Parse_triple * this_triple = iter_triple_1.value();
            sig_string this_sig = this_triple->p_sig_string;
            CSignature* this_sig_ptr;
            m_SuffixesFlag?
                this_sig_ptr = m_Signatures->find_or_fail(this_sig):
                this_sig_ptr = m_PrefixSignatures->find_or_fail(this_sig);
            QMapIterator<stem_t,Parse_triple*> iter_triple_2 = iter_triple_1;
            while (iter_triple_2.hasNext())
            {
                the_other_stem = iter_triple_2.next().key();
                int the_other_stem_length = the_other_stem.length();
                Parse_triple* the_other_triple = iter_triple_2.value();
                sig_string the_other_sig = the_other_triple->p_sig_string;
                m_SuffixesFlag?
                    the_other_sig_ptr = m_Signatures->find_or_fail(the_other_sig):
                    the_other_sig_ptr = m_PrefixSignatures->find_or_fail(the_other_sig);
                if (this_sig == the_other_sig){continue;}
                if (this_stem_length > the_other_stem_length ){     // this cannot happen now (it could in the previous formulation -- so fix this);
                            int length_of_difference = this_stem_length - the_other_stem_length;
                            m_SuffixesFlag?
                                difference = this_stem.mid(the_other_stem_length):
                                difference = this_stem.left(length_of_difference);
                            p_SigGraphEdge = new simple_sig_graph_edge (this, this_sig_ptr, the_other_sig_ptr, this_sig,the_other_sig,
                                                                        difference, pWord->get_key(), this_stem, the_other_stem);
                 } else{
                     int length_of_difference = the_other_stem_length - this_stem_length;
                     m_SuffixesFlag?
                           difference = the_other_stem.mid(this_stem_length):
                           difference = the_other_stem.left(length_of_difference);
                     p_SigGraphEdge = new simple_sig_graph_edge(this, the_other_sig_ptr, this_sig_ptr,  the_other_sig,this_sig,
                                                                difference, pWord->get_key(), the_other_stem, this_stem);
                }
                m_SigGraphEdgeList.append(p_SigGraphEdge);
                pWord->add_to_autobiography("multiple parse=" + this_stem + "=" +  this_sig + "=" + the_other_stem + "=" + the_other_sig);
            } // end of looking at stems longer than this_stem
        } //end of looking at each stem in this word.
    } // each word
}

/*!
 * This function takes the SigTreeEdge list, and makes a smaller list composed of
 * SigTreeEdges which share the same signature pair and string-difference. This
 * flavor of sig_graph_edge contains a list of all the words that participate in
 * this particular sig_graph_edge.
 *
 * This is Part 2 of the 3rd major function of Crab 2.
 */
void CLexicon::compute_sig_graph_edge_map() {
morph_t         edge_label;
word_t          this_word;
simple_sig_graph_edge * p_sig_graph_edge;
sig_graph_edge      //  * p_sig_graph_edge_2,
                     * p_sig_graph_edge_3;
lxa_sig_graph_edge_map* p_EdgeMap = & m_SigGraphEdgeMap;

QListIterator<simple_sig_graph_edge*> this_simple_sig_graph_edge_iter (m_SigGraphEdgeList);
while (this_simple_sig_graph_edge_iter.hasNext())
{
    p_sig_graph_edge = this_simple_sig_graph_edge_iter.next();
    edge_label = p_sig_graph_edge->label();
    this_word  = p_sig_graph_edge->word;
    // --> We iterate through the simple Edges contained in the TreeEdge List            <-- //
    // --> We build a map of larger TreeEdges, each containing multiple stems and words. <-- //
    if (p_EdgeMap->contains(edge_label)){
        p_sig_graph_edge_3 = p_EdgeMap->value(edge_label);
        word_stem_struct * this_word_stem_struct = new word_stem_struct;
        this_word_stem_struct->word = this_word;
        this_word_stem_struct->stem_1 = p_sig_graph_edge->stem_1;
        this_word_stem_struct->stem_2 = p_sig_graph_edge->stem_2;
        QString this_label = this_word_stem_struct->get_label();
        if ( ! p_sig_graph_edge_3->shared_word_stems.contains(this_label)){
               p_sig_graph_edge_3->shared_word_stems[this_label] = this_word_stem_struct;
        }
     } else {  // --> start a new sig_graph_edge with multiple stems <-- //
        sig_graph_edge * p_sig_graph_edge_2 = new sig_graph_edge(*p_sig_graph_edge);
        m_SigGraphEdgeMap.insert(p_sig_graph_edge_2->label(),p_sig_graph_edge_2);
     }
}
}
