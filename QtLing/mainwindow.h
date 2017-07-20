#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMap>
#include <QList>

QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QPlainTextEdit;
class QTextEdit;
class QSessionManager;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QMap<QString, int> Words; //would correspond to word collection
    QMap<QString, int> Protostems;
    QList<QPair<QString,QString>> Parses;
    QMap<QString,QMap<QString,int>> Stems; // value is a map from suffixes so that it acts like a set.
    QMap<QString,QStringList> Signatures;
public:
    MainWindow();

    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;
    void FindProtostems();
    void CreateStemAffixPairs();
    void AssignSuffixesToStems();
private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

private:
    void createHorizontalGroupBox();

    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QGroupBox *horizontalGroupBox;
    QGroupBox *verticalGroupBox;
    QPlainTextEdit *textEdit;
    QPlainTextEdit *littleEditor;
    QString curFile;
};

#endif
