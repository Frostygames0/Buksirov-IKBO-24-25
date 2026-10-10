// terminalwidget.cpp
#include "terminalwidget.h"
#include <QKeyEvent>
#include <QTextCursor>
#include <QFontDatabase>

TerminalWidget::TerminalWidget(QWidget *parent) : QPlainTextEdit(parent)
{
    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    setUndoRedoEnabled(false);
    setLineWrapMode(NoWrap);
}

void TerminalWidget::setPrompt(const QString &p) { m_prompt = p; }

void TerminalWidget::showPrompt()
{
    moveCursor(QTextCursor::End);
    insertPlainText(m_prompt);
    m_promptPos = textCursor().position();
    ensureCursorVisible();
}

void TerminalWidget::print(const QString &text)
{
    moveCursor(QTextCursor::End);
    insertPlainText(text);
    if (!text.endsWith('\n'))
        insertPlainText("\n");
    m_promptPos = textCursor().position();
    ensureCursorVisible();
}

QString TerminalWidget::currentInput() const
{
    QTextCursor c(document());
    c.setPosition(m_promptPos);
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    return c.selectedText();
}

void TerminalWidget::replaceInput(const QString &text)
{
    QTextCursor c(document());
    c.setPosition(m_promptPos);
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    c.insertText(text);
    setTextCursor(c);
}

void TerminalWidget::keyPressEvent(QKeyEvent *e)
{
    QTextCursor cur = textCursor();

    switch (e->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter: {
        QString line = currentInput();
        moveCursor(QTextCursor::End);
        insertPlainText("\n");
        m_promptPos = textCursor().position();
        if (!line.trimmed().isEmpty()) {
            m_history << line;
        }
        m_historyIndex = m_history.size();
        emit commandEntered(line);   // caller prints output, then calls showPrompt()
        return;
    }
    case Qt::Key_Backspace:
        if (cur.hasSelection() ? cur.selectionStart() < m_promptPos
                               : cur.position() <= m_promptPos)
            return;
        break;
    case Qt::Key_Left:
    case Qt::Key_Home:
        if (e->key() == Qt::Key_Home) {
            cur.setPosition(m_promptPos);
            setTextCursor(cur);
            return;
        }
        if (cur.position() <= m_promptPos) return;
        break;
    case Qt::Key_Up:
        if (m_historyIndex > 0)
            replaceInput(m_history[--m_historyIndex]);
        return;
    case Qt::Key_Down:
        if (m_historyIndex < m_history.size() - 1)
            replaceInput(m_history[++m_historyIndex]);
        else {
            m_historyIndex = m_history.size();
            replaceInput({});
        }
        return;
    default:
        break;
    }

    // Any text-modifying key: force the cursor into the editable area
    if (cur.position() < m_promptPos || cur.selectionStart() < m_promptPos) {
        if (!e->text().isEmpty() || e->key() == Qt::Key_Delete) {
            moveCursor(QTextCursor::End);
        }
    }
    QPlainTextEdit::keyPressEvent(e);
}