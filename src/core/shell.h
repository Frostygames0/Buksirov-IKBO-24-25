#ifndef SHELL_H
#define SHELL_H

#include <core/interpreter.h>
#include <core/session.h>

class QString;

namespace EmulatorMirea
{

class Shell
{
public:
    QString submit(const QString& line);
    QString previousHistory();
    QString nextHistory();
    QString prompt() const;
    QString title() const;

private:
    Session m_session;
    Interpreter m_interpreter;
};

} // namespace EmulatorMirea

#endif // SHELL_H
