#ifndef EVAL_PARSES_H
#define EVAL_PARSES_H

#include "goldstandard.h"
#include "Lexicon.h"

class GoldStandard;

class EvalParses
{
    friend class GoldStandard;
public:
    typedef QMap<QString, Parse_triple*> Parse_triple_map;
    typedef QMap<QString, Parse_triple_map*> ParseMap;
protected:
    ParseMapHandler         m_parses;
    int                     m_word_count;
    int                     m_true_positive_count;
    int                     m_correct_count;
    int                     m_retrieved_count;
    int                     m_overlap_word_count;

    double                  m_total_precision;
    double                  m_total_recall;

    QString                 m_file_name;

    bool                    m_evaluated;


public:
    EvalParses();
    explicit EvalParses(QString& file_name);
    EvalParses(const EvalParses& eval);
    EvalParses& operator=(const EvalParses& eval);
    ~EvalParses();


    ParseMapHandler         get_parses()                {return m_parses;}
    int                     get_word_count()            {return m_word_count;}
    int                     get_true_positive_count()   {return m_true_positive_count;}
    int                     get_retrieved_count()       {return m_retrieved_count;}
    int                     get_correct_count()         {return m_correct_count;}
    int                     get_overlap_word_count()    {return m_overlap_word_count;}

    double                  get_total_precision()       {return m_total_precision;}
    double                  get_total_recall()          {return m_total_recall;}

    bool                    evaluate(GoldStandard* p_gs);

    bool                    is_evaluated()              {return m_evaluated;}

    bool                    read_morfessor_txt_file();



};

#endif // EVAL_PARSES_H
