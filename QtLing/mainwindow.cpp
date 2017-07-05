#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <stdio.h>
#include "WordCollection.h"

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

void MainWindow::on_pushButton_clicked()
{
//    FILE* pFile = fopen("test.txt", "r");
//    if (!pFile) {
//        qDebug() << "rippp";
//        return;
//    }
//    char buff[100];

//    while(!feof(pFile)) {
//        if (fgets(buff, 100, pFile) == NULL) break;
//    }
//    fclose(pFile);
//    char* buffer = buff;
//    QString content(buffer);

//    QString filename = QString("test.txt");
//    QFile infile(filename);

//    if (!infile.open(pFile, QIODevice::ReadOnly)) {
//        qDebug() << "well shit.";
//        return;
//    }

//    QString content = infile.readAll();

//    QList<QString> list = content.split(" ");
    //int i;

//    CWordCollection wordCollection;

//    QList<QString> list;
//    list << "red" << "blue" << "green" << "yellow" << "purple" << "orange";
//    int count = list.count();
//    for (i = 0; i < count; i++) {
//        qDebug() << list.at(i);
//        wordCollection.operator <<( list.at(i) );
//    }
////    qSort(wordCollection.m_WordList);

//    QStandardItemModel *model = new QStandardItemModel(list.count(),1,this); //n Rows and 3 Columns
//    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
//    //    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
//    //    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));

//    ui->tableView->setModel(model);
//    int len = list.count();
//    for (i = 0; i < len; i++) {
//        QStandardItem *ithRow = new QStandardItem(wordCollection.m_WordList.at(i));
//        model->setItem(i,0,ithRow);
//    }
}
