#pragma once

#include <cstddef>

#include <QString>
#include <QtTypes>

namespace EmulatorMirea::Core
{
class Shell;
}

namespace EmulatorMirea::Presentation
{

class HistoryNavigator
{
public:
    explicit HistoryNavigator(const Core::Shell& shell) : m_shell(shell) {}

    QString up(const QString& current);
    QString down(const QString& current);

    void reset();

private:
    const Core::Shell& m_shell;
    std::size_t m_index = 0;
    QString m_scratch;
};

} // namespace EmulatorMirea::Presentation