#ifndef SHELL_H
#define SHELL_H

#include <QString>
#include <QStringList>
#include <QtTypes>

#include "core/interpreter.h"
#include "core/systemenvprovider.h"

namespace EmulatorMirea::Core
{

class IOutputStream;

class Shell
{
public:
    explicit Shell();

    ExecutionResult submit(const QString& line, IOutputStream& out);
    [[nodiscard]] qsizetype historySize() const;
    [[nodiscard]] QString historyAt(qsizetype index) const;
    [[nodiscard]] QString prompt() const;
    [[nodiscard]] QString title() const;

private:
    void addHistory(const QString& line);
    [[nodiscard]] QString fancyName() const;

    QString m_userName;
    QString m_machineName;

    SystemEnvProvider m_env;
    Interpreter m_interpreter;

    QStringList m_history;
};

} // namespace EmulatorMirea::Core

#endif // SHELL_H
