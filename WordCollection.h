#ifndef CWORDCOLLECTION_H
#define CWORDCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
#include <QMapIterator>
#include "Lexicon.h"


class CWord;


class CWordCollection
{


protected:

    QMap<QString, CWord*>   m_WordMap;
    QList<CWord*>           m_word_list;
    int                     m_CorpusCount;
    QString                 m_MemberName;
    QStringList             m_sorted_list;
    QStringList             m_end_sorted_list;
    CLexicon *              m_Lexicon;
    bool                    m_SortValidFlag;
    enum                    eSortStyle m_SortStyle;

    public:
    // construction/destruction.
    CWordCollection(CLexicon*);
    ~CWordCollection();

private:
    CWordCollection&        operator=(const CWordCollection& x);
    void                    sort_word_lists();

    const QStringList*      get_sorted_list() {return & m_sorted_list;}
    const QStringList*      get_end_sorted_list() {return & m_end_sorted_list;}

public:
    // Many functions below used to take the type `QString` as arguments.
    // Changed them to `const QString&` for possibly greater efficiency.
    // Changed made by Hanson 7.30
    friend class CLexicon;
    CWord*                  operator<< (const QString&);
    CWord*                  operator^= (const QString&);

    CWord*                  add_word (const QString&, int count= 1);
    bool                    contains(const QString& string)  {return m_WordMap.contains(string);}
    CWord*                  find_or_add (const QString&);
    CWord*                  find_or_fail(const QString& word_t);
    int                     get_count() const       { return m_WordMap.size(); }
    int                     get_token_count();
    QString                 get_string_from_sorted_list(int n) {return m_sorted_list[n];}
    QString                 get_string_from_end_sorted_list(int n){return m_end_sorted_list[n];}
    QList<CWord*> *         get_word_list() {return & m_word_list;}
    CWord*                  get_word(int n);
    CWord*                  get_at(int n) {get_word(n);}
    int                     get_word_count();
    const QMap<QString, CWord*> get_map() {return m_WordMap;}
    void                    input_words(QMap<QString, int> word_counts);
    void                    clear_all_words_parse_triple_maps();

    void                    assign_json_id();
    void                    write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED);

    // this should be replaced by find_or_fail:
    CWord*                  get_word (const QString&);     // returns false if the string is not in the word collection


    QMapIterator<QString,CWord*> *  get_iterator();
    QMap<word_t, CWord*>::iterator  begin()                    { return m_WordMap.begin();}
    QMap<word_t, CWord*>::iterator  end()                      { return m_WordMap.end();}
};

#endif // CWORDCOLLECTION_H


