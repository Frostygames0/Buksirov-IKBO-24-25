#ifndef COMMAND_H
#define COMMAND_H

#include <functional>

#include <QString>
#include <QStringList>

namespace EmulatorMirea::Core
{

class IOutputStream;
class IEnvProvider;

using ExitHandler = std::function<void(int)>;

struct CommandContext
{
    IEnvProvider& envProvider;
    IOutputStream& out;
    QString userName;
    QString machineName;
    ExitHandler requestExit;
};

namespace ExitCode
{
inline constexpr int Success = 0;
inline constexpr int GeneralError = 1;
inline constexpr int Misuse = 2;
inline constexpr int CommandNotFound = 127;
} // namespace ExitCode

class Command
{
public:
    virtual ~Command() noexcept = default;

    [[nodiscard]] virtual QString name() const = 0;
    [[nodiscard]] virtual int execute(const QStringList& args, const CommandContext& ctx) const = 0;
};

class StubCommand : public Command
{
public:
    explicit StubCommand(QString name);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] int execute(const QStringList& args, const CommandContext& ctx) const override;

private:
    QString m_name;
};

class ExitCommand : public Command
{
public:
    [[nodiscard]] QString name() const override;
    [[nodiscard]] int execute(const QStringList& args, const CommandContext& ctx) const override;
};

} // namespace EmulatorMirea::Core

#endif // COMMAND_H
