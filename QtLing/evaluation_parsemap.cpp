#include "evaluation.h"

ParseMapHandler::ParseMapHandler():
    m_map(new ParseMap()) { }

ParseMapHandler::ParseMapHandler(const ParseMapHandler &parsemap):
    m_map(parsemap.clone_map()) { }

ParseMapHandler& ParseMapHandler::operator=(const ParseMapHandler& parsemap)
{
    if (&parsemap != this) {
        destroy_map();
        if (parsemap.m_map != NULL)
            m_map = parsemap.clone_map();
        else
            m_map = NULL;
    }
    return *this;
}

ParseMapHandler::~ParseMapHandler()
{
    destroy_map();
}

/*!
 * \brief Makes a deep copy of all the contents stored within a ParseMap structure,
 * including all Parse_triple_maps and Parse_triples
 *
 * \return Pointer to a newly copied ParseMap
 */
ParseMapHandler::ParseMap *ParseMapHandler::clone_map() const
{
    if (m_map == NULL)
        return NULL;
    ParseMap *newmap = new ParseMap();
    for (ParseMap::const_iterator gsIter = m_map->constBegin();
         gsIter != m_map->constEnd(); gsIter++) {
        Parse_triple_map *newptm = new Parse_triple_map();
        Parse_triple_map currptm = *(gsIter.value());
        for (Parse_triple_map::const_iterator ptmIter = currptm.constBegin();
             ptmIter != currptm.constEnd(); ptmIter++) {
            Parse_triple *newpt = new Parse_triple(ptmIter.key(), ptmIter.value()->p_suffix, QString());
            newptm->insert(ptmIter.key(), newpt);
        }
        newmap->insert(gsIter.key(),newptm);
    }
    return newmap;
}

/*!
 * \brief Deletes all the content stored in a ParseMap.
 */
void ParseMapHandler::destroy_map()
{
    if (m_map == NULL)
        return;
    for (ParseMap::iterator gsIter = m_map->begin();
         gsIter != m_map->end(); gsIter++) {
        Parse_triple_map ptm = *(gsIter.value());
        for (Parse_triple_map::iterator ptmIter = ptm.begin();
             ptmIter != ptm.end(); ptmIter++) {
            delete ptmIter.value();
        }
        delete gsIter.value();
    }
    delete m_map;
}

void ParseMapHandler::add_parse_triple(const QString& word, const QString& stem, const QString& affix)
{
    ParseMap::iterator word_iter = m_map->find(word);
    if (word_iter == m_map->end()) {
        Parse_triple_map* this_ptm = new Parse_triple_map();
        Parse_triple* this_triple = new Parse_triple(stem, affix, QString());
        this_ptm->insert(stem, this_triple);
        m_map->insert(word, this_ptm);
    } else {
        Parse_triple* this_triple = new Parse_triple(stem, affix, QString());
        word_iter.value()->insert(stem, this_triple);
    }
}
