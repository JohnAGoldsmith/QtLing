#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include <QStandardItemModel>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <algorithm>
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
    // works for reading in .dx1 files
    // works on absolute paths
    QString abspath("/Users/Tapan/QtLing/QtLing");
    if (!QDir::setCurrent(abspath))
    {
        qDebug() << "Could not change to home directory";
    }
    QString filename("browncorpus.dx1");
    QFile infile(filename);

    if (!infile.exists())
    {
        qDebug() << "file doesn't exist";
    }
    else
    {
        qDebug() << "file exists at least";
    }

    if (!infile.open( QIODevice::ReadOnly | QFile::Text)) {
        qDebug() << "file open error";
        return;
    }

    QTextStream in(&infile);

    CWordCollection wordCollection;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        line.simplified();
        QList<QString> wordsInLine = line.split(" ");
        QString word = wordsInLine[0]; //first word in line is the word to be analyzed
        Words[word] = 1; // map for qstring to frequency: what the CWord class should do
        wordCollection << word;
    }

//    CWordCollection wordCollection;
//    wordCollection << QString("hello");
//    qDebug() << wordCollection.GetAt(0).GetWord();

    QMapIterator<QString,int> iter(Words);
    QList<CWord>::iterator word_iter;

    QStandardItemModel *model = new QStandardItemModel(Words.size(),1,this); //n Rows and 3 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));

    ui->tableView->setModel(model);

    int rowno = 0;
//    std::sort(wordCollection.GetBegin(), wordCollection.GetEnd(), wordLessThan);
//    for (word_iter = wordCollection.GetBegin(); word_iter != wordCollection.GetEnd(); ++word_iter)
//    {
//        QStandardItem* ithRow = new QStandardItem(word_iter->GetWord());
//        model->setItem(rowno,0,ithRow);
//        rowno++;
//    }

    while (iter.hasNext()) {
        iter.next();
        QStandardItem *ithRow = new QStandardItem(iter.key());
        model->setItem(rowno,0,ithRow);
        rowno++;
    }
}



























