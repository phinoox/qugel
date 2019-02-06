#include "mmlsyntaxhighliter.h"

class GLSLBlockData: public QTextBlockUserData
{
public:
    QList<int> bracketPositions;
};


MMLSyntaxHighliter::MMLSyntaxHighliter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::cyan);
    keywordFormat.setFontWeight(QFont::Bold);

    // default color scheme
    m_colors[ColorComponent::Normal]            = QColor("#000000");
    m_colors[ColorComponent::Comment]           = QColor("#808080");
    m_colors[ColorComponent::PreProcessor]      = QColor("#804040");
    m_colors[ColorComponent::Number]            = QColor("#008000");
    m_colors[ColorComponent::String]            = QColor("#800000");
    m_colors[ColorComponent::Operator]          = QColor("#808000");
    m_colors[ColorComponent::Identifier]        = QColor("#000020");
    m_colors[ColorComponent::Keyword]           = QColor("#000080");
    m_colors[ColorComponent::BuiltIn]           = QColor("#008080");
    m_colors[ColorComponent::Marker]            = QColor("#ffff00");

    m_keywords << "attribute";
    m_keywords << "const";
    m_keywords << "uniform";
    m_keywords << "varying";
    m_keywords << "layout";
    m_keywords << "centroid";
    m_keywords << "flat";
    m_keywords << "smooth";
    m_keywords << "noperspective";
    m_keywords << "patch";
    m_keywords << "sample";
    m_keywords << "break";
    m_keywords << "continue";
    m_keywords << "do";
    m_keywords << "for";
    m_keywords << "while";
    m_keywords << "switch";
    m_keywords << "case";
    m_keywords << "default";
    m_keywords << "if";
    m_keywords << "else";
    m_keywords << "subroutine";
    m_keywords << "in";
    m_keywords << "out";
    m_keywords << "inout";
    m_keywords << "true";
    m_keywords << "false";
    m_keywords << "invariant";
    m_keywords << "discard";
    m_keywords << "return";
    m_keywords << "lowp";
    m_keywords << "mediump";
    m_keywords << "highp";
    m_keywords << "precision";
    m_keywords << "struct";

    m_types << "float";
    m_types << "double";
    m_types << "int";
    m_types << "void";
    m_types << "bool";
    m_types << "mat2";
    m_types << "mat3";
    m_types << "mat4";
    m_types << "dmat2";
    m_types << "dmat3";
    m_types << "dmat4";
    m_types << "mat2x2";
    m_types << "mat2x3";
    m_types << "mat2x4";
    m_types << "dmat2x2";
    m_types << "dmat2x3";
    m_types << "dmat2x4";
    m_types << "mat3x2";
    m_types << "mat3x3";
    m_types << "mat3x4";
    m_types << "dmat3x2";
    m_types << "dmat3x3";
    m_types << "dmat3x4";
    m_types << "mat4x2";
    m_types << "mat4x3";
    m_types << "mat4x4";
    m_types << "dmat4x2";
    m_types << "dmat4x3";
    m_types << "dmat4x4";
    m_types << "vec2";
    m_types << "vec3";
    m_types << "vec4";
    m_types << "ivec2";
    m_types << "ivec3";
    m_types << "ivec4";
    m_types << "bvec2";
    m_types << "bvec3";
    m_types << "bvec4";
    m_types << "dvec2";
    m_types << "dvec3";
    m_types << "dvec4";
    m_types << "uint";
    m_types << "uvec2";
    m_types << "uvec3";
    m_types << "uvec4";
    m_types << "sampler1D";
    m_types << "sampler2D";
    m_types << "sampler3D";
    m_types << "samplerCube";
    m_types << "sampler1DShadow";
    m_types << "sampler2DShadow";
    m_types << "samplerCubeShadow";
    m_types << "sampler1DArray";
    m_types << "sampler2DArray";
    m_types << "sampler1DArrayShadow";
    m_types << "sampler2DArrayShadow";
    m_types << "isampler1D";
    m_types << "isampler2D";
    m_types << "isampler3D";
    m_types << "isamplerCube";
    m_types << "isampler1DArray";
    m_types << "isampler2DArray";
    m_types << "usampler1D";
    m_types << "usampler2D";
    m_types << "usampler3D";
    m_types << "usamplerCube";
    m_types << "usampler1DArray";
    m_types << "usampler2DArray";
    m_types << "sampler2DRect";
    m_types << "sampler2DRectShadow";
    m_types << "isampler2DRect";
    m_types << "usampler2DRect";
    m_types << "samplerBuffer";
    m_types << "isamplerBuffer";
    m_types << "usamplerBuffer";
    m_types << "sampler2DMS";
    m_types << "isampler2DMS";
    m_types << "usampler2DMS";
    m_types << "sampler2DMSArray";
    m_types << "isampler2DMSArray";
    m_types << "usampler2DMSArray";
    m_types << "samplerCubeArray";
    m_types << "samplerCubeArrayShadow";
    m_types << "isamplerCubeArray";
    m_types << "usamplerCubeArray";




}

void MMLSyntaxHighliter::highlightBlock(const QString &text)
{
    // parsing state
    enum {
        Start = 0,
        Number = 1,
        Identifier = 2,
        String = 3,
        Comment = 4,
        Regex = 5,
        PreProcessor =6
    };

    QList<int> bracketPositions;

    int blockState = previousBlockState();
    int bracketLevel = blockState >> 4;
    int state = blockState & 15;
    if (blockState < 0) {
        bracketLevel = 0;
        state = Start;
    }

    int start = 0;
    int i = 0;
    while (i <= text.length()) {
        QChar ch = (i < text.length()) ? text.at(i) : QChar();
        QChar next = (i < text.length() - 1) ? text.at(i + 1) : QChar();

        switch (state) {

        case Start:
            start = i;
            if (ch.isSpace()) {
                ++i;
            } else if (ch.isDigit()) {
                ++i;
                state = Number;
            } else if (ch.isLetter() || ch == '_') {
                ++i;
                state = Identifier;
            } else if (ch == '#' ) {
                i = text.length();
                setFormat(start, text.length(), m_colors[ColorComponent::PreProcessor]);
            } else if (ch == '\'' || ch == '\"') {
                ++i;
                state = String;
            } else if (ch == '/' && next == '*') {
                ++i;
                ++i;
                state = Comment;
            } else if (ch == '/' && next == '/') {
                i = text.length();
                setFormat(start, text.length(), m_colors[ColorComponent::Comment]);
            } else if (ch == '/' && next != '*') {
                ++i;
                state = Regex;
            } else {
                if (!QString("(){}[]").contains(ch))
                    setFormat(start, 1, m_colors[ColorComponent::Operator]);
                if (ch =='{' || ch == '}') {
                    bracketPositions += i;
                    if (ch == '{')
                        bracketLevel++;
                    else
                        bracketLevel--;
                }
                ++i;
                state = Start;
            }
            break;

        case Number:
            if (ch.isSpace() || !ch.isDigit()) {
                setFormat(start, i - start, m_colors[ColorComponent::Number]);
                state = Start;
            } else {
                ++i;
            }
            break;

        case Identifier:
            if (ch.isSpace() || !(ch.isDigit() || ch.isLetter() || ch == '_')) {
                QString token = text.mid(start, i - start).trimmed();
                if (m_keywords.contains(token))
                    setFormat(start, i - start, m_colors[ColorComponent::Keyword]);
                else if (m_types.contains(token) || m_builtins.contains(token))
                    setFormat(start, i - start, m_colors[ColorComponent::BuiltIn]);
                state = Start;
            } else {
                ++i;
            }
            break;

        case String:
            if (ch == text.at(start)) {
                QChar prev = (i > 0) ? text.at(i - 1) : QChar();
                if (prev != '\\') {
                    ++i;
                    setFormat(start, i - start, m_colors[ColorComponent::String]);
                    state = Start;
                } else {
                    ++i;
                }
            } else {
                ++i;
            }
            break;

        case Comment:
            if (ch == '*' && next == '/') {
                ++i;
                ++i;
                setFormat(start, i - start, m_colors[ColorComponent::Comment]);
                state = Start;
            } else {
                ++i;
            }
            break;



        case Regex:
            if (ch == '/') {
                QChar prev = (i > 0) ? text.at(i - 1) : QChar();
                if (prev != '\\') {
                    ++i;
                    setFormat(start, i - start, m_colors[ColorComponent::String]);
                    state = Start;
                } else {
                    ++i;
                }
            } else {
                ++i;
            }
            break;

        default:
            state = Start;
            break;
        }
    }

    if (state == Comment)
        setFormat(start, text.length(), m_colors[ColorComponent::Comment]);
    else if (state == PreProcessor)
        setFormat(start, text.length(), m_colors[ColorComponent::PreProcessor]);
    else
        state = Start;

    if (!m_markString.isEmpty()) {
        int pos = 0;
        int len = m_markString.length();
        QTextCharFormat markerFormat;
        markerFormat.setBackground(m_colors[ColorComponent::Marker]);
        markerFormat.setForeground(m_colors[ColorComponent::Normal]);
        for (;;) {
            pos = text.indexOf(m_markString, pos, m_markCaseSensitivity);
            if (pos < 0)
                break;
            setFormat(pos, len, markerFormat);
            ++pos;
        }
    }

    if (!bracketPositions.isEmpty()) {
        GLSLBlockData *blockData = reinterpret_cast<GLSLBlockData*>(currentBlock().userData());
        if (!blockData) {
            blockData = new GLSLBlockData;
            currentBlock().setUserData(blockData);
        }
        blockData->bracketPositions = bracketPositions;
    }

    blockState = (state & 15) | (bracketLevel << 4);
    setCurrentBlockState(blockState);
}
