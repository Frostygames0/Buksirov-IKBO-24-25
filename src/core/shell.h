#ifndef SHELL_H
#define SHELL_H

#include <QString>

#include <core/interpreter.h>
#include <core/session.h>

namespace EmulatorMirea::Core
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

} // namespace EmulatorMirea::Core

#endif // SHELL_H
