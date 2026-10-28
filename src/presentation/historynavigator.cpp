#include "historynavigator.h"

#include <QString>
#include <QtTypes>

#include "core/shell.h"

namespace EmulatorMirea::Presentation
{

QString HistoryNavigator::up(const QString& current)
{
    if (m_shell.historySize() == 0)
    {
        return current;
    }
    if (m_index == m_shell.historySize())
    {
        m_scratch = current;
    }
    if (m_index > 0)
    {
        --m_index;
    }
    return m_shell.historyAt(m_index);
}

QString HistoryNavigator::down(const QString& current)
{
    const qsizetype n = m_shell.historySize();

    if (m_index >= n)
    {
        m_scratch = current;
        return m_scratch;
    }

    ++m_index;
    return (m_index == n) ? m_scratch : m_shell.historyAt(m_index);
}

void HistoryNavigator::reset()
{
    m_index = m_shell.historySize();
    m_scratch.clear();
}

}; // namespace EmulatorMirea::Presentation