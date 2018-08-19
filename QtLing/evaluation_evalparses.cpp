#include "evaluation.h"
#include <QTextStream>
#include <QRegularExpression>

EvalParses::EvalParses(): m_parses() { }

EvalParses::EvalParses(QString& file_name):
    m_parses(), m_file_name(file_name) { }

EvalParses::EvalParses(const EvalParses& eval)
{
    m_parses = eval.m_parses;
    m_results = eval.m_results;
    m_file_name = eval.m_file_name;
    m_evaluated = eval.m_evaluated;
}

EvalParses& EvalParses::operator=(const EvalParses& eval)
{
    if (&eval != this) {
        m_parses = eval.m_parses;
        m_results = eval.m_results;
        m_file_name = eval.m_file_name;
        m_evaluated = eval.m_evaluated;
    }
    return *this;
}

EvalParses::~EvalParses()
{ }

bool EvalParses::evaluate(GoldStandard *p_gs)
{
    return p_gs->evaluate(this);
}

bool EvalParses::read_morfessor_txt_file()
{
    if(m_file_name.isEmpty())
        return false;

    m_parses = ParseMapHandler();
    QFile parse_file(m_file_name);

    if (!parse_file.open(QIODevice::ReadOnly))
        return false;

    QTextStream parse_input(&parse_file);

    int word_count = 0;

    while (!parse_input.atEnd()) {
        QString line = parse_input.readLine();
        if (line[0] == "#")
            continue;
        word_count++;
        line.remove(QRegularExpression("\\/[A-Z]+"));
        QStringList tokens = line.split(QRegularExpression("\\s\\+\\s|1\\s"));
        word_t str_word = tokens.join("");
        int word_len = str_word.length();
        int cut_position = tokens[0].length();
        int list_len = tokens.length();
        for (int i = 1; i < list_len; i++) {
            stem_t stem = str_word.left(cut_position);
            affix_t suffix = str_word.right(word_len - cut_position);
            cut_position += tokens[i].length();
            m_parses.add_parse_triple(str_word, stem, suffix);
        }
    }
    m_results.set_retrieved_word_count(word_count);
    return true;
}
