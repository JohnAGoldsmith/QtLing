#ifndef LXAMODELS_H
#define LXAMODELS_H
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include "generaldefinitions.h"
#include "evaluation.h"
class CWordCollection;
class CStemCollection;
class CSuffixCollection;
class CPrefixCollection;
class CSignatureCollection;
class CHypothesis;
class MainWindow;
class sig_graph_edge;
class protostem;
class CompoundWordCollection;

class LxaStandardItemModel : public QStandardItemModel
{
    Q_OBJECT

public:
    LxaStandardItemModel(MainWindow* parent);
    LxaStandardItemModel(QString shortname);
    ~LxaStandardItemModel();

private:
    QString                 m_ShortName;
    QString                 m_Description;
    eSortStyle              m_sort_style;
    CSignatureCollection*   m_Signatures;

public:

    //void        sort(int column_no, Qt::SortOrder order);
    void        load_words(CWordCollection*);
    void        load_stems(CStemCollection * p_stems);
    void        load_prefixes(CPrefixCollection * p_prefixes);
    void        load_suffixes(CSuffixCollection * p_suffixes);
    void        load_signatures(CSignatureCollection * p_signatures, eSortStyle = SIG_BY_STEM_COUNT);
    void        load_parasignatures(CSignatureCollection * p_signatures);
    void        sort_signatures(eSortStyle);
    void        load_sig_graph_edges(QMap<QString, sig_graph_edge*> *, int size);
    void        load_passive_signatures (CSignatureCollection* p_passive_signatures);
    void        load_positive_signatures(CSignatureCollection* p_positive_signatures,  eSortStyle = SIG_BY_STEM_COUNT);
    void        load_positive_prefix_signatures(CSignatureCollection* p_positive_prefix_signatures);
    void        load_hypotheses(QList<CHypothesis*>*);
    void        load_hypotheses_2(QList<CHypothesis*>*);
// add component 11
    void        load_category(QString component_name, eComponentType);

    // for displaying protostems
    void        load_protostems(QMap<QString, protostem*>* p_protostems);
    void        load_compounds(CompoundWordCollection* p_compounds);

    void        load_parsemap_from_gs(GoldStandard* p_gs, ParseMapHandler parsemap, const QString &type);
};


class LxaSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    LxaSortFilterProxyModel(QObject*);
    //void  sort(int, Qt::SortOrder order = Qt::AscendingOrder) override {QSortFilterProxyModel::sort(-1, order);}

private:
    bool lessThan(const QModelIndex & left, const QModelIndex & right) const override;
    // function to support sorting by signature affix count

};


























#endif
