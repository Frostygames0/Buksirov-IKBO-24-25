#include "terminalwidget.h"

#include <QKeyEvent>
#include <QMimeData>
#include <QString>
#include <QTextCursor>

namespace EmulatorMirea::Ui
{

TerminalWidget::TerminalWidget(QWidget* parent) : QPlainTextEdit(parent)
{
    setUndoRedoEnabled(false);
    setLineWrapMode(QPlainTextEdit::NoWrap);
}

void TerminalWidget::setPrompt(const QString& prompt)
{
    m_prompt = prompt;
}

void TerminalWidget::writeLine(const QString& text)
{
    if (text.isEmpty())
    {
        return;
    }

    moveCursorToEnd();
    insertPlainText(text);

    if (!text.endsWith(u'\n'))
    {
        insertPlainText(QString{u'\n'});
    }

    ensureCursorVisible();
}

void TerminalWidget::showPrompt()
{
    moveCursorToEnd();
    insertPlainText(m_prompt);
    m_promptPos = textCursor().position();
    ensureCursorVisible();
}

void TerminalWidget::setInput(const QString& text)
{
    QTextCursor c = textCursor();
    c.setPosition(m_promptPos);
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    c.insertText(text);
    setTextCursor(c);
    ensureCursorVisible();
}

QString TerminalWidget::currentInput() const
{
    QTextCursor c(document());
    c.setPosition(m_promptPos);
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    return c.selectedText();
}

void TerminalWidget::moveCursorToEnd()
{
    QTextCursor c = textCursor();
    c.movePosition(QTextCursor::End);
    setTextCursor(c);
}

void TerminalWidget::keyPressEvent(QKeyEvent* event)
{
    const QTextCursor c = textCursor();
    const bool touchesOutput = c.position() < m_promptPos || (c.hasSelection() && c.selectionStart() < m_promptPos);

    switch (event->key())
    {
        case Qt::Key_Return:
        case Qt::Key_Enter: {
            const QString line = currentInput();
            moveCursorToEnd();
            insertPlainText(QString{u'\n'});
            emit commandEntered(line);
            return;
        }
        case Qt::Key_Up:
            emit historyUpRequested(currentInput());
            return;
        case Qt::Key_Down:
            emit historyDownRequested();
            return;
        case Qt::Key_Backspace:
            if (touchesOutput || (!c.hasSelection() && c.position() <= m_promptPos))
            {
                return;
            }
            break;
        case Qt::Key_Delete:
            if (touchesOutput)
            {
                return;
            }
            break;
        case Qt::Key_Left:
            if (!c.hasSelection() && c.position() <= m_promptPos)
            {
                return;
            }
            break;
        case Qt::Key_Home: {
            QTextCursor home = c;
            const bool extend = event->modifiers() & Qt::ShiftModifier; // NOLINT
            home.setPosition(m_promptPos, extend ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
            setTextCursor(home);
            return;
        }
        default:
            break;
    }

    if (event->matches(QKeySequence::Cut) && touchesOutput)
    {
        return;
    }

    // Typing while the cursor sits in old output jumps back to the input line
    if (touchesOutput && !event->text().isEmpty() && event->text().at(0).isPrint())
    {
        moveCursorToEnd();
    }

    QPlainTextEdit::keyPressEvent(event);
}

void TerminalWidget::insertFromMimeData(const QMimeData* source)
{
    if (!source->hasText())
    {
        return;
    }

    const QTextCursor c = textCursor();
    if (c.position() < m_promptPos || (c.hasSelection() && c.selectionStart() < m_promptPos))
    {
        moveCursorToEnd();
    }

    QString text = source->text();
    text.replace(u'\n', u' ').remove(u'\r');
    insertPlainText(text);
}

} // namespace EmulatorMirea::Ui