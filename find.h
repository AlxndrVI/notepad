#ifndef FIND_H
#define FIND_H

#include <QDialog>
#include <QTextEdit>

namespace Ui {
class Find;
}

class Find : public QDialog
{
    Q_OBJECT

public:
    explicit Find(QWidget *parent = 0);
    ~Find();

signals:
    void findText(QString what, QTextDocument::FindFlags findFlags);

private slots:
    void on_FindNextButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::Find *ui;

};

#endif // FIND_H
