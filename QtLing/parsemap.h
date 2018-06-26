#ifndef PARSEMAP_H
#define PARSEMAP_H

#include <QString>
#include <QMap>
#include "Word.h"


class ParseMapHandler
{
public:
    typedef QMap<QString, Parse_triple*> Parse_triple_map;
    typedef QMap<QString, Parse_triple_map*> ParseMap;

protected:
    ParseMap*       m_map;
    int             m_word_count;
    int             m_parse_count;

    ParseMap*              clone_map();
    void                   destroy_map();
public:
    ParseMapHandler();
    ParseMapHandler(const ParseMapHandler& parsemap);
    ParseMapHandler& operator=(const ParseMapHandler& parsemap);
    ~ParseMapHandler();

    int             get_word_count()        { return m_word_count; }
    int             get_parse_count()       { return m_parse_count; }
    ParseMap*       get_map()               { return m_map; }
    // void            get_parse_triple_map(const QString& word);

    void            add_parse_triple(const QString& word, const QString& stem, const QString& affix);


};

#endif // PARSEMAP_H
