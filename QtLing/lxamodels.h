#ifndef LXAMODELS_H
#define LXAMODELS_H
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include "generaldefinitions.h"
class CWordCollection;
class CStemCollection;
class CSuffixCollection;
class CHypothesis;


class LxaStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    LxaStandardItemModel();
    LxaStandardItemModel(QString shortname);
    ~LxaStandardItemModel();

private:
    QString                 m_ShortName;
    QString                 m_Description;
    eSortStyle              m_sort_style;
    CSignatureCollection*   m_Signatures;

public:

    void        sort(int column_no, Qt::SortOrder order);
    void        load_words(CWordCollection*);
    void        load_stems(CStemCollection * p_stems);
    void        load_suffixes(CSuffixCollection * p_suffixes);
    void        load_signatures(CSignatureCollection * p_signatures, eSortStyle = SIG_BY_STEM_COUNT);
    void        load_parasignatures(CSignatureCollection * p_signatures);
    void        sort_signatures(eSortStyle);
    void        load_sig_graph_edges(QMap<QString, sig_graph_edge*> *);
    void        load_passive_signatures (CSignatureCollection* p_passive_signatures);
    void        load_positive_signatures(CSignatureCollection* p_positive_signatures);
    void        load_positive_prefix_signatures(CSignatureCollection* p_positive_prefix_signatures);
    void        load_hypotheses(QList<CHypothesis*>*);
    void        load_hypotheses_2(QList<CHypothesis*>*);
// add component 11
    void        load_category(QString component_name, eComponentType);
};

class LxaSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT;
public:
    LxaSortFilterProxyModel(QObject*);
    void  sort() {QSortFilterProxyModel::sort(-1);}

private:
    bool lessThan(const QModelIndex & left, const QModelIndex & right) const override;

};


























#endif
