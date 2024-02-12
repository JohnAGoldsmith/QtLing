/* I moved any functions related to the treeview in the main window to here
 * because more and more things got added to the treeview and lxamodels.cpp
 * was getting too long.
 *
 * Comment added by Hanson 8.1
 */

#include "mainwindow.h"
#include "WordCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "compound.h"

#include <QDebug>

void remove_item_from_tree(const QString name, QStandardItem* item)
{
    for (int row_i = 0; row_i < item->rowCount(); ) {
        QString data = item->child(row_i, 0)->data(Qt::DisplayRole).toString();
        if (data == name) {
            item->removeRow(row_i);
        } else {
            row_i++;
        }
    }
}

void MainWindow::append_eval_results(EvaluationResults& results, QStandardItem* parent_item)
{
    typedef QStandardItem QSI;
    QList<QSI*> word_items;
    QSI* word_item = new QSI(QString("Gold Standard Words"));
    int gs_word_count = results.get_gs_word_count();
    QSI* word_count_item = new QSI(QString::number(gs_word_count));
    word_items.append(word_item);
    word_items.append(word_count_item);

    QList<QSI*> retrieved_word_items;
    QSI* retrieved_word_item = new QSI(QString("Retrieved Words"));
    int retrieved_word_count = results.get_retrieved_word_count();
    QSI* retrieved_word_count_item = new QSI(QString::number(retrieved_word_count));
    retrieved_word_items.append(retrieved_word_item);
    retrieved_word_items.append(retrieved_word_count_item);

    QList<QSI*> precision_items;
    QSI* precision_item = new QSI(QString("Precision"));
    double precision = results.get_total_precision();
    QSI* precision_value_item = new QSI(QString::number(precision));
    precision_items.append(precision_item);
    precision_items.append(precision_value_item);

    QList<QSI*> recall_items;
    QSI* recall_item = new QSI(QString("Recall"));
    double recall = results.get_total_recall();
    QSI* recall_value_item = new QSI(QString::number(recall));
    recall_items.append(recall_item);
    recall_items.append(recall_value_item);

    QList<QSI*> true_positive_items;
    QSI* true_positive_item = new QSI(QString("True Positive Parses"));
    int true_positive_count = results.get_true_positive_count();
    QSI* true_positive_count_item = new QSI(QString::number(true_positive_count));
    true_positive_items.append(true_positive_item);
    true_positive_items.append(true_positive_count_item);

    QList<QSI*> correct_items;
    QSI* correct_item = new QSI(QString("Gold Standard Parses"));
    int correct_count = results.get_gs_parse_count();
    QSI* correct_count_item = new QSI(QString::number(correct_count));
    correct_items.append(correct_item);
    correct_items.append(correct_count_item);

    QList<QSI*> retrieved_items;
    QSI* retrieved_item = new QSI(QString("Retrieved Parses"));
    int retrieved_count = results.get_retrieved_parse_count();
    QSI* retrieved_count_item = new QSI(QString::number(retrieved_count));
    retrieved_items.append(retrieved_item);
    retrieved_items.append(retrieved_count_item);

    parent_item->appendRow(word_items);
    parent_item->appendRow(retrieved_word_items);
    parent_item->appendRow(precision_items);
    parent_item->appendRow(recall_items);
    parent_item->appendRow(true_positive_items);
    parent_item->appendRow(correct_items);
    parent_item->appendRow(retrieved_items);
}

void MainWindow::update_TreeModel_for_gs(CLexicon* lexicon)
{
    typedef QStandardItem QSI;
    QSI* parent = m_treeModel->invisibleRootItem();
    int parent_row_count = parent->rowCount();
    QSI* curr_lexicon_item = parent->child(parent_row_count-1, 0);

    remove_item_from_tree("Gold Standard", curr_lexicon_item);

    QSI* gs_item = new QSI(QString("Gold Standard"));

    append_eval_results(lexicon->get_goldstandard()->get_results(), gs_item);

    curr_lexicon_item->appendRow(gs_item);
}

void MainWindow::update_TreeModel_for_eval(CLexicon *lexicon)
{
    if (lexicon->get_eval_parses() == NULL) {
        qDebug() << "MainWindow::update_TreeModel_for_eval: "
                    "EvalParses not loaded";
        return;
    }

    typedef QStandardItem QSI;
    QSI* parent = m_treeModel->invisibleRootItem();
    int parent_row_count = parent->rowCount();
    QSI* curr_lexicon_item = parent->child(parent_row_count-1, 0);

    QSI* eval_item = new QSI(QString("Morfessor Parses"));

    append_eval_results(lexicon->get_eval_parses()->get_results(), eval_item);

    curr_lexicon_item->appendRow(eval_item);
}

