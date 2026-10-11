#include "core/shell.h"

namespace EmulatorMirea
{

QString Shell::submit(const QString& line)
{
    m_session.addHistory(line);   // skips blank lines and consecutive duplicates

    if (line.trimmed().isEmpty())
        return {};

    return m_interpreter.execute(line, m_session);
}

QString Shell::previousHistory()
{
    return m_session.previousHistoryItem();
}

QString Shell::nextHistory()
{
    return m_session.nextHistoryItem();
}

QString Shell::prompt() const
{
    return QStringLiteral("[%1]$ ").arg(m_session.fancyName());
}

QString Shell::title() const
{
    return m_session.fancyName();
}

}