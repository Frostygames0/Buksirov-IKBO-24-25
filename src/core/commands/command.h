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

    [[nodiscard]] virtual QString description() const { return {}; };

    [[nodiscard]] virtual ExecutionResult execute(const QStringList& args,
                                                  const ExecutionContext& ctx) const = 0;

protected:
    [[nodiscard]] QString formatOutput(QStringView out) const { return name() + u": " + out; }
};

class ExitCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override { return QStringLiteral("exit"); };

    [[nodiscard]] QString description() const override { return QStringLiteral("exits the terminal"); };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
};

class ListCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override { return QStringLiteral("ls"); };

    [[nodiscard]] QString description() const override
    {
        return QStringLiteral("lists all files/directories at path");
    };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
};

class VfsInitCommand final : public Command
{
public:
    VfsInitCommand(QString vfsLoadPath);

    [[nodiscard]] QString name() const override { return QStringLiteral("vfs-init"); };

    [[nodiscard]] QString description() const override
    {
        return QStringLiteral("initializes VFS from scratch");
    };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;

private:
    QString m_vfsLoadPath;
};

class ChangeDirectoryCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override { return QStringLiteral("cd"); };

    [[nodiscard]] QString description() const override
    {
        return QStringLiteral("changes the current working directory");
    };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
};

class ClearCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override { return QStringLiteral("clear"); };

    [[nodiscard]] QString description() const override { return QStringLiteral("clears the terminal"); };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
};

class EchoCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override { return QStringLiteral("echo"); };

    [[nodiscard]] QString description() const override
    {
        return QStringLiteral("outputs the passed input, parsed correctly");
    };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
};

class HelpCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override { return QStringLiteral("help"); };

    [[nodiscard]] QString description() const override
    {
        return QStringLiteral("displays the list of all commands");
    };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
};

class RemoveCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override { return QStringLiteral("rm"); };

    [[nodiscard]] QString description() const override
    {
        return QStringLiteral("removes files/directories");
    };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
};

class MoveCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override { return QStringLiteral("mv"); };

    [[nodiscard]] QString description() const override
    {
        return QStringLiteral("moves files/directories from source to destination");
    };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
};

class WcCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override { return QStringLiteral("wc"); };

    [[nodiscard]] QString description() const override
    {
        return QStringLiteral("counts words/lines/bytes");
    };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
};

class UniqCommand final : public Command
{
public:
    [[nodiscard]] QString name() const override { return QStringLiteral("uniq"); };

    [[nodiscard]] QString description() const override { return QStringLiteral("counts unique lines"); };

    [[nodiscard]] ExecutionResult execute(const QStringList& args,
                                          const ExecutionContext& ctx) const override;
};

} // namespace EmulatorMirea::Core
