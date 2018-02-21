#include "findandreplace.h"
#include "ui_findandreplace.h"

FindAndReplace::FindAndReplace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindAndReplace)
{
    ui->setupUi(this);
}

FindAndReplace::~FindAndReplace()
{
    delete ui;
}

void FindAndReplace::on_FindNext_clicked()
{
    QTextDocument::FindFlags findFlags = QTextDocument::FindFlags();
    if(ui->checkBox->isChecked()) findFlags.setFlag(QTextDocument::FindCaseSensitively);
    emit findText(ui->lineEdit->text(), findFlags);
}

void FindAndReplace::on_Replace_clicked()
{
    emit replaceText(ui->lineEdit_2->text());
}

void FindAndReplace::on_ReplaceAll_clicked()
{
    QTextDocument::FindFlags findFlags = QTextDocument::FindFlags();
    if(ui->checkBox->isChecked()) findFlags.setFlag(QTextDocument::FindCaseSensitively);
    emit replaceAll(ui->lineEdit->text(), ui->lineEdit_2->text(), findFlags);
}

void FindAndReplace::on_lineEdit_textChanged(const QString&)
{
    bool enable = !ui->lineEdit->text().isEmpty();
    ui->FindNext->setEnabled(enable);
    ui->Replace->setEnabled(enable);
    ui->ReplaceAll->setEnabled(enable);
}
