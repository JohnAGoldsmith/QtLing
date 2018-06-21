#ifndef GOLDSTANDARD_H
#define GOLDSTANDARD_H

#include <QString>
#include <QList>
#include <QMap>
#include "Word.h"

class MainWindow;
class Lexicon;
class CWordCollection;

class GoldStandard
{
public:
    typedef QMap<QString, Parse_triple*> Parse_triple_map;
    typedef QMap<QString, Parse_triple_map*> GSMap;

protected:
    /* -- The variables below are reset by m_evaluation() -- */

    // the count of stem-affix parses that appear both in GS
    // and in result given by Lxa
    double      m_total_recall;
    double      m_total_precision;
    int         m_gs_word_count;
    int         m_overlap_word_count;
    // the count of words that are found both in GS and in
    // result given by Lxa
    int         m_true_positive_count;
    int         m_correct_count;
    int         m_retrieved_count;
    GSMap*      m_true_positive_parses;
    GSMap*      m_retrieved_parses;

    /* -- The variables above are reset by m_evaluation() -- */

    QString     m_XML_directory;
    QString     m_XML_file_name;

    GSMap*      m_GS;

    GSMap*      clone_GSMap(GSMap* map) const;
    void        delete_GSMap(GSMap* map);

    void        add_parse_triple(GSMap* map, const QString& word, const QString& stem, const QString& affix);
public:
    GoldStandard();
    GoldStandard(const GoldStandard& gs);
    explicit GoldStandard(QString& XML_file_name);
    GoldStandard& operator=(const GoldStandard& gs);
    ~GoldStandard();

    void        read_XML();
    bool        evaluate(CWordCollection* p_word_collection);

    double      get_total_recall() { return m_total_recall; }
    double      get_total_precision() { return m_total_precision; }
    int         get_gs_word_count() { return m_gs_word_count; }
    int         get_overlap_word_count() { return m_overlap_word_count; }
    int         get_true_positive_count() { return m_true_positive_count; }
    int         get_correct_count() { return m_correct_count; }
    int         get_retrieved_count() { return m_retrieved_count; }
    GSMap*      get_true_positive_parses() { return m_true_positive_parses; }
    GSMap*      get_retrieved_parses() { return m_retrieved_parses; }
    GSMap*      get_gold_standard_words() { return m_GS; }

};

#endif // GOLDSTANDARD_H
