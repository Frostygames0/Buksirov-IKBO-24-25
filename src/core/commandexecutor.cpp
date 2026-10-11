#include "core/commandexecutor.h"

#include <QCoreApplication>
#include <QString>

namespace EmulatorMirea
{

QString CommandExecutor::Execute(const QStringList& tokens, Session& session) const
{
    if (tokens.isEmpty())
        return {};

    const QString& name = tokens.first();

    if (name == "ls" || name == "cd")
    {
        return tokens.join(' ');
    }

    if (name == "exit")
    {
        QCoreApplication::quit();
        return {};
    }

    return name + ": command not found";
}

}
