#ifndef MDL_H
#define MDL_H

#include "QString"
#include "QMap"
#include "Lexicon.h"
#include "Word.h"
class MDL
{
public:
    CLexicon*               m_Lexicon;
    double                  this_MDL;
    bool                    m_suffix_flag;
    double                  bits_per_letter;
    QMap<QString, int>                     m_affixes;            // counts number of word types using each affix -- not obvious this is the way to count pointer length though.
    QMap<QString, double>                  m_affix_pointer_length; // length in bits to each affix
    QMap<QString, QMap<QString, double>* > m_affix2corpus_count_given_signature;
    QMap<QString, QMap<QString, double>* > m_affix2pointer_length_affix_choice_within_signature;
    QMap<QString, int>                     m_affix2corpus_count;
    QMap<QString, int>                     m_affix2index;
    QMap<QString, int>      stems;
    QMap<QString, double>   stem2corpus_count; // how do these differ?
    QMap<QString, double>   stem2pointer_length_given_signature;
    QMap<QString, double>   length_of_pointer_to_affix;
    QMap<QString, int>      signatures;         // the int serves as an index, used for pointers.
    QMap<QString, double>   signature2corpus_count; // counts number of word tokens using each signature -- i.e., corpus counts
    QMap<QString, double>   signature_plogs;    // number of bits in pointer to sig, based on word corpus counts;
    QMap<QString, QStringList*> signature2stems; // map from signature to a list of its stems
    QMap<QString, int>      signature_stem_count; // counts how many stems in a signature
    QMap<QString, int>      alphabet;           // counts number of occurrences of each letter in the word list.
    QMap<QString, int>      unanalyzed_words;   // the integer is a "pointer" -- a unique index for the word.
    QMap<QString, double>   m_unanalyzed_word2pointer_length;
    QMap<QString, QString>  word2sig;
    QMap<QString, QString>  word2stem;
    QMap<QString, QString>  word2affix;
    QMap<QString, QList<int>* > word_counts_for_each_stem; // the values are lists of ints, one for each affix in the stem's signature
    QMap<QString, int>      m_word_token_counts;


    double m_total_token_count_of_words;
    double m_token_count_of_unanalyzed_words ;
    double m_token_word_count_in_all_signatures;

    double total_type_count_of_words;
    double type_word_count_in_all_signatures;
    double m_type_count_of_unanalyzed_words;

    //void analyze_all_words();



    int total_letter_count_in_stems;
    int total_letter_count_in_affixes;
    //int total_letter_count;

    int grammar_length_in_bits;
    /*this will include:
     * 1. affix and stem phonological content: the sum of these in:
     *
     *      function: stem_plus_affix_content_in_bits_Architecture_1
     *
     * 2. Pointers which express the architecture of the system:
     * --signature to affix pointers:
     *      signature_to_affix_pointers_Architexture
     * --pointers to stems
     * flagging which words are analyzed and which are not
     *
     * 3. Pointers that will be used in the corpus assessment --
     * all of these involve choices in the grammar:
     * a. choice of a signature
     * b. choice of a stem, given the signature
     * c. choice of an affix, given the signature
     *
     *
     * 6. cost of choosing a stem, given a signature: entropy of stem choice within signature.
     * cost_of_a_set_of_pointers_to_signatures_3
     *
     */


//    int corpus_description_in_bits;
    double m_corpus_description_from_analyzed_words;
    double m_corpus_description_from_unanalyzed_words;


    double stem_content_in_bits;
    double affix_content_in_bits;
    double stem_plus_affix_content_in_bits_Architecture_1;
    double total_cost_of_affix_choice_within_all_signatures_Choice_2;
    double pointers_to_signatures_Choice_3;
    double choice_of_stems_within_sigs_Choice_4;
    double pointers_to_stems_among_stems_Architeture_5; // todo --
    double m_flag_to_mark_an_unanalyzed_word;
    double m_flag_to_mark_an_analyzed_word;
    double m_total_flagging_of_analyzed_words;
    double m_total_flagging_of_unanalyzed_words;
    double signature_to_affix_pointers_Architexture_8;
    double m_corpus_costs;



    /* Cost of grammar:
     * Cost of content in stems and in affixes;
     * Pointer in each signature from each affix choice in the sig to the affix content: from plog of token count of each affix
     * Pointer in each signature to each affix choice within the sisgnature: from plog of affix within the signature
     * Pointer in each signature to each stem content based on plog of stem token count
     * Pointer in each signature to its stem choices: based on plogs of stem choices inside signature, same as preceding line
     * Choice for each word as to analyzed or not
     * For each unanalyzed word its content and a pointer to it
     *
     * Cost of corpus description:
     * For each word:
     *  Analyzed or not -- that choice in bits
     *  if not analyzed:
     *    pointer to its content * number of occurrences
     *  if analyzed:
     *      (pointer to sig choice + pointer to stem choice + pointer to affix choice) * number of occurrences
     */

public:
    MDL(CLexicon* );
    void compute_signature_pointer_lengths_Choice(QTextStream&, QTextStream & out_summaries);
    void corpus_description(QTextStream& out_corpus, QTextStream & out_summaries);
    void process_a_signature(CSignature*, QTextStream&, QTextStream&);
    void process_signatures(CSignatureCollection*, QTextStream& distributions, QTextStream& summaries);
    void process_signatures_bis_Architecture(QTextStream&, QTextStream& summaries);
    void process_stems_Content(QTextStream&, QTextStream& out_summaries);
    void process_a_stem_given_a_signature_and_its_words( CSignature*,QString, QTextStream&);
    void process_a_word(CWord*, QTextStream& );
    void process_affixes(QTextStream&, QTextStream& out_summaries);
    void process_unanalyzed_words(QTextStream&, QTextStream&);
    void adjust_stem_token_count(QString, int );
    void adjust_affix_token_count_For_Choice(QString affix, QString word, int word_count,  QString);
    void adjust_word_token_count(QString word, int count);
    void compute_stem_pointer_lengths_within_signature_Choice(QTextStream& distributions, QTextStream& summaries);
    void reports(QTextStream&);
    void print_grammar(QTextStream&);
    void compute_corpus_length(QString, QTextStream&);
    void weigh_analyzed_and_unanalyzed_words(QTextStream& , QTextStream& summaries);
    CSignatureCollection* get_signatures();
    void add_letters_to_alphabet(QString);

    bool    word_is_analyzed(QString word);


    double get_grammar_length_in_bits();
    double get_total_letter_count() {return total_letter_count_in_stems + total_letter_count_in_affixes;};
    double get_corpus_description(){ return m_corpus_description_from_unanalyzed_words + m_corpus_description_from_analyzed_words;}};

#endif // MDL_H
