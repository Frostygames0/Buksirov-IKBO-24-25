#pragma once

#include <QString>
#include <QStringList>

#include "core/executionresult.h"

namespace EmulatorMirea::Core
{

class IOutputStream;
class IEnvProvider;
class ExecutionContext;
class CommandRegistry;

class Command
{
public:
    virtual ~Command() noexcept = default;

    [[nodiscard]] virtual QString name() const = 0;

    [[nodiscard]] virtual QString description() const { return QStringLiteral("This is a command"); };

    [[nodiscard]] virtual ExecutionResult execute(const QStringList& args,
                                                  const ExecutionContext& ctx) const = 0;

protected:
    [[nodiscard]] QString formatOutput(QStringView out) const { return name() + u": " + out; }
};

class ExitCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override;

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;

    [[nodiscard]] QString description() const override;
};

class ListCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override;

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;

    [[nodiscard]] QString description() const override;
};

class VfsInitCommand final : public Command
{
public:
    VfsInitCommand(QString vfsLoadPath);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
    [[nodiscard]] QString description() const override;

private:
    QString m_vfsLoadPath;
};

class ChangeDirectoryCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override;
    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
    [[nodiscard]] QString description() const override;
};

class ClearCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override;
    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
    [[nodiscard]] QString description() const override;
};

class EchoCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override;
    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
    [[nodiscard]] QString description() const override;
};

class HelpCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override;
    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
    [[nodiscard]] QString description() const override;
};

} // namespace EmulatorMirea::Core
