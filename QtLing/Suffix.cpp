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

void CSuffix::to_json(QJsonObject &ref_json)
{
    ref_json["suffix"] = m_key;
    ref_json["frequency"] = m_frequency;
    ref_json["count"] = m_count;
    ref_json["id"] = m_json_id;
}

CPrefix::CPrefix(QString  suffix): m_key(suffix)
{m_count = 0;
}

CPrefix::CPrefix(CPrefix& suffix)
{
    m_key = suffix.get_key();
    m_frequency = suffix.GetFrequency();
    m_count = 0;
}

void CPrefix::to_json(QJsonObject &ref_json)
{
    ref_json["prefix"] = m_key;
    ref_json["frequency"] = m_frequency;
    ref_json["count"] = m_count;
    ref_json["id"] = m_json_id;
}
