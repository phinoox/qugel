#include "qutexteditor.h"
#include "ui_qutexteditor.h"

quTextEditor::quTextEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::quTextEditor)
{
    ui->setupUi(this);
    mlHighliter = new MMLSyntaxHighliter(ui->textEdit->document());
}

quTextEditor::~quTextEditor()
{
    delete ui;
}

void quTextEditor::on_pushButton_clicked()
{

}

QString quTextEditor::getFilePath() const
{
    return filePath;
}

void quTextEditor::setFilePath(const QString &value)
{
    filePath = value;
}
