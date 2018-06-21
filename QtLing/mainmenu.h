#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include "mainwindow.h"
#include <QObject>
#include <QWidget>

class MainMenu : public QObject
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

    bool            gs_is_loaded;
    bool            lexicon_is_ready;

public:
    explicit MainMenu(MainWindow *parent = nullptr);

signals:

public slots:
    void            gs_loaded();
    void            lexicon_ready();
};

#endif // MAINMENU_H
