#ifndef GOLDSTANDARD_H
#define GOLDSTANDARD_H

#include <QString>
#include <QList>
#include <QMap>
#include "Word.h"

class MainWindow;
class Lexicon;
class CWordCollection;

class GoldStandard
{
public:
    typedef QMap<QString, Parse_triple*> Parse_triple_map;
    typedef QMap<QString, Parse_triple_map*> GSMap;

protected:
    GSMap*      m_GS;

    double      m_total_recall;
    double      m_total_precision;
    QString     m_XML_directory;
    QString     m_XML_file_name;

    GSMap*      m_clone_GS_map() const;
    void        m_delete_GS_map();
public:

    GoldStandard();
    GoldStandard(const GoldStandard& gs);
    GoldStandard& operator=(const GoldStandard& gs);
    ~GoldStandard();

    void        add_parse_triple(QString& word, QString& stem, QString& affix);
    void        m_openXML(MainWindow* p_main_window);
    void        m_parseXML();
    void        m_evaluate(CWordCollection* p_word_collection);

};

#endif // GOLDSTANDARD_H
