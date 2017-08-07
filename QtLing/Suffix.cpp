#include "Suffix.h"

CSuffix::CSuffix(CStringSurrogate ssWord) : CParse(ssWord), m_Suffix(ssWord.GetKey()), m_frequency(0)
{}

CSuffix::CSuffix(CSuffix& suffix)
{
    m_Suffix = suffix.GetSuffix();
    m_frequency = suffix.GetFrequency();
}
