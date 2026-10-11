#ifndef COMMAND_H
#define COMMAND_H

#include <QStringList>

class QString;

namespace EmulatorMirea
{

class Session;

namespace Commands
{

class Command
{
public:
    virtual ~Command() noexcept = default;

    [[nodiscard]] virtual QString name() const = 0;
    virtual QString execute(const QStringList& args, Session& session) const = 0;
};

class StubCommand : public Command
{
public:
    explicit StubCommand(QString name);

    [[nodiscard]] QString name() const override;
    QString execute(const QStringList& args, Session& session) const override;

private:
    QString m_name;
};

class ExitCommand : public Command
{
public:
    [[nodiscard]] QString name() const override;
    QString execute(const QStringList& args, Session& session) const override;
};

} // namespace Commands
} // namespace EmulatorMirea

#endif // COMMAND_H
