#ifndef COMMAND_H
#define COMMAND_H

#include <QStringList>

class QString;

namespace EmulatorMirea::Core
{

class ISession;

class Command
{
public:
    virtual ~Command() noexcept = default;

    [[nodiscard]] virtual QString name() const = 0;
    virtual QString execute(const QStringList& args, ISession& session) const = 0;
};

class StubCommand : public Command
{
public:
    explicit StubCommand(QString name);

    [[nodiscard]] QString name() const override;
    QString execute(const QStringList& args, ISession& session) const override;

private:
    QString m_name;
};

class ExitCommand : public Command
{
public:
    [[nodiscard]] QString name() const override;
    QString execute(const QStringList& args, ISession& session) const override;
};

} // namespace EmulatorMirea::Core

#endif // COMMAND_H
