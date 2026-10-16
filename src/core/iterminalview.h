#ifndef ITERMINALVIEW_H
#define ITERMINALVIEW_H

#include <QString>

namespace EmulatorMirea::Core
{
class ITerminalView
{
public:
    virtual ~ITerminalView() = default;
    virtual void print(const QString& text) = 0;
    virtual void setPrompt(const QString& prompt) = 0;
    virtual void showPrompt() = 0;
    virtual void setInput(const QString& text) = 0;
    [[nodiscard]] virtual QString currentInput() const = 0;
};

} // namespace EmulatorMirea::Core

#endif // ITERMINALVIEW_H
