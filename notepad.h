#ifndef NOTEPAD_H
#define NOTEPAD_H

#include "find.h"
#include "findandreplace.h"
#include "gotoline.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QLocale>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>

namespace Ui {
class Notepad;
}

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionExit_triggered();

    void on_actionPrint_triggered();

    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionDelete_triggered();

    void on_textEdit_selectionChanged();

    void on_actionAdd_Time_Date_triggered();

    void on_actionFind_triggered();

    void FindNext_triggered(QString what, QTextDocument::FindFlags findFlags);

    void Replace_triggered(QString with);

    void ReplaceAll_triggered(QString what, QString with, QTextDocument::FindFlags findFlags);

    void on_actionFind_next_triggered();

    void on_actionReplace_triggered();

    void on_actionSelect_All_triggered();

    void on_textEdit_undoAvailable(bool b);

    void on_textEdit_redoAvailable(bool b);

    void on_actionGoto_triggered();

    void Goto_triggered(int);

    void on_actionWord_Wrap_toggled(bool arg1);

private:

    Ui::Notepad *ui;
    Find *finder;
    FindAndReplace *replacer;
    GoToLine *gotoliner;
    QString currentFile;
    QString findText;
    QTextDocument::FindFlags findFlags;
    bool undoAvailable = false;
    bool redoAvailable = false;
    bool eventFilter(QObject *watched, QEvent *event);
    void activate_actionUndo();
};

#endif // NOTEPAD_H
