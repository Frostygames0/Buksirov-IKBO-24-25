#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include <QPlainTextEdit>

namespace EmulatorMirea::Ui
{

class TerminalWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TerminalWidget(QWidget* parent = nullptr);

public slots:
    void setPrompt(const QString& prompt);
    void print(const QString& text);
    void showPrompt();
    void setInput(const QString& text);

signals:
    void commandEntered(const QString& line);
    void historyUpRequested();
    void historyDownRequested();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void insertFromMimeData(const QMimeData* source) override;

private:
    [[nodiscard]] QString currentInput() const;
    void moveCursorToEnd();

    QString m_prompt = "$ ";
    int m_promptPos = 0;
};

} // namespace EmulatorMirea::Ui

#endif // TERMINALWIDGET_H