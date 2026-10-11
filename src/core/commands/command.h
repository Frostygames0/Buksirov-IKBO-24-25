#ifndef COMMAND_H
#define COMMAND_H

#include <QStringList>
#include "core/session.h"

namespace EmulatorMirea::Commands
{

class Command
{
public:
    ~Command() noexcept = default;

    virtual QString name() const = 0;
    virtual QString execute(const QStringList& args, Session& session) const = 0;
};

class StubCommand : public Command
{
public:
    explicit StubCommand(const QString& name);

    QString name() const override;
    QString execute(const QStringList& args, Session& session) const override;
private:
    QString m_name;
};

class ExitCommand : public Command
{
public:
    QString name() const override;
    QString execute(const QStringList& args, Session& session) const override;
};

}

#endif // COMMAND_H
