#include "evaluation.h"


EvaluationResults::EvaluationResults()
{
    clear();
}

/*!
 * \brief Calculates precision and recall.
 *
 * Replaces the values of m_total_recall and m_total_precision by new values.
 */
void EvaluationResults::calculate_precision_recall()
{
    m_total_precision = (double) m_true_positive_count / (double) m_retrieved_parse_count;
    m_total_recall = (double) m_true_positive_count / (double) m_gs_parse_count;
}

/*!
 * \brief Resets values to 0 (of -1, if that value may be a denominator).
 */
void EvaluationResults::clear()
{
    m_total_recall = 0;
    m_total_precision = 0;
    m_gs_word_count = 0;
    m_retrieved_word_count = 0;
    m_overlap_word_count = 0;
    m_true_positive_count = 0;
    m_gs_parse_count = -1;
    m_retrieved_parse_count = -1;
}
