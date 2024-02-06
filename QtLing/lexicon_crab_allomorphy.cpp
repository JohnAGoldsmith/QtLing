
#include <Lexicon.h>
#include <stringalignment.h>

void CLexicon::step11_find_allomorphic_signatures(){
    for (int i = 0; i < m_Signatures->get_count(); i++){
        CSignature * sig1 = m_Signatures->at(i);
        for (int j = i+1; j < m_Signatures->get_count(); j++){
            CSignature * sig2 = m_Signatures->at(j);
            if ( sig2->get_number_of_affixes() == sig1->get_number_of_affixes() ) {
                StringAlignment alignment (sig1->get_affix_string_list(), sig2->get_affix_string_list());
                alignment.match_up();
                //qDebug() << 12 << "Aligned? " << alignment.properly_aligned() <<   alignment.display() << "\n";
            }
        }
    }
}
