#include "Suffix.h"

CSuffix::CSuffix(QString  suffix): m_key(suffix)
{m_count = 0;
}

CSuffix::CSuffix(CSuffix& suffix)
{
    m_key = suffix.get_key();
    m_frequency = suffix.GetFrequency();
    m_count = 0;
}
