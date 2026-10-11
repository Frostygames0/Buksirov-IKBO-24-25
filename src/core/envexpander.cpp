#include "core/envexpander.h"

#include <QString>
#include <QtTypes>

#include "core/session.h"

namespace EmulatorMirea
{

namespace
{
bool isNameStart(QChar c)
{
    return c.isLetter() || c == '_';
}
bool isNameChar(QChar c)
{
    return c.isLetterOrNumber() || c == '_';
}
} // namespace

QString EnvExpander::expand(const QString& token, const Session& session) const
{
    if (!token.contains('$'))
        return token;

    QString result;
    const qsizetype n = token.size();

    for (qsizetype i = 0; i < n; ++i)
    {
        const QChar c = token[i];

        if (c != '$' || i + 1 >= n)
        {
            result += c;
            continue;
        }

        // ${NAME}
        if (token[i + 1] == '{')
        {
            const qsizetype end = token.indexOf('}', i + 2);
            if (end == -1)
            {
                result += c; // unterminated, keep literally
                continue;
            }
            result += session.getEnv(token.mid(i + 2, end - i - 2));
            i = end;
        }
        // $NAME
        else if (isNameStart(token[i + 1]))
        {
            qsizetype j = i + 1;
            while (j < n && isNameChar(token[j]))
                ++j;
            result += session.getEnv(token.mid(i + 1, j - i - 1));
            i = j - 1;
        }
        // lone '$' or '$' followed by something else
        else
        {
            result += c;
        }
    }

    return result;
}

} // namespace EmulatorMirea