#ifndef QUTEXTEDITOR_H
#define QUTEXTEDITOR_H

#include <QWidget>
#include "mmlsyntaxhighliter.h"

namespace Ui {
class quTextEditor;
}

class quTextEditor : public QWidget
{
    Q_OBJECT

public:
    explicit quTextEditor(QWidget *parent = nullptr);
    ~quTextEditor();


    QString getFilePath() const;
    void setFilePath(const QString &value);

private slots:
    void on_pushButton_clicked();

private:
    Ui::quTextEditor *ui;
    QString filePath;
    QString text;
    MMLSyntaxHighliter* mlHighliter;
};

#endif // QUTEXTEDITOR_H
