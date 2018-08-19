#include "evaluation.h"

ParseMapHandler::ParseMapHandler():
    m_map(new Word_to_parse_triple_collection_map()) { }

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
 *
 * Modified by Hanson to use foreach syntax. 8.1
 * Currently I am not sure whether it is necessary to define wrapper classes
 * around the two data structures you mentioned, with copy constructors for each one,
 * as ParseMapHandler is already a wrapper around Word_to_parse_triple_collection_map
 * (originally ParseMap).
 */
ParseMapHandler::Word_to_parse_triple_collection_map *ParseMapHandler::clone_map() const
{
    if (m_map == NULL)
        return NULL;
    Word_to_parse_triple_collection_map *newmap = new Word_to_parse_triple_collection_map();
    foreach (word_t this_word, m_map->keys()) {
        Parse_triple_collection *newptm = new Parse_triple_collection();
        Parse_triple_collection *curr_ptm = m_map->value(this_word);
        foreach(stem_t this_stem, curr_ptm->keys()) {
            Parse_triple *newpt = new Parse_triple(this_stem,
                                                   curr_ptm->value(this_stem)->m_suffix,
                                                   QString());
            newptm->insert(this_stem, newpt);
        }
        newmap->insert(this_word, newptm);
    }
    return newmap;
}

/*ParseMapHandler::Word_to_parse_triple_collection_map *ParseMapHandler::clone_map() const
{
    if (m_map == NULL)
        return NULL;
    // *** JG August 2018 I am not sure which is Hanson's latest code for this function.
    ParseMap *newmap = new ParseMap();
    for (ParseMap::const_iterator gsIter = m_map->constBegin();
         gsIter != m_map->constEnd(); gsIter++) {
        Parse_triple_map *newptm = new Parse_triple_map();
        Parse_triple_map currptm = *(gsIter.value());
        for (Parse_triple_map::const_iterator ptmIter = currptm.constBegin();
             ptmIter != currptm.constEnd(); ptmIter++) {
            Parse_triple *newpt = new Parse_triple(ptmIter.key(), ptmIter.value()->m_suffix, QString());
            newptm->insert(ptmIter.key(), newpt);
         }
        newmap->insert(this_word, newptm);
    }
    return newmap;
}
*/


//copy constructor, 2 of them:

// Make ParseMap a class, with a copy constructor, and with a member variable m_parsemap
/*
ParseMap(const ParseMap &parsemap) {

    Parse_triple_map * current_map;
    Parse_triple_map * newptm;
    foreach(stem_t this_stem, *m_map->keys()) {
          newptm = new Parse_triple_map(current_map);
          m_parsemap[this_stem] = newptm;
    }
}

//make Parse_triple_map a class or a struct which likewise has a memberr variable m_map (find better name):
Parse_triple_map(const Parse_triple_map & this_parse_triple_map){
    foreach (word_t this_word, this_parse_triple_map.keys()){
        Parse_triple * old_parse_triple = this_parse_triple_map[this_word];
        Parse_triple * new_parse_triple = new Parse_triple(this_word, old_parse_triple->m_suffix, QString() );
        m_map[this_word] = new_parse_triple;
    }

}
*/

/*!
 * \brief Deletes all the content stored in a ParseMap.
 *
 * Changed to foreach -- Hanson 8.1
 */
void ParseMapHandler::destroy_map()
{
    if (m_map == NULL)
        return;
    foreach (Parse_triple_collection* this_triple_map, *m_map) {
        foreach (Parse_triple* this_triple, *this_triple_map) {
            delete this_triple;
        }
        delete this_triple_map;
    }
    delete m_map;
}

/*      Hanson, I think the following is clearer:
void ParseMapHandler::destroy_map()
{
    if (m_map == NULL)
        return;
    foreach(Parse_triple_map* this_triple_map, *m_map){
        foreach(Parse_triple* this_triple, *this_triple_map) {
            delete this_triple;
        }
        delete this_triple_map;
    }
    delete m_map;
}
*/
void ParseMapHandler::add_parse_triple(const word_t& word, const stem_t& stem, const affix_t& affix)
{
    Word_to_parse_triple_collection_map::iterator word_iter = m_map->find(word);
    if (word_iter == m_map->end()) {
        Parse_triple_collection* this_ptm = new Parse_triple_collection();
        Parse_triple* this_triple = new Parse_triple(stem, affix, QString());
        this_ptm->insert(stem, this_triple);
        m_map->insert(word, this_ptm);
    } else {
        Parse_triple* this_triple = new Parse_triple(stem, affix, QString());
        word_iter.value()->insert(stem, this_triple);
    }
}
