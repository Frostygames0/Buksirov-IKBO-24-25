#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include <QPlainTextEdit>

class TerminalWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TerminalWidget(QWidget* parent = nullptr);

    void setPrompt(const QString& prompt);
    void print(const QString& text);
    void showPrompt();

signals:
    void commandEntered(const QString& line);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    int inputStartPos() const { return m_promptPos; }
    QString currentInput() const;
    void replaceInput(const QString& text);

    QString m_prompt = "$ ";
    int m_promptPos = 0;
    QStringList m_history;
    int m_historyIndex;
};

#endif // TERMINALWIDGET_H
