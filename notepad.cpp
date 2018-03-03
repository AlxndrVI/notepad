#include <QKeyEvent>
#include "notepad.h"
#include "ui_notepad.h"
#include "find.h"
#include "gotoline.h"

bool Notepad::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->textEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->matches(QKeySequence::Undo))
        {
            on_actionUndo_triggered();
            return true;
        }
    }
    return false;
}

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    finder = new Find(this);
    finder->setModal(false);
    finder->setWindowFlags(Qt::Dialog
                         | Qt::MSWindowsFixedSizeDialogHint
                         | Qt::WindowTitleHint
                         | Qt::WindowCloseButtonHint
                         | Qt::CustomizeWindowHint);
    QObject::connect(finder, SIGNAL(findText(QString,QTextDocument::FindFlags)), this, SLOT(FindNext_triggered(QString,QTextDocument::FindFlags)));

    replacer = new FindAndReplace(this);
    replacer->setModal(false);
    replacer->setWindowFlags(Qt::Dialog
                           | Qt::MSWindowsFixedSizeDialogHint
                           | Qt::WindowTitleHint
                           | Qt::WindowCloseButtonHint
                           | Qt::CustomizeWindowHint);
    QObject::connect(replacer, SIGNAL(findText(QString,QTextDocument::FindFlags)), this, SLOT(FindNext_triggered(QString,QTextDocument::FindFlags)));
    QObject::connect(replacer, SIGNAL(replaceText(QString)), this, SLOT(Replace_triggered(QString)));
    QObject::connect(replacer, SIGNAL(replaceAll(QString,QString,QTextDocument::FindFlags)), this, SLOT(ReplaceAll_triggered(QString,QString,QTextDocument::FindFlags)));

    gotoliner = new GoToLine(this);
    gotoliner->setModal(false);
    gotoliner->setWindowFlags(Qt::Dialog
                            | Qt::MSWindowsFixedSizeDialogHint
                            | Qt::WindowTitleHint
                            | Qt::WindowCloseButtonHint
                            | Qt::CustomizeWindowHint);
    QObject::connect(gotoliner, SIGNAL(goTo(int)), this, SLOT(Goto_triggered(int)));

    ui->setupUi(this);

    this->setCentralWidget(ui->textEdit);
    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active, QPalette::Highlight));
    ui->textEdit->setPalette(p);
    ui->textEdit->installEventFilter(this);

    on_actionWord_Wrap_toggled(false);
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
    setWindowTitle("Untitled - Notepad");
}

void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        currentFile.clear();
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName + " - Notepad");
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void Notepad::on_actionSave_triggered()
{
    QString fileName;
    if (currentFile.isEmpty())
    {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
        setWindowTitle(fileName + " - Notepad");
    }
    else
    {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void Notepad::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName + " - Notepad");
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void Notepad::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void Notepad::on_actionPrint_triggered()
{
#if QT_CONFIG(printer)
    QPrinter printDev;
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected)
    {
        QMessageBox::warning(this, "Warning", "Cannot be used on non-native printers");
        return;
    }
#endif // QT_CONFIG(printdialog)
    ui->textEdit->print(&printDev);
#endif // QT_CONFIG(printer)
}

void Notepad::on_actionUndo_triggered()
{
    if (undoAvailable)
        ui->textEdit->undo();
    else if (redoAvailable)
    {
        QTextCursor cursor = ui->textEdit->textCursor();
        ui->textEdit->redo();
    }
}

void Notepad::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void Notepad::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void Notepad::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void Notepad::on_actionDelete_triggered()
{
    ui->textEdit->textCursor().removeSelectedText();
}

void Notepad::on_textEdit_selectionChanged()
{
    bool hasSelection = ui->textEdit->textCursor().hasSelection();
    ui->actionCut->setEnabled(hasSelection);
    ui->actionCopy->setEnabled(hasSelection);
    ui->actionDelete->setEnabled(hasSelection);
}

void Notepad::on_actionAdd_Time_Date_triggered()
{
    QLocale loc;
    ui->textEdit->textCursor().insertText(
                QTime::currentTime().toString(loc.timeFormat(QLocale::ShortFormat)) + " " +
                QDate::currentDate().toString(loc.dateFormat(QLocale::ShortFormat)));
}

void Notepad::on_actionFind_triggered()
{
    finder->show();
}

void Notepad::on_actionFind_next_triggered()
{
    if (findText.isEmpty())
        finder->show();
    else
        ui->textEdit->find(findText, findFlags);
}

void Notepad::on_actionReplace_triggered()
{
     replacer->show();
}

void Notepad::on_actionSelect_All_triggered()
{
    ui->textEdit->selectAll();
}

void Notepad::FindNext_triggered(QString what, QTextDocument::FindFlags flags)
{
    findText = what;
    findFlags = flags;
    ui->textEdit->find(findText, findFlags);
}

void Notepad::Replace_triggered(QString with)
{
    if (ui->textEdit->textCursor().hasSelection())
        ui->textEdit->textCursor().insertText(with);
}

void Notepad::ReplaceAll_triggered(QString what, QString with, QTextDocument::FindFlags flags)
{
    findText = what;
    findFlags = flags;
    ui->textEdit->textCursor().beginEditBlock();
    while (ui->textEdit->find(findText, findFlags))
    {
        if (ui->textEdit->textCursor().hasSelection())
            ui->textEdit->textCursor().insertText(with);
    }
    ui->textEdit->textCursor().endEditBlock();
}

void Notepad::on_textEdit_undoAvailable(bool b)
{
    undoAvailable = b;
    activate_actionUndo();
}

void Notepad::on_textEdit_redoAvailable(bool b)
{
    redoAvailable = b;
    activate_actionUndo();
}

void Notepad::activate_actionUndo()
{
    ui->actionUndo->setEnabled(undoAvailable | redoAvailable);
}

void Notepad::on_actionGoto_triggered()
{
    gotoliner->show();
}

void Notepad::Goto_triggered(int row)
{
    row = qMax(row, ui->textEdit->document()->blockCount()) - 1;
    QTextCursor cur = ui->textEdit->textCursor();
    cur.movePosition(QTextCursor::Start);
    cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, row);
    ui->textEdit->setTextCursor(cur);
    /*QTextCursor cur = ui->textEdit->textCursor();
    QTextBlock block = ui->textEdit->document()->findBlockByLineNumber(row);
    cur.setPosition(block.position());
    ui->textEdit->setTextCursor(cur);*/
}

void Notepad::on_actionWord_Wrap_toggled(bool arg1)
{
    if (arg1)
        ui->textEdit->setWordWrapMode(QTextOption::WrapMode::WordWrap);
    else
        ui->textEdit->setWordWrapMode(QTextOption::WrapMode::NoWrap);
}
