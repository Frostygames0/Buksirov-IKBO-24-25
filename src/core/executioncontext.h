#pragma once

#include <QString>

#include "core/output.h"
#include "core/useridentity.h"
#include "core/vfs/path.h"

namespace EmulatorMirea::Core
{

class IEnvProvider;
class IOutput;
class IVfs;
class UserIdentity;

struct ExecutionContext
{
    UserIdentity identity;
    IEnvProvider& envProvider;
    IOutput& out;
    IVfs& vfs;
    VfsPath& cwd;
};
} // namespace EmulatorMirea::Core
