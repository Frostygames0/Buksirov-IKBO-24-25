#ifndef ITERMINALVIEW_H
#define ITERMINALVIEW_H

#include <QString>

#include "core/stream.h"

namespace EmulatorMirea::Core
{
class ITerminalView : public IOutputStream
{
public:
    void write(const QString& text) override = 0;

    virtual void setPrompt(const QString& prompt) = 0;
    virtual void showPrompt() = 0;
    virtual void setInput(const QString& text) = 0;

    [[nodiscard]] virtual QString currentInput() const = 0;
};

} // namespace EmulatorMirea::Core

#endif // ITERMINALVIEW_H
