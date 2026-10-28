#include "lineparser.h"

#include <QChar>
#include <QString>
#include <QStringList>
#include <QStringView>

#include "core/envprovider.h"

namespace EmulatorMirea::Core::LineParser
{

namespace
{
bool isDoubleQuoteEscapable(QChar c)
{
    return c == u'"' || c == u'\\' || c == u'$';
}

bool isNameStart(QChar c)
{
    return c.isLetter() || c == u'_';
}

bool isNameChar(QChar c)
{
    return c.isLetterOrNumber() || c == u'_';
}

qsizetype expandDollar(QStringView in, qsizetype i, const IEnvProvider& env, QString& out)
{
    const qsizetype n = in.size();

    if (i + 1 < n && in[i + 1] == u'{')
    {
        const qsizetype end = in.indexOf(u'}', i + 2);
        if (end != -1)
        {
            out += env.getEnv(in.mid(i + 2, end - i - 2).toString(), {});
            return end;
        }
    }
    else if (i + 1 < n && isNameStart(in[i + 1]))
    {
        qsizetype j = i + 1;
        while (j < n && isNameChar(in[j]))
        {
            ++j;
        }
        out += env.getEnv(in.mid(i + 1, j - i - 1).toString(), {});
        return j - 1;
    }

    out += u'$';
    return i;
}
} // namespace

QStringList parse(QStringView in, const IEnvProvider& env)
{
    QStringList tokens;
    QString word;
    bool inToken = false;
    bool quoted = false; // token used quotes, so an empty value is kept
    QChar quote;

    auto finishToken = [&] {
        // An unquoted token that expanded to nothing disappears; "" stays.
        if (!word.isEmpty() || quoted)
        {
            tokens << word;
        }
        word.clear();
        inToken = false;
        quoted = false;
    };

    for (qsizetype i = 0; i < in.size(); ++i)
    {
        const QChar c = in[i];
        const bool hasNext = i + 1 < in.size();

        // If it's a single quote, then parse it verbatim, no expansion or anything
        if (quote == u'\'')
        {
            if (c == quote)
            {
                quote = QChar();
            }
            else
            {
                word += c;
            }
        }
        // If it's a double quote, then parse it verbatim BUT expand dollars and escape chars
        else if (quote == u'"')
        {
            if (c == quote)
            {
                quote = QChar();
            }
            else if (c == u'\\' && hasNext && isDoubleQuoteEscapable(in[i + 1]))
            {
                word += in[++i];
            }
            else if (c == u'$')
            {
                i = expandDollar(in, i, env, word);
            }
            else
            {
                word += c;
            }
        }
        // If char is a quote, then start to process according to quote rules
        else if (c == u'\'' || c == u'"')
        {
            quote = c;
            inToken = true;
            quoted = true;
        }
        // if it's a slash, read it verbatim
        else if (c == u'\\' && hasNext)
        {
            word += in[++i];
            inToken = true;
        }
        // if it's a dollar, then expand dollar
        else if (c == u'$')
        {
            i = expandDollar(in, i, env, word);
            inToken = true;
        }
        // if it's a space and nothing else, then just finish the token
        else if (c.isSpace())
        {
            if (inToken)
            {
                finishToken();
            }
        }
        // else process
        else
        {
            word += c;
            inToken = true;
        }
    }

    if (inToken)
    {
        finishToken();
    }

    return tokens;
}

} // namespace EmulatorMirea::Core::LineParser