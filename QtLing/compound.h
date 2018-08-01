#ifndef COMPOUND_H
#define COMPOUND_H

#include <QString>
#include <QList>
#include <QMap>
#include <QPair>
#include "Typedefs.h"

class CWord;
class CLexicon;
class CompoundComponent;
class CompoundWord;
class CompoundComponentCollection;
class CompoundWordCollection;
class QProgressBar;

/*!
 * \brief CompoundComponent is a class that contains information about a word
 * that can be considered a part (or "components") of compound words.
 *
 * For instance, the word "lighthouse" is composed of two parts, "light" and
 * "house". "light" and "house" are each represented by a CompoundComponent
 * object. Each compound word has a list of connections, i.e. a list that keeps
 * track which words this component appeared in, and what position in that
 * word. e.g. the CompoundComponent object of "light" would have in its
 * m_connections the following entry in its m_connections map:
 *   key: "lighthouse", value: (0, <ptr>)
 * where 0 represents the leftmost position in a compound word, and <ptr>
 * is a pointer to the corresponding CompoundWord object for "lighthouse".
 *
 */
class CompoundComponent
{
public:
    typedef QMap<QString, QPair<int, CompoundWord*>> CompoundConnectionMap;
protected:
    friend class CompoundComponentCollection;
    QString                                     m_word;
    QMap<QString, QPair<int, CompoundWord*>>    m_connections;

public:
    CompoundComponent();
    CompoundComponent(const word_t& word);
    void                                        add_connection(CompoundWord* compword, int position);
    const CompoundConnectionMap&                get_connections() const { return m_connections; }
    const word_t&                               get_word() const {return m_word;}
    bool                                        check_valid();
};

/*!
 * \brief CompoundWord is a class that contains information about a compound
 * word.
 *
 * Each compound word found has a corresponding Compound Word object, which
 * contains a list of compositions. A "CompoundComposition" is a list of
 * pointers to CompoundComponents, and represents a possible combination
 * of compound components to form the compound word, in left-to-right order.
 * e.g. the word "lighthouse" has the composition "light" + "house" which is
 * represented by a QList containing pointers to the CompoundComponent objects
 * of these two words. A list of compositions is stored because Linguistica
 * may find multiple possible compositions for one word.
 */
class CompoundWord
{
public:
    typedef QList<CompoundComponent*>           CompoundComposition;
protected:
    friend class CompoundWordCollection;
    QString                                     m_word;
    QList<CompoundComposition*>                 m_compositions;

public:
    CompoundWord();
    ~CompoundWord();
    CompoundWord(const word_t& word);
    const QString &                             get_word() const { return m_word; }
    const QList<CompoundComposition*>&          get_compositions() const { return m_compositions; }
    //void                                        add_component(CompoundComponent* p_part);
    void                                        add_composition(const CompoundComposition&);
    bool                                        remove_composition_if_contains(CompoundComponent* p_comp);
    QString                                     composition_to_str(CompoundComposition* p_comp);

};

/*!
 * \brief The CompoundComponentCollection class is a wrapper class for a QMap
 * data structure containing pointers to all CompoundComponent objects created
 * during compound discovery.
 *
 * A component is also a word, hence the use of word_t
 */
class CompoundComponentCollection
{
    QMap<word_t, CompoundComponent*>   m_map;
    CompoundWordCollection*             m_word_collection;
    CLexicon*                           m_lexicon;
public:
    CompoundComponentCollection();
    CompoundComponentCollection(CompoundWordCollection* p_word_collection, CLexicon* p_lexicon);
    ~CompoundComponentCollection();

    QMap<word_t, CompoundComponent*>&  get_map() { return m_map; }
    CompoundComponent*                  add_or_find_compound_component(const word_t& str_word);
    void                                remove_component(CompoundComponent* p_component);
};

/*!
 * \brief The CompoundComponentCollection class is a wrapper class for a QMap
 * data structure containing pointers to all CompoundWord objects created
 * during compound discovery. This class serves as the primary interface for
 * accessing and storing data related to compound discovery.
 */
class CompoundWordCollection
{
    QMap<word_t, CompoundWord*>        m_map;
    CompoundComponentCollection*        m_component_collection;
    CLexicon*                           m_lexicon;

public:
    CompoundWordCollection();
    CompoundWordCollection(CLexicon* p_lexicon);
    ~CompoundWordCollection();
    CompoundWordCollection(const CompoundWordCollection& other);

    const QMap<word_t, CompoundWord*>& get_map() const { return m_map; }
    int                                 get_count() const { return m_map.size(); }
    CompoundComponentCollection*        get_components() const { return m_component_collection; }
    CompoundWord*                       add_compound_word(const word_t& word, const QStringList& composition);
    CompoundWord*                       add_compound_word(const word_t& whole, const QString& part0, const QString& part1);
    CompoundWord*                       get_compound_word(const word_t& word) const;
    void                                remove_compound_word(CompoundWord* p_word);
    void                                remove_invalid_components(QProgressBar* p_progressbar = NULL);

};

#endif // COMPOUND_H
