#ifndef MMLSYNTAXHIGHLITER_H
#define MMLSYNTAXHIGHLITER_H


#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;

class MMLSyntaxHighliter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    MMLSyntaxHighliter(QTextDocument *parent = 0);
    typedef enum {
        Background,
        Normal,
        Comment,
        Number,
        String,
        Operator,
        Identifier,
        Keyword,
        BuiltIn,
        Sidebar,
        LineNumber,
        Cursor,
        Marker,
        BracketMatch,
        BracketError,
        FoldIndicator,
        PreProcessor
    } ColorComponent;


protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    QSet<QString> m_keywords;
    QSet<QString> m_types;
    QSet<QString> m_builtins;
    QHash<ColorComponent, QColor> m_colors;
    QString m_markString;
    Qt::CaseSensitivity m_markCaseSensitivity;
};


#endif // MMLSYNTAXHIGHLITER_H
