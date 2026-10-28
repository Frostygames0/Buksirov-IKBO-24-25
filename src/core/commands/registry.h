#pragma once

#include <memory>
#include <unordered_map>

#include <QString>

#include "core/commands/command.h"

namespace EmulatorMirea::Core
{

class CommandRegistry
{
public:
    struct Info
    {
        QString name;
        QString desciption;

        [[nodiscard]] QString toString() const { return QStringLiteral("%1 - %2").arg(name, desciption); }
    };

    void registerCommand(std::unique_ptr<Command> command);

    Command* getCommand(const QString& name) const;

    QList<Info> allCommandsInfo() const;

private:
    std::unordered_map<QString, std::unique_ptr<Command>> m_commands;
};

} // namespace EmulatorMirea::Core