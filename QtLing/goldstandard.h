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

    int         m_true_positives;
    // the count of stem-affix parses that appear both in GS
    // and in result given by Lxa
    double      m_total_recall;
    double      m_total_precision;
    int         m_gs_word_count;
    int         m_overlap_word_count;
    // the count of words that are found both in GS and in
    // result given by Lxa

    /* -- The variables above are reset by m_evaluation() -- */

    GSMap*      m_GS;

    QString     m_XML_directory;
    QString     m_XML_file_name;

    GSMap*      m_clone_GS_map() const;
    void        m_delete_GS_map();

    void        add_parse_triple(QString& word, QString& stem, QString& affix);
public:

    GoldStandard();
    GoldStandard(const GoldStandard& gs);
    GoldStandard& operator=(const GoldStandard& gs);
    ~GoldStandard();

    bool        m_openXML(MainWindow* p_main_window);
    void        m_parseXML();
    bool        m_evaluate(CWordCollection* p_word_collection);

    double      get_total_recall() { return m_total_recall; }
    double      get_total_precision() { return m_total_precision; }
    double      get_gs_word_count() { return m_gs_word_count; }
    double      get_overlap_word_count() { return m_overlap_word_count; }

};

#endif // GOLDSTANDARD_H
