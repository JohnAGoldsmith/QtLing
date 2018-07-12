#include "compound.h"
#include "Lexicon.h"
#include "WordCollection.h"
#include <QDebug>

CompoundComponentCollection::CompoundComponentCollection()
{

}

CompoundComponentCollection::CompoundComponentCollection
(CompoundWordCollection *p_word_collection, CLexicon *p_lexicon):
    m_word_collection(p_word_collection), m_lexicon(p_lexicon) {}

CompoundComponent* CompoundComponentCollection::add_or_find_compound_component
(QString& word)
{
    QMap<QString, CompoundComponent*>::ConstIterator comp_iter;
    comp_iter = m_map.find(word);
    if (comp_iter == m_map.constEnd())
        return comp_iter.value();
    else {
        CWord* cword_ptr = m_lexicon->get_words()->get_word(word);
        if (cword_ptr == NULL) {
            qDebug() << "CompoundComponentCollection::add_or_find_compound_component()"
                        ": added compound is not in CWordCollection";
            return NULL;
        }
        CompoundComponent* new_component = new CompoundComponent(word, cword_ptr);
        m_map.insert(word, new_component);
        return new_component;
    }
}


CompoundWordCollection::CompoundWordCollection(CLexicon* p_lexicon):
    m_lexicon(p_lexicon)
{
    m_component_collection = new CompoundComponentCollection(this, p_lexicon);
}

CompoundWord* CompoundWordCollection::add_compound_word
(QString &word, QList<QString> &composition)
{
    if (!m_map.contains(word)) {
        CWord* cword_ptr = m_lexicon->get_words()->get_word(word);
        if (cword_ptr == NULL) {
            qDebug() << "CompoundWordCollection::add_compound_word():"
                        " added word is not in CWordCollection";
            return NULL;
        }
        CompoundWord* new_word = new CompoundWord(word, cword_ptr);

        int composition_len = composition.length();
        for (int i = 0; i < composition_len; i++)
        {
            CompoundComponent* curr_comp;
            curr_comp = m_component_collection->add_or_find_compound_component(composition[i]);
            if (curr_comp = NULL) {
                delete new_word;
                return NULL;
            }
            new_word->add_component(curr_comp);
            curr_comp->add_connection(new_word, i);
            m_map.insert(word, new_word);
        }

    } else {
        qDebug() << "CompoundWordCollection::add_compound_word(): "
                    "adding a compound word that exists: " << word;
        return NULL;
    }
}

CompoundWord* CompoundWordCollection::add_compound_word
(QString &part1, QString &part2)
{
    QString str_word = part1 + part2;
    if (!m_map.contains(str_word)) {
        CWord* cword_ptr = m_lexicon->get_words()->get_word(str_word);
        if (cword_ptr == NULL) {
            qDebug() << "CompoundWordCollection::add_compound_word():"
                        " added word is not in CWordCollection";
            return NULL;
        }
        CompoundComponent *comp0, *comp1;
        comp0 = m_component_collection->add_or_find_compound_component(part1);
        if (comp0 == NULL) {
            return NULL;
        }
        comp1 = m_component_collection->add_or_find_compound_component(part2);
        if (comp1 == NULL) {
            return NULL;
        }
        CompoundWord* new_word = new CompoundWord(word, cword_ptr);
        new_word->add_component(comp0);
        new_word->add_component(comp1);
        comp0->add_connection(new_word, 0);
        comp1->add_connection(new_word, 1);
        m_map.insert(str_word, new_word);

    } else {
        qDebug() << "CompoundWordCollection::add_compound_word(): "
                    "adding a compound word that exists: " << str_word;
        return NULL;
    }
}
