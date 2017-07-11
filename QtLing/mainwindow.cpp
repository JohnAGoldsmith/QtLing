#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <stdio.h>
#include "WordCollection.h"
#include "Word.h"

#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool wordLessThan(CWord word1, CWord word2)
{
    return word1.GetWord() < word2.GetWord();
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << QDir::currentPath();
    QFile infile("../../../../QtLing/test.txt");
    if (!infile.exists()) {
        qDebug() << "file read error";
    }
    if (!infile.open( QIODevice::ReadOnly)) {
        qDebug() << "file open error";
        return;
    }
    QString content = infile.readAll();
    qDebug() << content;


    QList<QString> list = content.split(" ");
    qSort(list);
    CWordCollection wordCollection;
    int i;
    int count = list.count();

    for (i = 0; i < count; i++) {
        qDebug() << list.at(i);
        wordCollection << list.at(i);
//        qDebug() << wordCollection.GetAt(i).GetWord();
    }
//    qSort(wordCollection.m_WordList);

    QStandardItemModel *model = new QStandardItemModel(list.count(),1,this); //n Rows and 3 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
    //    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
    //    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));

    ui->tableView->setModel(model);
    uint len = (uint)list.count();
    for (uint j = 0; j < len; j++) {
        QStandardItem *ithRow = new QStandardItem(wordCollection.GetAt(j).GetWord() );
        qDebug() << wordCollection.GetAt(j).GetWord();
        qDebug() << wordCollection.GetLength();
//        QStandardItem *ithRow = new QStandardItem(list.at(j));
        model->setItem(j,0,ithRow);
    }
}
