#pragma once

#include <QString>

#include "core/terminalview.h"
#include "core/useridentity.h"
#include "core/vfs/path.h"

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
    VfsPath& cwd;
};
} // namespace EmulatorMirea::Core
