#include "compound.h"
#include "Lexicon.h"
#include "WordCollection.h"
#include "Word.h"
#include <QMap>
#include <QPair>
#include <QDebug>

// -------------- CompoundComponent ---------------- //
CompoundComponent::CompoundComponent(QString &word, CWord* p_cword):
    m_word(word), m_cword_ptr(p_cword)
{

}

void CompoundComponent::add_connection(CompoundWord *p_compword, int position)
{
    m_connections[p_compword->get_word()] = QPair<int, CompoundWord*>(position, p_compword);
}

// -------------- CompoundWord ---------------- //
CompoundWord::CompoundWord(QString &word, CWord *p_cword):
    m_word(word), m_cword_ptr(p_cword) { }

CompoundWord::CompoundWord(CWord* p_cword):
    m_word(p_cword->get_word()), m_cword_ptr(p_cword) { }

CompoundWord::~CompoundWord()
{
    foreach (CompoundComposition* composition, m_compositions)
        delete composition;
}

void CompoundWord::add_composition(const CompoundComposition& composition)
{
    CompoundComposition* new_composition = new CompoundComposition();
    foreach (CompoundComponent* component, composition) {
        new_composition->append(component);
    }
    m_compositions.append(new_composition);
}

QString CompoundWord::composition_to_str(CompoundComposition *p_composition)
{
    QStringList comp_list;
    foreach (CompoundComponent* component, *p_composition) {
        comp_list.append(component->get_word());
    }
    return QString(comp_list.join('='));
}

// -------------- CompoundComponentCollection ---------------- //

CompoundComponentCollection::CompoundComponentCollection
(CompoundWordCollection *p_word_collection, CLexicon *p_lexicon):
    m_word_collection(p_word_collection), m_lexicon(p_lexicon) {}

CompoundComponentCollection::~CompoundComponentCollection()
{
    QMap<QString, CompoundComponent*>::iterator iter;
    for (iter = m_map.begin(); iter != m_map.end(); iter++) {
        delete iter.value();
    }
}

CompoundComponent* CompoundComponentCollection::add_or_find_compound_component
(CWord* p_word)
{
    QString str_word = p_word->get_word();
    QMap<QString, CompoundComponent*>::ConstIterator comp_iter;
    comp_iter = m_map.find(str_word);
    if (comp_iter == m_map.constEnd()) {
        CompoundComponent* new_component = new CompoundComponent(str_word, p_word);
        m_map.insert(str_word, new_component);
        return new_component;
    } else {
        return comp_iter.value();
    }
}

// -------------- CompoundWordCollection ---------------- //
CompoundWordCollection::CompoundWordCollection(CLexicon* p_lexicon):
    m_lexicon(p_lexicon)
{
    m_component_collection = new CompoundComponentCollection(this, p_lexicon);
}

/*!
 * \brief Destructor
 */
CompoundWordCollection::~CompoundWordCollection()
{
    QMap<QString, CompoundWord*>::iterator iter;
    for (iter = m_map.begin(); iter != m_map.end(); iter++) {
        delete iter.value();
    }
    delete m_component_collection;
}

/*!
 * \brief Copy Constructor
 * \param other
 */
CompoundWordCollection::CompoundWordCollection(const CompoundWordCollection &other):
    m_lexicon(other.m_lexicon)
{
    m_component_collection = new CompoundComponentCollection(this, m_lexicon);

    QMap<QString, CompoundWord*>::ConstIterator iter;
    for (iter = other.m_map.constBegin(); iter != other.m_map.constEnd(); iter++) {
        CompoundWord* other_word = iter.value();
        CWord* other_cword = other_word->m_cword_ptr;
        QList<CompoundComponent*>* other_composition;
        foreach (other_composition, other_word->m_compositions) {
            QList<CWord*> other_cword_list;
            CompoundComponent* p_component;
            foreach (p_component, *other_composition)
                other_cword_list.append(p_component->get_cword_ptr());
            add_compound_word(other_cword, other_cword_list);
        }
    }
}

CompoundWord* CompoundWordCollection::get_compound_word(const QString &word) const
{
    QMap<QString, CompoundWord*>::ConstIterator iter = m_map.find(word);
    if (iter == m_map.constEnd()) {
        return NULL;
    } else {
        return iter.value();
    }
}

CompoundWord* CompoundWordCollection::add_compound_word
(CWord* p_word, QList<CWord*> &composition)
{
    QString str_word = p_word->get_word();
    QMap<QString, CompoundWord*>::iterator iter = m_map.find(str_word);
    CompoundWord* curr_word;
    if (iter == m_map.end()) {
        curr_word = new CompoundWord(str_word, p_word);
        m_map.insert(str_word, curr_word);
    } else {
        curr_word = iter.value();
    }
    int composition_len = composition.length();
    QList<CompoundComponent*> curr_composition;
    for (int i = 0; i < composition_len; i++)
    {
        CompoundComponent* curr_comp;
        curr_comp = m_component_collection->add_or_find_compound_component(composition[i]);
        curr_composition.append(curr_comp);
        curr_comp->add_connection(curr_word, i);
    }
    curr_word->add_composition(curr_composition);
    return curr_word;
}

CompoundWord* CompoundWordCollection::add_compound_word
(CWord* whole, CWord *part0, CWord *part1)
{
    QList<CWord*> composition_list = {part0, part1};
    return add_compound_word(whole, composition_list);
}
