#include "historynavigator.h"

#include <cstddef>

#include <QString>
#include <QtTypes>

namespace EmulatorMirea::Presentation
{

QString HistoryNavigator::up(const QString& current)
{
    if (m_history.size() == 0)
    {
        return current;
    }

    if (m_index == m_history.size())
    {
        m_scratch = current;
    }

    if (m_index > 0)
    {
        --m_index;
    }

    return QString::fromUtf8(m_history.at(m_index));
}

QString HistoryNavigator::down(const QString& current)
{
    const std::size_t n = m_history.size();

    if (m_index >= n)
    {
        m_scratch = current;
        return m_scratch;
    }

    ++m_index;
    return (m_index == n) ? m_scratch : QString::fromUtf8(m_history.at(m_index));
}

void HistoryNavigator::reset()
{
    m_index = m_history.size();
    m_scratch.clear();
}

}; // namespace EmulatorMirea::Presentation