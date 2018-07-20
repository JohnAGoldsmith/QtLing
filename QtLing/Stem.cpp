#include "Lexicon.h"
#include "Stem.h"
#include "Suffix.h"
#include "Signature.h"
#include <QJsonArray>

CStem::CStem(QString stem) :  m_key(stem), m_Signatures(),m_count(0) {}

CStem::CStem(CStem& stem) {
    m_key = stem.get_key();
    m_count = stem.get_count();
    QListIterator<CSignature*> sig_iter(*stem.GetSignatures());
    while (sig_iter.hasNext()){
        m_Signatures.append(sig_iter.next());
    }

}

QString CStem::display(){
    return m_key;
}

void CStem::write_json(QJsonObject &ref_json) const
{
    ref_json["stem"] = m_key;
    ref_json["count"] = m_count;
    ref_json["wordListStartPoint"] = m_word_list_start_point;
    ref_json["wordListEndPoint"] = m_word_list_end_point;
    QJsonArray arr_signatures;
    CSignature* p_sig;

    foreach (p_sig, m_Signatures) {
        QJsonObject obj_sig;
        obj_sig["signature"] = p_sig->get_key();
        obj_sig["id"] = p_sig->get_json_id();
        arr_signatures.append(obj_sig);
    }
    ref_json["signatures"] = arr_signatures;

    QJsonArray arr_parasignature;
    CSuffix* p_suffix;
    foreach (p_suffix, m_parasignature) {
        QJsonObject obj_affix;
        obj_affix["affix"] = p_suffix->get_key();
        obj_affix["id"] = p_suffix->get_json_id();
        arr_parasignature.append(obj_affix);
    }
    ref_json["parasignature"] = arr_parasignature;

    QJsonArray arr_autobiography;
    foreach (auto string, m_Autobiography) {
        arr_autobiography.append(string);
    }
    ref_json["autobiography"] = arr_autobiography;
}

void CStem::read_json_1(const QJsonObject &ref_json)
{
    m_key = ref_json["stem"].toString();
    m_count = ref_json["count"].toInt();
    m_word_list_start_point = ref_json["wordListStartPoint"].toInt();
    m_word_list_end_point = ref_json["wordListEndPoint"].toInt();
    QJsonArray::ConstIterator iter;
    const QJsonArray& arr_autobiography = ref_json["autobiography"].toArray();
    for (iter = arr_autobiography.constBegin();
         iter != arr_autobiography.constEnd();
         iter++) {
        m_Autobiography.append(iter->toString());
    }
}

void CStem::read_json_2(const QJsonObject &ref_json, const CJsonInfo &ref_pointers)
{
    if (ref_pointers.suffix_flag) {
        QJsonArray::ConstIterator iter;
        const QJsonArray& arr_sigs = ref_json["signatures"];
        for (iter = arr_sigs.constBegin(); iter != arr_sigs.constEnd(); iter++) {
            const QJsonObject& obj_sig = iter->toObject();
            CSignature* p_sig = ref_pointers.suffix_signatures[obj_sig["id"].toInt()];
            add_signature(p_sig);
        }
        const QJsonArray& arr_parasig = ref_json["parasignature"];
        for (iter = arr_parasig.constBegin();
             iter != arr_parasig.constEnd();
             iter++) {
            const QJsonObject& obj_suf = iter->toObject();
            CSuffix* p_suf = ref_pointers.suffixes[obj_suf["id"].toInt()];
            m_parasignature.append(p_suf);
        }
    } else {
        QJsonArray::ConstIterator iter;
        const QJsonArray& arr_sigs = ref_json["signatures"];
        for (iter = arr_sigs.constBegin(); iter != arr_sigs.constEnd(); iter++) {
            const QJsonObject& obj_sig = iter->toObject();
            CSignature* p_sig = ref_pointers.prefix_signatures[obj_sig["id"].toInt()];
            add_signature(p_sig);
        }
        m_parasignature.clear();
    }
}
