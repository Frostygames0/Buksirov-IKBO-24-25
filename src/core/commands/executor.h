#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <QStringList>
#include <QString>

#include "core/commands/registry.h"
#include "core/session.h"

namespace EmulatorMirea::Commands
{

class Executor
{
public:
    explicit Executor();
    QString execute(const QStringList& tokens, Session& session) const;
private:
    Registry m_registry;
};

}

#endif // COMMANDEXECUTOR_H
