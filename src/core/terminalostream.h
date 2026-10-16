#ifndef TERMINALOSTREAM_H
#define TERMINALOSTREAM_H

#include "core/iterminalview.h"
#include "core/stream.h"

namespace EmulatorMirea::Core
{
class TerminalOStream final : public IOutputStream
{
public:
    explicit TerminalOStream(ITerminalView& view) : m_widget(&view) {}

    void write(const QString& text) override { m_widget->print(text); }

private:
    ITerminalView* m_widget;
};
} // namespace EmulatorMirea::Core

#endif // TERMINALOSTREAM_H
