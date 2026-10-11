#include "core/tokenizer.h"

#include <QString>
#include <QStringView>
#include <QStringList>

namespace EmulatorMirea
{

QStringList Tokenizer::tokenize(QStringView input, const Session& session) const
{
    QStringList tokens;
    QString current;
    bool inToken = false;
    bool literal = false; // token contained '...', so don't expand it
    QChar quote;

    auto finishToken = [&]
    {
        const QString toPush = (literal ? current : m_envExpander.expand(current, session));

        if (!toPush.isEmpty())
            tokens << toPush;

        current.clear();
        inToken = false;
        literal = false;
    };

    for (int i = 0; i < input.size(); ++i)
    {
        const QChar c = input[i];

        if (!quote.isNull())
        {
            if (c == quote)
                quote = QChar();
            else if (c == '\\' && quote == '"' && i + 1 < input.size())
                current += input[++i];
            else
                current += c;
        }
        else if (c == '\'' || c == '"')
        {
            quote = c;
            inToken = true;
            if (c == '\'')
                literal = true;
        }
        else if (c == '\\' && i + 1 < input.size())
        {
            current += input[++i];
            inToken = true;
        }
        else if (c.isSpace())
        {
            if (inToken)
                finishToken();
        }
        else
        {
            current += c;
            inToken = true;
        }
    }

    if (inToken)
        finishToken();

    return tokens;
}

} // namespace EmulatorMirea
