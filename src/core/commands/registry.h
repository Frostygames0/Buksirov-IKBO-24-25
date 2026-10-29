#pragma once

#include <memory>
#include <unordered_map>
#include <utility>

#include <QString>
#include <QStringLiteral>

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

    template <class T, class... Args>
    void registerCommand(Args&&... args)
    {
        registerCommand(std::make_unique<T>(std::forward<Args>(args)...));
    };

    [[nodiscard]] Command* getCommand(const QString& name) const;

    [[nodiscard]] QList<Info> allCommandsInfo() const;

private:
    std::unordered_map<QString, std::unique_ptr<Command>> m_commands;
};

} // namespace EmulatorMirea::Core