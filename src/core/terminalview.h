#pragma once

#include <QString>

namespace EmulatorMirea::Core
{
class ITerminalView
{
public:
    virtual ~ITerminalView() = default;

    virtual void write(const QString& text) = 0;

    virtual void setPrompt(const QString& prompt) = 0;
    virtual void showPrompt() = 0;
    virtual void setInput(const QString& text) = 0;

    [[nodiscard]] virtual QString currentInput() const = 0;
};

} // namespace EmulatorMirea::Core
