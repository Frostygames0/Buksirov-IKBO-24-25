#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <QStringList>

class QString;

namespace EmulatorMirea
{

class Session;

class CommandExecutor
{
public:
    QString Execute(const QStringList& tokens, Session& session) const;
};
}

#endif // COMMANDEXECUTOR_H
