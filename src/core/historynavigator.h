#pragma once

#include <QString>
#include <QtTypes>

namespace EmulatorMirea::Core
{

class Shell;

class HistoryNavigator
{
public:
    explicit HistoryNavigator(const Shell& shell);

    QString up(const QString& current);
    QString down();

    void reset();

private:
    const Shell& m_shell;
    qsizetype m_index = 0;
    QString m_scratch;
};

} // namespace EmulatorMirea::Core