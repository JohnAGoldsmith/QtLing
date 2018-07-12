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

class CompoundComponent
{
public:
    typedef QMap<QString, QPair<int, CompoundWord*>> CompoundConnectionMap;
protected:
    friend class CompoundComponentCollection;
    QString                                     m_word;
    QMap<QString, QPair<int, CompoundWord*>>    m_connections;
    CWord*                                      m_cword_ptr;

public:
    CompoundComponent();
    CompoundComponent(QString& word, CWord* p_cword);
    void                                        add_connection(CompoundWord* compword, int position);
    const CompoundConnectionMap&                get_connections() const { return m_connections; }
    CWord*                                      get_cword_ptr() const { return m_cword_ptr; }
    const QString&                              get_word() const {return m_word;}
};

class CompoundWord
{
public:
    typedef QList<CompoundComponent*>           CompoundComposition;
protected:
    friend class CompoundWordCollection;
    QString                                     m_word;
    QList<CompoundComposition*>                 m_compositions;
    CWord*                                      m_cword_ptr;

public:
    CompoundWord();
    ~CompoundWord();
    CompoundWord(QString& word, CWord* p_cword);
    CompoundWord(CWord* p_cword);
    QString                                     get_word() { return m_word; }
    const QList<QList<CompoundComponent*>*>&    get_compositions() const { return m_compositions; }
    //void                                        add_component(CompoundComponent* p_part);
    void                                        add_composition(const CompoundComposition&);
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

    CompoundComponent*                  add_or_find_compound_component(CWord* p_word);
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
    CompoundWord*                       add_compound_word(CWord* word, QList<CWord*>& composition);
    CompoundWord*                       add_compound_word(CWord* whole, CWord* part0, CWord* part1);
    CompoundWord*                       get_compound_word(const QString& word) const;

};

#endif // COMPOUND_H
