#ifndef TERMINALOSTREAM_H
#define TERMINALOSTREAM_H

#include "core/stream.h"
#include "ui/terminalwidget.h"

namespace EmulatorMirea::Ui
{
class TerminalOStream final : public Core::IOutputStream
{
public:
    explicit TerminalOStream(TerminalWidget& widget) : m_widget(&widget) {}

    void write(const QString& text) override { m_widget->print(text); }

private:
    TerminalWidget* m_widget;
};
} // namespace EmulatorMirea::Ui

#endif // TERMINALOSTREAM_H
