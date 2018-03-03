#ifndef GOTOLINE_H
#define GOTOLINE_H

#include <QDialog>

namespace Ui {
class GoToLine;
}

class GoToLine : public QDialog
{
    Q_OBJECT

public:
    explicit GoToLine(QWidget *parent = 0);
    ~GoToLine();

signals:
    void goTo(int);

private slots:
    void on_gotoButton_clicked();

private:
    Ui::GoToLine *ui;
};

#endif // GOTOLINE_H
