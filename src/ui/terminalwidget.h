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
    void setPrompt(const QString& prompt);
    void writeLine(const QString& text);
    void showPrompt();
    void setInput(const QString& text);

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

    QString m_prompt{u"$ "};
    int m_promptPos = 0;
};

} // namespace EmulatorMirea::Ui