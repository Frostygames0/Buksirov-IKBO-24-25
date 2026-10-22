#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include <QPlainTextEdit>

#include "core/iterminalview.h"

namespace EmulatorMirea::Ui
{

class TerminalWidget : public QPlainTextEdit, public Core::ITerminalView
{
    Q_OBJECT
public:
    explicit TerminalWidget(QWidget* parent = nullptr);

public slots:
    void setPrompt(const QString& prompt) override;
    void write(const QString& text) override;
    void showPrompt() override;
    void setInput(const QString& text) override;
    [[nodiscard]] QString currentInput() const override;

signals:
    void commandEntered(const QString& line);
    void historyUpRequested();
    void historyDownRequested();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void insertFromMimeData(const QMimeData* source) override;

private:
    void moveCursorToEnd();

    QString m_prompt{u"$ "};
    int m_promptPos = 0;
};

} // namespace EmulatorMirea::Ui

#endif // TERMINALWIDGET_H