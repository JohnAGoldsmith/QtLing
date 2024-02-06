

//      Find full signatures
// Not used:
/*
void remove_final_letter (QStringList & stem_list, bool suffix_flag){
    for (int i = 0; i < stem_list.size(); i++){
        if (suffix_flag){
            stem_list[i] = stem_list[i].left(stem_list[i].length()-1);
        }else{
            stem_list[i] = stem_list[i].mid(1);
        }
    }
}

// Not used:
void add_initial_letter (QStringList & this_affix_list, QString letter, bool suffix_flag){
    for (int i = 0; i < this_affix_list.size(); i++){
        if (this_affix_list[i]== "NULL"){
            this_affix_list[i] = letter;
        } else {
            if (suffix_flag){
                this_affix_list[i] = letter + this_affix_list[i];
            }else{
                this_affix_list[i] = this_affix_list[i] + letter;
            }
        }
    }
}
*/

/*!
 * This function looks at pairs of signatures joined by a sig-tree-edge, where
 * the morpheme that separates them is a single letter. We look to see  how tight
 * the fit is between these two sets of signatures.
 *
 * Not currently used. 2022.
 */
/*
void CLexicon::test_for_phonological_relations_between_signatures()
{
    sig_pair_iter  sig_iter (m_SigPairMap);
    QString difference;
    QSet<QString> differences_1_letter, differences_longer;
    while (sig_iter.hasNext()){
        sig_iter.next();
        QString morph = sig_iter.value()->morph;
        if (morph.length() == 1){
            differences_1_letter.insert(sig_iter.value()->morph);
        } else{
            differences_longer.insert(sig_iter.value()->morph);
        }
    }

    //QStringList differences = differences_1_letter.toList();
    QStringList differences;
    foreach (QString diff, differences_1_letter){
        differences.append(diff);
    }
    differences.sort();
    QSet<CSignature*> SignatureSet_1, SignatureSet_2;

    for (int i = 0; i < differences.size(); i++){
        difference = differences[i];
        //--> Pull out the sig_graph_edges with each particular difference (morpheme).
        sig_iter.toFront();
        while (sig_iter.hasNext()){
            sig_iter.next();
            if (sig_iter.value()->morph == difference){
                CSignature* pSig1 = sig_iter.value()->m_sig_1;
                CSignature* pSig2 = sig_iter.value()->m_sig_2;
                SignatureSet_1.insert(pSig1);
                SignatureSet_2.insert(pSig2);
                *m_PassiveSignatures << pSig1;
            }
        }

        //--> Now these two sets have signatures which are from opposite sides of the phonological rule. <---//
        compare_opposite_sets_of_signatures(&SignatureSet_1, &SignatureSet_2, difference);

    }
}
*/

/*!
 * This function takes two sets of signatures, each taken from the opposing signatures
 * in a sig-tree-edge, where the morph separating them is a specific morph of length = 1 letter.
 *
 * This is not currently being used.
 */
/*
void CLexicon::compare_opposite_sets_of_signatures(QSet<CSignature*>* sig_set_1, QSet<CSignature*>* sig_set_2, QString morph)
{   sig_pair * p_edge;
    //TEMP:
    (void) sig_set_1;
    (void) sig_set_2;
    CSignature* pSig_1, *pSig_2;
    QHash<QString,int> Counts;
    foreach(p_edge,  m_SigPairMap){
        if (p_edge->morph == morph){
            pSig_1 = p_edge->m_sig_1;
            pSig_2 = p_edge->m_sig_2;
            QString code = pSig_1->get_key() + "@" + pSig_2->get_key();
            Counts[code] = p_edge->get_number_of_words();
        }
    }
}
*/

/*   Not used
void CLexicon::dump_suffixes(QList<QString> * pList)
{
    return m_Suffixes->get_suffixes(pList);
}
*/

/**
 * @brief CLexicon::collect_parasuffixes
 * Parasignatures are signatures with only a single stem, hence are not used directly.
 * Parasuffixes are affixes found in a parasignature.
 *
 *
 */
/*
void CLexicon::collect_parasuffixes()
{
    sigstring_t     sigstring;
    suffix_t        suffix;
    CSignature*     pSig;
    CSuffix *       pSuffix;
    QStringList     suffixes;
    map_sigstring_to_sig_ptr_iter sig_iter (* m_ParaSignatures->get_map());
    while (sig_iter.hasNext())
    {
        pSig = sig_iter.next().value();
        sigstring = pSig->get_key();
        qDebug() << sigstring;
        suffixes = sigstring.split("=");
        foreach (suffix, suffixes){
            pSuffix = *m_ParaSuffixes <<  suffix;
            pSuffix->increment_sig_count();
        }
    }
    m_ParaSuffixes->sort_by_count();
}
*/
