#include "Suffix.h"
#include "Lexicon.h"
#include <QJsonValue>
#include <QDebug>

CSuffix::CSuffix(QString  suffix): m_key(suffix)
{m_sig_count = 0;
}

CSuffix::CSuffix(CSuffix& suffix)
{
    m_key = suffix.get_key();
    m_frequency = suffix.GetFrequency();
    m_sig_count = 0;
}

CPrefix::CPrefix(QString  suffix): m_key(suffix)
{m_sig_count = 0;
}

CPrefix::CPrefix(CPrefix& prefix)
{
    m_key = prefix.get_key();
    m_frequency = prefix.GetFrequency();
    m_sig_count = 0;
}
