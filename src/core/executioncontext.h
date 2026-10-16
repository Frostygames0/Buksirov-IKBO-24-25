#ifndef EXECUTIONCONTEXT_H
#define EXECUTIONCONTEXT_H

#include <QString>

namespace EmulatorMirea::Core
{
class IEnvProvider;
class IOutputStream;

struct ExecutionContext
{
    IEnvProvider& envProvider;
    IOutputStream& out;
    QString userName;
    QString machineName;
};
} // namespace EmulatorMirea

#endif // EXECUTIONCONTEXT_H
