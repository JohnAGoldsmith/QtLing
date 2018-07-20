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

void CSuffix::write_json(QJsonObject &ref_json) const
{
    ref_json["suffix"] = m_key;
    ref_json["frequency"] = m_frequency;
    ref_json["count"] = m_count;
    ref_json["id"] = m_json_id;
}


void CSuffix::read_json(const QJsonObject &ref_json)
{
    m_key = ref_json["suffix"].toString();
    m_frequency = ref_json["frequency"].toInt();
    m_count = ref_json["count"].toInt();
    m_json_id = ref_json["id"].toInt();
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

void CPrefix::write_json(QJsonObject &ref_json) const
{
    ref_json["prefix"] = m_key;
    ref_json["frequency"] = m_frequency;
    ref_json["count"] = m_count;
    ref_json["id"] = m_json_id;
}

void CPrefix::read_json(const QJsonObject &ref_json)
{
    m_key = ref_json["prefix"].toString();
    m_frequency = ref_json["frequency"].toInt();
    m_count = ref_json["count"].toInt();
    m_json_id = ref_json["id"].toInt();
}
