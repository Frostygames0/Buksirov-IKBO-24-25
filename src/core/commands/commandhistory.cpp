#include "commandhistory.h"

namespace EmulatorMirea::Core
{

void CommandHistory::record(const QString& line)
{
    if (!line.trimmed().isEmpty() && (m_entries.isEmpty() || m_entries.last() != line))
    {
        m_entries.append(line);
    }
    m_cursor = m_entries.size();
}

std::optional<QString> CommandHistory::moveUp()
{
    if (m_entries.isEmpty())
    {
        return std::nullopt;
    }

    if (m_cursor > 0)
    {
        --m_cursor;
    }

    return m_entries[m_cursor];
}

std::optional<QString> CommandHistory::moveDown()
{
    if (m_entries.isEmpty())
    {
        return std::nullopt;
    }

    if (m_cursor < m_entries.size() - 1)
    {
        ++m_cursor;
        return m_entries[m_cursor];
    }

    m_cursor = m_entries.size();
    return QString{};
}

} // namespace EmulatorMirea::Core
