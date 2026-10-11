#ifndef SHELL_H
#define SHELL_H

#include <QString>

#include <core/session.h>
#include <core/interpreter.h>

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

}

#endif // SHELL_H
