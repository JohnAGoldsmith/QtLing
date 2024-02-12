#ifndef SIGPAIR_H
#define SIGPAIR_H
#include <QDebug>
#include "Typedefs.h"

class CLexicon;


// a word_sig_pair expresses information about pairs of signatures that both contain a particular word
// under two different stem-analyses.
//-----------------------------------------------------------------------//
class word_sig_pair{
    //-----------------------------------------------------------------------//
public:
    CSignature*         m_sig_1;
    CSignature*         m_sig_2;
    CLexicon *          m_Lexicon;
    sigstring_t         m_sig_string_1;
    sigstring_t         m_sig_string_2;
    double              m_sig_1_entropy = -1;
    double              m_sig_2_entropy = -1;
    morph_t             morph;
    word_t              word;
    stem_t              longer_stem; // the longer stem, was stem_1
    stem_t              shorter_stem;  // was stem_2
    word_sig_pair();
    word_sig_pair(CLexicon* lexicon, CSignature* pSig1, CSignature* pSig2, sigstring_t sig_string_1, sigstring_t sig_string_2,morph_t m,word_t w, stem_t stem1, stem_t stem2)
    {

        m_sig_1 = pSig1;
        m_sig_2 = pSig2;
        m_Lexicon = lexicon;
        m_sig_string_1 = sig_string_1;
        m_sig_string_2 = sig_string_2;
        morph = m;
        word = w;
        longer_stem = stem1;
        shorter_stem = stem2;
        if (longer_stem.length() < shorter_stem.length()){
            qDebug() << "constructor of word_sig_pair, the stems are in reversed order.";
        }
    };
    QString label() {return morph + "/" + m_sig_string_1 + "/" + m_sig_string_2; }
};
//-----------------------------------------------------------------------//

//-----------------------------------------------------------------------//
class sig_pair{   // was: sig_graph_edge{
    //-----------------------------------------------------------------------//
public:
    CSignature*  m_sig_1;
    CSignature*  m_sig_2;
    CLexicon*    m_lexicon;
    sigstring_t  m_sig_string_1;
    sigstring_t  m_sig_string_2;
    double       m_sig_1_entropy = -1;
    double       m_sig_2_entropy = -1;
    morph_t      morph;
    QStringList  my_stems;
    QMap<QString, word_stem_struct*> shared_word_stems;
    sig_pair();
    sig_pair(QString amorph, CSignature* sig1, CSignature* sig2){
        morph = amorph;
        m_sig_1 = sig1;
        m_sig_2 = sig2;
    }
    sig_pair(QString a_morph, QString shorter_stem_sig_string, QString longer_stem_sig_string){
        morph = a_morph;
        m_sig_string_1 = shorter_stem_sig_string;
        m_sig_string_2 = longer_stem_sig_string;
    }
    sig_pair(word_sig_pair this_word_sig_pair){
        m_lexicon = this_word_sig_pair.m_Lexicon;
        m_sig_1 = this_word_sig_pair.m_sig_1;
        m_sig_2 = this_word_sig_pair.m_sig_2;
        m_sig_string_1  = this_word_sig_pair.m_sig_string_1;
        m_sig_string_2  = this_word_sig_pair.m_sig_string_2;
        m_sig_1_entropy = this_word_sig_pair.m_sig_1_entropy;
        m_sig_2_entropy = this_word_sig_pair.m_sig_2_entropy;
        morph = this_word_sig_pair.morph;
        /*
             word_stem_struct * this_word_stems = new word_stem_struct;
             this_word_stems->word = this_word_sig_pair.word;
             this_word_stems->stem_1 = this_word_sig_pair.stem_1;
             this_word_stems->stem_2 = this_word_sig_pair.stem_2;
             */
        word_stem_struct * this_word_stems = new word_stem_struct (
            this_word_sig_pair.word,
            this_word_sig_pair.longer_stem,
            this_word_sig_pair.shorter_stem);
        shared_word_stems[this_word_stems->get_label()] = this_word_stems;
    }
    QString     label() {return morph + "/" + m_sig_string_1 + "/" + m_sig_string_2; }
    int         get_number_of_words() {return my_stems.size();}
    CSignature* get_sig_1() {return m_sig_1;}
    CSignature* get_sig_2() {return m_sig_2;}
    sigstring_t get_sig1_string() { return m_sig_string_1;}
    sigstring_t get_sig2_string() { return m_sig_string_2;}
    morph_t     get_morph() {return morph;}
    QString     display();
    void        add_stem(QString stem) {my_stems.append(stem);}
    QStringList& get_my_stems() {return my_stems;}
    bool        contains_stem(QString stem) {return my_stems.contains(stem);}
};


#endif // SIGPAIR_H
