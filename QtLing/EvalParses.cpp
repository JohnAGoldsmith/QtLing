#include "EvalParses.h"
#include "goldstandard.h"
#include <QTextStream>
#include <QRegularExpression>

EvalParses::EvalParses(): m_parses(NULL)
{
    m_parses = new GoldStandard::GSMap();
}

EvalParses::EvalParses(QString& file_name):
    m_file_name(file_name)
{
    m_parses = new GoldStandard::GSMap();
}

EvalParses::EvalParses(const EvalParses& eval)
{
    m_parses = clone_GSMap(eval.m_parses);
}

EvalParses& EvalParses::operator=(const EvalParses& eval)
{
    if (&eval != this) {
        delete_GSMap(m_parses);
        if (eval.m_parses)
            m_parses = clone_GSMap(eval.m_parses);
        else
            m_parses = NULL;

        m_word_count = eval.m_word_count;
        m_true_positive_count = eval.m_true_positive_count;
        m_correct_count = eval.m_correct_count;
        m_retrieved_count = eval.m_retrieved_count;
        m_overlap_word_count = eval.m_overlap_word_count;

        m_total_precision = eval.m_total_precision;
        m_total_recall = eval.m_total_recall;

        m_file_name = eval.m_file_name;

        m_evaluated = eval.m_evaluated;
    }
    return *this;
}

EvalParses::~EvalParses()
{
    delete_GSMap(m_parses);
}

bool EvalParses::evaluate(GoldStandard *p_gs)
{
    return p_gs->evaluate(this);
}

bool EvalParses::read_morfessor_txt_file()
{
    if(m_file_name.isEmpty())
        return false;

    m_parses = new GoldStandard::GSMap();
    QFile parse_file(m_file_name);

    if (!parse_file.open(QIODevice::ReadOnly))
        return false;

    QTextStream parse_input(&parse_file);

    m_word_count = 0;

    while (!parse_input.atEnd()) {
        QString line = parse_input.readLine();
        if (line[0] == "#")
            continue;
        m_word_count++;
        QStringList tokens = line.split(QRegularExpression("\\s\\+\\s|1\\s"));
        QString str_word = tokens.join("");
        int word_len = str_word.length();
        int cut_position = tokens[0].length();
        int list_len = tokens.length();
        for (int i = 1; i < list_len; i++) {
            QString stem = str_word.left(cut_position);
            QString suffix = str_word.right(word_len - cut_position);
            cut_position += tokens[i].length();
            add_parse_triple(m_parses, str_word, stem, suffix);
        }
    }
    return true;
}
