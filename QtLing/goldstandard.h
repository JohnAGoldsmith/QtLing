#ifndef GOLDSTANDARD_H
#define GOLDSTANDARD_H

#include <QString>
#include <QList>
#include <QMap>
#include "Word.h"
#include "parsemap.h"

class MainWindow;
class Lexicon;
class CWordCollection;
class EvalParses;

class GoldStandard
{
friend class EvalParses;
public:
    typedef QMap<QString, Parse_triple*> Parse_triple_map;
    typedef QMap<QString, Parse_triple_map*> ParseMap;

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
    ParseMapHandler m_true_positive_parses;
    ParseMapHandler m_retrieved_parses;

    /* -- The variables above are reset by m_evaluation() -- */

    QString     m_XML_directory;
    QString     m_XML_file_name;

    // ParseMap*      m_GS;
    ParseMapHandler m_gs_parses;

public:
    GoldStandard();
    GoldStandard(const GoldStandard& gs);
    explicit GoldStandard(QString& XML_file_name);
    GoldStandard& operator=(const GoldStandard& gs);
    ~GoldStandard();

    bool                read_XML();
    bool                evaluate(CWordCollection* p_word_collection);
    bool                evaluate(EvalParses* p_eval_parses);

    double              get_total_recall() { return m_total_recall; }
    double              get_total_precision() { return m_total_precision; }
    int                 get_gs_word_count() { return m_gs_word_count; }
    int                 get_overlap_word_count() { return m_overlap_word_count; }
    int                 get_true_positive_count() { return m_true_positive_count; }
    int                 get_correct_count() { return m_correct_count; }
    int                 get_retrieved_count() { return m_retrieved_count; }
    ParseMapHandler     get_true_positive_parses() { return m_true_positive_parses; }
    ParseMapHandler     get_retrieved_parses() { return m_retrieved_parses; }
    ParseMapHandler     get_gs_parses() { return m_gs_parses; }

};

// non-member functions


#endif // GOLDSTANDARD_H
