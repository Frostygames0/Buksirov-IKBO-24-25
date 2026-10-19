#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QStringList>

#include "core/executionresult.h"

namespace EmulatorMirea::Core
{

class IOutputStream;
class IEnvProvider;
class ExecutionContext;

class Command
{
public:
    virtual ~Command() noexcept = default;

    [[nodiscard]] virtual QString name() const = 0;
    [[nodiscard]] virtual ExecutionResult execute(const QStringList& args, const ExecutionContext& ctx) const = 0;
};

class StubCommand : public Command
{
public:
    explicit StubCommand(const QString& name);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] ExecutionResult execute(const QStringList& args, const ExecutionContext& ctx) const override;

private:
    QString m_name;
};

class ExitCommand : public Command
{
public:
    [[nodiscard]] QString name() const override;
    [[nodiscard]] ExecutionResult execute(const QStringList& args, const ExecutionContext& ctx) const override;
};

} // namespace EmulatorMirea::Core

#endif // COMMAND_H
