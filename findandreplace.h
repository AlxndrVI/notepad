#ifndef FINDANDREPLACE_H
#define FINDANDREPLACE_H

#include <QDialog>
#include <QTextDocument>

namespace Ui {
class FindAndReplace;
}

class FindAndReplace : public QDialog
{
    Q_OBJECT

public:
    explicit FindAndReplace(QWidget *parent = 0);
    ~FindAndReplace();

signals:
    void findText(QString what, QTextDocument::FindFlags findFlags);

    void replaceText(QString with);

    void replaceAll(QString what, QString with, QTextDocument::FindFlags findFlags);

private slots:
    void on_FindNext_clicked();

    void on_Replace_clicked();

    void on_ReplaceAll_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::FindAndReplace *ui;
};

#endif // FINDANDREPLACE_H
