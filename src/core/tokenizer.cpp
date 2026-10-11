#include "core/tokenizer.h"

#include <QStringView>
#include <QString>

namespace EmulatorMirea
{

QStringList Tokenizer::Tokenize(QStringView input) const
{
    QStringList tokens;
    QString current;
    bool inToken = false;
    QChar quote;

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
        }
        else if (c == '\\' && i + 1 < input.size())
        {
            current += input[++i];
            inToken = true;
        }
        else if (c.isSpace())
        {
            if (inToken)
            {
                tokens << current;
                current.clear();
                inToken = false;
            }
        }
        else
        {
            current += c;
            inToken = true;
        }
    }

    if (inToken)
        tokens << current;

    return tokens;
}

}
