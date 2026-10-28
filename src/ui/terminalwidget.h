#pragma once

#include <QPlainTextEdit>

namespace EmulatorMirea::Ui
{

class TerminalWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TerminalWidget(QWidget* parent = nullptr);

public slots:
    void write(const QString& text);
    void writeLine(const QString& text);
    void showPrompt(const QString& prompt);
    void setInput(const QString& text);
    void clearScreen();

signals:
    void commandEntered(const QString& line);
    void historyUpRequested(const QString& currentInput);
    void historyDownRequested();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void insertFromMimeData(const QMimeData* source) override;

private:
    void moveCursorToEnd();
    [[nodiscard]] QString currentInput() const;

    int m_promptPos = 0;
};

} // namespace EmulatorMirea::Ui