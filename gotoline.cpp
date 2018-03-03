#include "gotoline.h"
#include "ui_gotoline.h"

GoToLine::GoToLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoToLine)
{
    ui->setupUi(this);
}

GoToLine::~GoToLine()
{
    delete ui;
}

void GoToLine::on_gotoButton_clicked()
{
    bool ok;
    int row = ui->lineEdit->text().toInt(&ok);
    if (ok)
    {
        emit goTo(row);
        this->hide();
    }
}
