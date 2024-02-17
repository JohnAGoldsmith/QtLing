#include "Lexicon.h"
#include "WordCollection.h"
#include "StemCollection.h"
#include "compound.h"

/**
 * @brief CLexicon::Crab_5
 *
 */
void CLexicon::Crab_5()
{
    m_StatusBar->showMessage("Crab 5:find simple compounds.");
    step10_find_compounds();
    m_StatusBar->showMessage("Crab 5:simple compounds completed.");
}
/**
 * @brief CLexicon::Crab_compounds
 *
 */
void CLexicon::Crab_compounds()
{
    m_StatusBar->showMessage("Crab compounds:find simple compounds.");
    step10_find_compounds();
    m_StatusBar->showMessage("Crab compounds:simple compounds completed.");
}

extern QString QStringList2QString(QStringList);

void CLexicon::step10_find_compounds()
{

    CStemCollection* p_stems = m_suffix_flag ?
        m_suffixal_stems :
        m_prefixal_stems;
    initialize_progress(m_Words->get_count() + p_stems->get_count());
    m_StatusBar->showMessage("8: Finding Compounds - part 1.");

    find_hyphenated_compounds();

    //find_stems_made_of_stems();
    find_words_made_of_words();

    return;

}
void CLexicon::find_hyphenated_compounds(){
    CStemCollection* p_stems = m_suffix_flag ? m_suffixal_stems : m_prefixal_stems;
    initialize_progress(m_Words->get_count() + p_stems->get_count());
    m_StatusBar->showMessage("5: Finding Compounds - part 1.");

    int MINIMUM_COMPOUND_STEM_LENGTH (4);

    for (int wordno = 0; wordno < m_Words->get_count(); wordno++){
        CWord* word = m_Words->get_word(wordno);
        QString word_string = word->get_key();
        if (word_string.contains("-")){
            QStringList components = word_string.split("-");
            if (components.length() > 2){
                continue;
                // to do: deal with compounds longer than 2 pieces TO DO
            }
            QString compound_string =  components.join("=");
            word->analyze_as_compound(components[0], components[1]);
            //m_Compounds->add_compound_word(word, components); // 2023
        }
    }
}

void CLexicon::find_stems_made_of_stems(){
    // iterate through stems, find those that are compounds of stem+stem
    // alphabetize the stems first:
    int MINIMUM_SIZE_COMPOUND_COMPONENT = 4;
    for (int m = 0; m < m_suffixal_stems->get_count(); m++){
        CStem* stem1_object = m_suffixal_stems->get_at(m);
        QString stem1 = stem1_object->get_key();
        //qDebug() << 77 << stem1 << m;
        if (stem1.length() <   MINIMUM_SIZE_COMPOUND_COMPONENT){
           continue;
        }
        for (int n = m+1; n < m_suffixal_stems->get_count(); n++){
            QString stem2 = m_suffixal_stems->get_at(n)->get_key();
            if (stem2.length() <  2 * MINIMUM_SIZE_COMPOUND_COMPONENT){
                continue;
            }
            if (!stem2.startsWith(stem1)){
                break;
            }
            QString extension = stem2.mid(stem1.length());
            if (extension.length() < MINIMUM_SIZE_COMPOUND_COMPONENT){
                continue;
            }
            if (m_suffixal_stems->find_or_fail(extension)){
                qDebug() << 91 << stem1 << " = `" << extension;
                stem1_object->make_compound(stem1, extension);
            }
        }
    }
}

void CLexicon::find_words_made_of_words(){
    int MINIMUM_SIZE_COMPOUND_COMPONENT = 3;
    for (int m = 0; m < m_Words->get_count(); m++){
        CWord* word_object = m_Words->get_at(m);
        QString word1 = word_object->get_key();
        //qDebug() << 77 << stem1 << m;
        if (word1.length() <   MINIMUM_SIZE_COMPOUND_COMPONENT){
            continue;
        }
        for (int n = m+1; n < m_Words->get_count(); n++){
            QString word2 = m_Words->get_at(n)->get_key();
            if (word2.length() <  2 * MINIMUM_SIZE_COMPOUND_COMPONENT){
                continue;
            }
            if (!word2.startsWith(word1)){
                break;
            }
            QString extension = word2.mid(word1.length());
            if (extension.length() < MINIMUM_SIZE_COMPOUND_COMPONENT){
                continue;
            }
            if (m_Words->find_or_fail(extension)){
                qDebug() << 91 << word1 << " = " << extension;
                //stem1_object->make_compound(stem1, extension);
            }
        }
    }
}
