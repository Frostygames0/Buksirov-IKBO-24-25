#ifndef EXECUTIONCONTEXT_H
#define EXECUTIONCONTEXT_H

#include <QString>

namespace EmulatorMirea::Core
{

class IEnvProvider;
class IOutputStream;
class IVfs;

struct ExecutionContext
{
    IEnvProvider& envProvider;
    IOutputStream& out;
    QString userName;
    QString machineName;
};
} // namespace EmulatorMirea::Core

#endif // EXECUTIONCONTEXT_H
