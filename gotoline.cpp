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
