#include "Suffix.h"

CSuffix::CSuffix(QString  suffix): m_key(suffix)
{}

CSuffix::CSuffix(CSuffix& suffix)
{
    m_key = suffix.get_key();
    m_frequency = suffix.GetFrequency();
}
