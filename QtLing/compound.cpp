#include "compound.h"
#include "Lexicon.h"
#include "StemCollection.h"
#include "WordCollection.h"
#include "Word.h"
#include <QMap>
#include <QPair>
#include <QDebug>
#include <QProgressBar>


// -------------- CompoundComponent ---------------- //
CompoundComponent::CompoundComponent(const word_t &word):
    m_word(word) { }

void CompoundComponent::add_connection(CompoundWord *p_compword, int position)
{
    m_connections[p_compword->get_word()] = QPair<int, CompoundWord*>(position, p_compword);
}

bool CompoundComponent::check_valid()
{
    if (m_connections.size() < 2)
        return false;
    int first_position = m_connections.begin().value().first;
    CompoundConnectionMap::ConstIterator map_iter;
    for (map_iter = m_connections.constBegin();
         map_iter != m_connections.constEnd();
         map_iter++) {
        if (map_iter.value().first != first_position)
            return true;
    }
    return false;
}

// -------------- CompoundWord ---------------- //
CompoundWord::CompoundWord(const word_t &word):
    m_word(word){ }


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

bool CompoundWord::remove_composition_if_contains(CompoundComponent* p_component)
{
    //qDebug() << "\tcheck if" << m_word << "contains" << p_component->get_word();
    QList<CompoundComposition*>::iterator comp_list_iter;
    for (comp_list_iter = m_compositions.begin();
         comp_list_iter != m_compositions.end();) {
        CompoundComposition* p_comp_list = *comp_list_iter;
        //qDebug() << "\tPointer of comp_list:" << p_comp_list;
        if (p_comp_list->contains(p_component)) {
            delete p_comp_list;
            //qDebug() << "\t comp_list" << p_comp_list << "deleted";
            comp_list_iter = m_compositions.erase(comp_list_iter);
        } else {
            comp_list_iter++;
        }
    }
    return m_compositions.size() == 0;
}

QString CompoundWord::composition_to_str(CompoundComposition *p_composition)
{
    QList<word_t> comp_list;
    foreach (CompoundComponent* component, *p_composition) {
        comp_list.append(component->get_word());
    }
    return QString(comp_list.join('='));
}

// -------------- CompoundComponentCollection ---------------- //

CompoundComponentCollection::CompoundComponentCollection
(CompoundWordCollection *p_compword_collection, CLexicon *p_lexicon):
    m_word_collection(p_compword_collection), m_lexicon(p_lexicon) {}

CompoundComponentCollection::~CompoundComponentCollection()
{
    QMap<QString, CompoundComponent*>::iterator iter;
    for (iter = m_map.begin(); iter != m_map.end(); iter++) {
        delete iter.value();
    }
}

CompoundComponent* CompoundComponentCollection::add_or_find_compound_component
(const word_t& str_word)
{
    QMap<QString, CompoundComponent*>::ConstIterator comp_iter;
    comp_iter = m_map.find(str_word);
    if (comp_iter == m_map.constEnd()) {
        CompoundComponent* new_component = new CompoundComponent(str_word);
        m_map.insert(str_word, new_component);
        return new_component;
    } else {
        return comp_iter.value();
    }
}

void CompoundComponentCollection::remove_component
(CompoundComponent *p_component)
{
    typedef QMap<word_t, QPair<int, CompoundWord*>> CompoundConnectionMap;
    // iterate through compound words containing that component
    const CompoundConnectionMap& ref_connections_map = p_component->get_connections();
    word_t str_word;
    foreach (str_word, ref_connections_map.keys()) {
        // skip if that word has already been removed
        if (!m_word_collection->get_map().contains(str_word))
            continue;

        // for each word, remove the composition containing that component
        CompoundWord* p_word = ref_connections_map[str_word].second;//conn_map_iter.value().second;
        bool remove_word = p_word->remove_composition_if_contains(p_component);
        // if that is the last composition in the list, remove that word
        // from the list of compound wordsd
        if (remove_word)
            m_word_collection->remove_compound_word(p_word);
    }

    const word_t& str_component = p_component->get_word();
    m_map.remove(str_component);
    delete p_component;
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
    QMap<word_t, CompoundWord*>::iterator iter;
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

    QMap<word_t, CompoundWord*>::ConstIterator iter;
    for (iter = other.m_map.constBegin(); iter != other.m_map.constEnd(); iter++) {
        CompoundWord* other_word = iter.value();
        const word_t& other_str_word = other_word->get_word();
        QList<CompoundComponent*>* other_composition;
        foreach (other_composition, other_word->m_compositions) {
            QStringList other_component_str_list;
            CompoundComponent* p_component;
            foreach (p_component, *other_composition)
                other_component_str_list.append(p_component->get_word());
            add_compound_word(other_str_word, other_component_str_list);
        }
    }
}

CompoundWord* CompoundWordCollection::get_compound_word(const word_t &word) const
{
    QMap<word_t, CompoundWord*>::ConstIterator iter = m_map.find(word);
    return iter == m_map.constEnd() ? nullptr : iter.value();
}

CompoundWord* CompoundWordCollection::add_compound_word
(const word_t& str_word, const QStringList& composition)
{
    QMap<word_t, CompoundWord*>::iterator iter = m_map.find(str_word);
    CompoundWord* p_curr_word;
    if (iter == m_map.end()) {
        p_curr_word = new CompoundWord(str_word);
        m_map.insert(str_word, p_curr_word);
    } else {
        p_curr_word = iter.value();
    }

    int composition_len = composition.length();
    QList<CompoundComponent*> curr_composition;
    for (int i = 0; i < composition_len; i++)
    {
        CompoundComponent* curr_comp;
        curr_comp = m_component_collection->add_or_find_compound_component(composition[i]);
        curr_composition.append(curr_comp);
        curr_comp->add_connection(p_curr_word, i);
    }
    p_curr_word->add_composition(curr_composition);
    return p_curr_word;
}

CompoundWord* CompoundWordCollection::add_compound_word
(const word_t& whole, const QString& part0, const QString& part1)
{
    QStringList composition_list = {part0, part1};
    return add_compound_word(whole, composition_list);
}

void CompoundWordCollection::remove_compound_word(CompoundWord* p_word)
{
    const word_t& str_word = p_word->get_word();
    /*
    m_lexicon->add_to_word_autobiographies(str_word,
        QString("[Compound]=I am removed."));*/
    m_map.remove(str_word);
    delete p_word;

}

void CompoundWordCollection::remove_invalid_components(QProgressBar *p_progressbar)
{
    QList<CompoundComponent*> list_to_remove;
    //QList<QString> list_to_remove;
    QMap<word_t, CompoundComponent*>& ref_components_map
            = m_component_collection->get_map();
    int itercount = 0;
    if (p_progressbar != nullptr) {
        p_progressbar->reset();
        p_progressbar->setMinimum(0);
        p_progressbar->setMaximum(ref_components_map.size());
    }
    QMap<word_t, CompoundComponent*>::iterator components_iter;
    CompoundComponent* p_component;
    foreach (p_component, ref_components_map) {
        if (p_progressbar != nullptr)
            p_progressbar->setValue(itercount++);
        if (!p_component->check_valid()) {
            list_to_remove.append(p_component);
        }
    }
    /*
    QMap<QString, CStem*>* p_stem_map = m_lexicon->get_suffix_flag()?
                m_lexicon->get_suffixal_stems()->get_map():
                m_lexicon->get_prefixal_stems()->get_map();*/
    CompoundComponent* p_component_to_remove;
    foreach (p_component_to_remove, list_to_remove) {
        /*
        const QString& str_component = p_component_to_remove->get_word();
        if (m_lexicon->get_words()->contains(str_component)) {
            m_lexicon->add_to_word_autobiographies(str_component,
                QString("[Compound]=I am removed because I am invalid."));
        }
        if (p_stem_map->contains(str_component)) {
            m_lexicon->add_to_stem_autobiographies(str_component,
                QString("[Compound]=I am removed because I am invalid."));
        }*/
        m_component_collection->remove_component(p_component_to_remove);

    }
}
