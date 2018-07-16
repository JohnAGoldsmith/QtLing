#ifndef COMPOUND_H
#define COMPOUND_H

#include <QString>
#include <QList>
#include <QMap>
#include <QPair>

class CWord;
class CLexicon;
class CompoundComponent;
class CompoundWord;
class CompoundComponentCollection;
class CompoundWordCollection;

/*!
 * \brief The CompoundComponent class.
 *
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
    CompoundComponent(const QString& word);
    void                                        add_connection(CompoundWord* compword, int position);
    const CompoundConnectionMap&                get_connections() const { return m_connections; }
    const QString&                              get_word() const {return m_word;}
    bool                                        check_valid();
};

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
    CompoundWord(const QString& word);
    const QString &                             get_word() const { return m_word; }
    const QList<CompoundComposition*>&          get_compositions() const { return m_compositions; }
    //void                                        add_component(CompoundComponent* p_part);
    void                                        add_composition(const CompoundComposition&);
    bool                                        remove_composition_if_contains(CompoundComponent* p_comp);
    QString                                     composition_to_str(CompoundComposition* p_comp);

};

class CompoundComponentCollection
{
    QMap<QString, CompoundComponent*>   m_map;
    CompoundWordCollection*             m_word_collection;
    CLexicon*                           m_lexicon;
public:
    CompoundComponentCollection();
    CompoundComponentCollection(CompoundWordCollection* p_word_collection, CLexicon* p_lexicon);
    ~CompoundComponentCollection();

    QMap<QString, CompoundComponent*>&  get_map() { return m_map; }
    CompoundComponent*                  add_or_find_compound_component(const QString& str_word);
    void                                remove_component(CompoundComponent* p_component);
};

class CompoundWordCollection
{
    QMap<QString, CompoundWord*>        m_map;
    CompoundComponentCollection*        m_component_collection;
    CLexicon*                           m_lexicon;

public:
    CompoundWordCollection();
    CompoundWordCollection(CLexicon* p_lexicon);
    ~CompoundWordCollection();
    CompoundWordCollection(const CompoundWordCollection& other);

    const QMap<QString, CompoundWord*>& get_map() const { return m_map; }
    int                                 get_count() const { return m_map.size(); }
    CompoundComponentCollection*        get_components() const { return m_component_collection; }
    CompoundWord*                       add_compound_word(const QString& word, const QStringList& composition);
    CompoundWord*                       add_compound_word(const QString& whole, const QString& part0, const QString& part1);
    CompoundWord*                       get_compound_word(const QString& word) const;
    void                                remove_compound_word(CompoundWord* p_word);

};

#endif // COMPOUND_H
