#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <QString>
#include <QStringList>

#include "core/commands/registry.h"

namespace EmulatorMirea
{

class Session;

namespace Commands
{

class Executor
{
public:
    explicit Executor();
    QString execute(const QStringList& tokens, Session& session) const;

private:
    Registry m_registry;
};

} // namespace Commands
} // namespace EmulatorMirea

#endif // COMMANDEXECUTOR_H
