#include "Lexicon.h"
#include <QJsonObject>

static bool CJsonInfo::check_tags(const QJsonObject& ref_json,
                                  const QList<QPair<QJsonValue::Type, QString>>& tags)
{
    QPair<QJsonValue::Type, QString> pair;
    foreach (pair, tags) {
        QJsonObject::iterator iter = ref_json.find(pair.second);
        if (iter = ref_json.end())
            return false;
        if (iter->type() != pair.first)
            return false;
    }
    return true;
}
