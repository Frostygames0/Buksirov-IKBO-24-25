#ifndef EXECUTIONCONTEXT_H
#define EXECUTIONCONTEXT_H

#include <QString>

#include "core/useridentity.h"

namespace EmulatorMirea::Core
{

class IEnvProvider;
class IOutputStream;
class IVfs;
class UserIdentity;

struct ExecutionContext
{
    IEnvProvider& envProvider;
    IOutputStream& out;
    UserIdentity identity;
};
} // namespace EmulatorMirea::Core

#endif // EXECUTIONCONTEXT_H
