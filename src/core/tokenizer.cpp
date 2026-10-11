#include "core/tokenizer.h"

#include <QChar>
#include <QString>
#include <QStringList>
#include <QStringView>

namespace EmulatorMirea
{

namespace
{
// Inside double quotes a backslash only escapes these characters (as in bash).
bool isDoubleQuoteEscapable(QChar c)
{
    return c == '"' || c == '\\' || c == '$';
}
} // namespace

QStringList Tokenizer::tokenize(QStringView input, const Session& session) const
{
    QStringList tokens;
    QString result;  // finished text of the current token
    QString pending; // raw text that still needs $VAR expansion
    bool inToken = false;
    bool quoted = false; // token used quotes, so an empty value is kept
    QChar quote;

    auto flushPending = [&]
    {
        result += m_envExpander.expand(pending, session);
        pending.clear();
    };

    auto appendLiteral = [&](QChar c)
    {
        flushPending();
        result += c;
    };

    auto finishToken = [&]
    {
        flushPending();

        // An unquoted token that expanded to nothing disappears; "" stays.
        if (!result.isEmpty() || quoted)
            tokens << result;

        result.clear();
        inToken = false;
        quoted = false;
    };

    for (qsizetype i = 0; i < input.size(); ++i)
    {
        const QChar c = input[i];
        const bool hasNext = i + 1 < input.size();

        if (quote == '\'')
        {
            if (c == quote)
                quote = QChar();
            else
                result += c; // single quotes: everything is literal
        }
        else if (quote == '"')
        {
            if (c == quote)
                quote = QChar();
            else if (c == '\\' && hasNext && isDoubleQuoteEscapable(input[i + 1]))
                appendLiteral(input[++i]);
            else
                pending += c;
        }
        else if (c == '\'' || c == '"')
        {
            flushPending(); // text before the quote is expanded on its own
            quote = c;
            inToken = true;
            quoted = true;
        }
        else if (c == '\\' && hasNext)
        {
            appendLiteral(input[++i]); // escaped char is never expanded
            inToken = true;
        }
        else if (c.isSpace())
        {
            if (inToken)
                finishToken();
        }
        else
        {
            pending += c;
            inToken = true;
        }
    }

    if (inToken)
        finishToken();

    return tokens;
}

} // namespace EmulatorMirea