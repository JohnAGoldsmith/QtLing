#include "Suffix.h"

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

CPrefix::CPrefix(CPrefix& suffix)
{
    m_key = suffix.get_key();
    m_frequency = suffix.GetFrequency();
    m_sig_count = 0;
}
