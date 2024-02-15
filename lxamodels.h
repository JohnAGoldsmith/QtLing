#ifndef LXAMODELS_H
#define LXAMODELS_H
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
//#include <QAbstractItemModel>    // to do Dec 2023
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
class SigPairCollection;

class SigPairModel: public QAbstractTableModel{
    Q_OBJECT

    SigPairCollection * sig_pairs;

public:
    SigPairModel(SigPairCollection*);
    ~SigPairModel();
    int rowCount (const QModelIndex &parent = QModelIndex() )const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data  (const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
};


class sigpairproxymodel: public QSortFilterProxyModel{
    Q_OBJECT

public:
    sigpairproxymodel(QObject*);
    ~sigpairproxymodel(){};

private:
    bool lessThan(const QModelIndex & left, const QModelIndex & right) const override;

};



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


    void        load_words(CWordCollection*);
    void        load_stems(CStemCollection * p_stems);
    void        load_prefixes(CPrefixCollection * p_prefixes);
    void        load_suffixes(CSuffixCollection * p_suffixes);
    void        load_signatures(CSignatureCollection * p_signatures, eSortStyle = SIG_BY_STEM_COUNT);
    void        load_parasignatures(CSignatureCollection * p_signatures);
    void        sort_signatures(eSortStyle);
    void        load_sig_graph_edges(QMap<QString, sig_pair*> *, int size);
    void        load_passive_signatures (CSignatureCollection* p_passive_signatures);
    void        load_positive_signatures(CSignatureCollection* p_positive_signatures,  eSortStyle = SIG_BY_STEM_COUNT);
    void        load_positive_prefix_signatures(CSignatureCollection* p_positive_prefix_signatures);
    void        load_hypotheses(QList<CHypothesis*>*);
    void        load_hypotheses_2(QList<CHypothesis*>*);

    // add component 11
    void        load_category(QString component_name, eComponentType);
    void        load_parses(QMap<QString, CParse*>* );

    // for displaying protostems
    void        load_protostems(QMap<QString, protostem*>* p_protostems);
    void        load_compounds(CompoundWordCollection* p_compounds);

    void        load_parsemap_from_gs(GoldStandard* p_gs, ParseMapHandler parsemap, const QString &type);
    void        load_parasuffixes(QMap<QString,QStringList*>* p_parasuffixes);
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
