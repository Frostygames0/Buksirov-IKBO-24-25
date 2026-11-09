#pragma once

#include <cstddef>

#include <QString>
#include <QtTypes>

#include "core/history.h"

namespace EmulatorMirea::Core
{
class Shell;
}

namespace EmulatorMirea::Presentation
{

class HistoryNavigator
{
public:
    explicit HistoryNavigator(const Core::History& history) : m_history(history) {}

    QString up(const QString& current);
    QString down(const QString& current);

    void reset();

private:
    const Core::History& m_history;
    std::size_t m_index{};
    QString m_scratch;
};

} // namespace EmulatorMirea::Presentation