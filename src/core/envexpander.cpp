#include "core/envexpander.h"

#include "core/session.h"

namespace EmulatorMirea
{

namespace
{
bool IsNameStart(QChar c) { return c.isLetter() || c == '_'; }
bool IsNameChar(QChar c)  { return c.isLetterOrNumber() || c == '_'; }
}

QString EnvExpander::Expand(const QString& token, Session& session) const
{
    if (!token.contains('$'))
        return token;

    QString result;
    const int n = token.size();

    for (int i = 0; i < n; ++i)
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
            const int end = token.indexOf('}', i + 2);
            if (end == -1)
            {
                result += c;   // unterminated, keep literally
                continue;
            }
            result += session.getEnv(token.mid(i + 2, end - i - 2));
            i = end;
        }
        // $NAME
        else if (IsNameStart(token[i + 1]))
        {
            int j = i + 1;
            while (j < n && IsNameChar(token[j]))
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

}