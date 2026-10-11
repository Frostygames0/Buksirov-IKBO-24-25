#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <QString>
#include <QStringList>

#include "core/commands/registry.h"

namespace EmulatorMirea::Core
{

class Session;

class CommandExecutor
{
public:
    explicit CommandExecutor();
    QString execute(const QStringList& tokens, ISession& session) const;

private:
    CommandRegistry m_registry;
};

} // namespace EmulatorMirea::Core

#endif // COMMANDEXECUTOR_H
