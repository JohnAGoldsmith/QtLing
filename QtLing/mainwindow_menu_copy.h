#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include "mainwindow.h"
#include <QObject>
#include <QWidget>

class MainMenuBar : public QMenuBar
{
    Q_OBJECT

    QMenu           *fileMenu;
    QToolBar        *fileToolBar;
    QAction         *openAct;
    QAction         *saveAsAct;
    QAction         *exitAct;

    QMenu           *editMenu;
    QToolBar        *editToolBar;

    QAction         *cutAct;
    QAction         *copyAct;
    QAction         *pasteAct;

    QMenu           *helpMenu;

    QAction         *aboutAct;
    QAction         *aboutQtAct;

    QMenu           *evalMenu;

    QAction         *importAct;
    QAction         *evalAct;
    QAction         *importMorfessorAct;
    QAction         *evalMorfessorAct;

    bool            gs_is_ready;
    bool            lexicon_is_ready;
    bool            eval_parse_is_ready;

    void            check_and_enable_evalAct();
    void            check_and_enable_evalMorfessorAct();

public:
    explicit MainMenuBar(MainWindow *parent = nullptr);

signals:

public slots:
    void            gs_ready();
    void            lexicon_ready();
    void            eval_parse_ready();
};

#endif // MAINMENU_H
