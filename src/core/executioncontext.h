#ifndef EXECUTIONCONTEXT_H
#define EXECUTIONCONTEXT_H

#include <QString>

#include "core/iterminalview.h"
#include "core/useridentity.h"

namespace EmulatorMirea::Core
{

class IEnvProvider;
class ITerminalView;
class IVfs;
class UserIdentity;

struct ExecutionContext
{
    UserIdentity identity;
    IEnvProvider& envProvider;
    ITerminalView& view;
    IVfs& vfs;
};
} // namespace EmulatorMirea::Core

#endif // EXECUTIONCONTEXT_H
