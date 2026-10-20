#ifndef SHELL_H
#define SHELL_H

#include <QString>
#include <QStringList>
#include <QtTypes>

#include "core/executionresult.h"
#include "core/useridentity.h"

namespace EmulatorMirea::Core
{

class IEnvProvider;
class Interpreter;
class IVfs;
class IOutputStream;

class Shell
{
public:
    explicit Shell(UserIdentity identity, IEnvProvider& envProvider, Interpreter& interpreter, IVfs& vfs);

    Shell(const Shell&) = delete;
    Shell& operator=(const Shell&) = delete;

    Shell(Shell&&) = delete;
    Shell& operator=(Shell&&) = delete;

    ExecutionResult submit(const QString& line, IOutputStream& out);

    
    [[nodiscard]] qsizetype historySize() const;
    [[nodiscard]] QString historyAt(qsizetype index) const;
    [[nodiscard]] QString prompt() const;
    [[nodiscard]] QString title() const;

private:
    void addHistory(const QString& line);

    UserIdentity m_identity;

    IEnvProvider& m_envProvider;
    Interpreter& m_interpreter;
    IVfs& m_vfs;

    QStringList m_history;
};

} // namespace EmulatorMirea::Core

#endif // SHELL_H
