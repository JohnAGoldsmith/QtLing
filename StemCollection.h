#ifndef CSTEMCOLLECTION_H
#define CSTEMCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
#include "Stem.h"
#include "Typedefs.h"

class QJsonObject;
class CJsonInfo;
class CLexicon;


class CStemCollection
{
protected:
    //QMap<stem_t, CStem*>
    map_string_to_stem *        m_StringToStemMap;
    int                         m_CorpusCount;
    QString                     m_MemberName;
    QList<CStem*> *             m_stem_list;
    //QStringList                 m_SortedStringList;
    bool                        m_forward_sort_valid_flag;
    enum   eSortStyle           m_SortStyle;
    CLexicon*                   m_Lexicon;

public:
    CStemCollection(CLexicon*);
    ~CStemCollection();

// disable copy
private:
    CStemCollection(const CStemCollection& x);
    CStemCollection& operator=(const CStemCollection& x);

public:
    // Many functions below used to take the type `QString` as arguments.
    // Changed them to `const QString&` for possibly greater efficiency.
    // Changed made by Hanson 7.30

    CStem*                          add (const QString&);  // deprecated
    void                            add_stem_pointer (CStem* pStem);
    void                            clear();
    CStem*                          find_or_add (const QString&);  // same as <<
    CStem*                          find_or_fail (const QString&); // same as ^=
    CStem*                          get_from_string( QString);
    CStem*                          get_at(int index);
    QString                         get_string_at(int index);
    int                             get_count() const { return m_StringToStemMap->size(); }
    map_string_to_stem *            get_map() { return m_StringToStemMap;}         //       FIX THIS< DO IT RIGHT;
    QListIterator<CStem*> *         get_sorted_list_iterator();
    //QString                         get_string_from_sorted_list(int n) {return m_SortedStringList[n];}
    CStem*                          get_stem(int n) {return m_stem_list->at(n);}
    CStem*                          get_stem(QString stem){return (*m_StringToStemMap)[stem];}
    QList<CStem*> *                 get_stem_list() {return m_stem_list;}
    QStringList                     get_list_of_stem_strings();
    map_string_to_stem *            get_stem_map() { return m_StringToStemMap;}
    void                            sort_alphabetically();
    CStem*                          operator<< (const QString& );
    CStem*                          operator^= (const QString& );

    void                            assign_json_id();
    void                            write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED);
    void                            read_json_1(const QJsonObject& ref_json, CJsonInfo &ref_pointers);
    void                            read_json_2(const QJsonObject& ref_json, const CJsonInfo &ref_pointers);
};

#endif // CSTEMCOLLECTION_H
