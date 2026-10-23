#pragma once

#include <QPlainTextEdit>

#include "core/output.h"

namespace EmulatorMirea::Ui
{

class TerminalWidget : public QPlainTextEdit, public Core::IOutput
{
    Q_OBJECT
public:
    explicit TerminalWidget(QWidget* parent = nullptr);

public slots:
    void setPrompt(const QString& prompt);
    void writeLine(const QString& text) override;
    void showPrompt();
    void setInput(const QString& text);

signals:
    void commandEntered(const QString& line, Core::IOutput& out);
    void historyUpRequested(const QString& currentInput);
    void historyDownRequested();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void insertFromMimeData(const QMimeData* source) override;

private:
    void moveCursorToEnd();
    [[nodiscard]] QString currentInput() const;

    QString m_prompt{u"$ "};
    int m_promptPos = 0;
};

} // namespace EmulatorMirea::Ui