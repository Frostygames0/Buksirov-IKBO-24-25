#pragma once

#include <QString>

#include "core/output.h"
#include "core/useridentity.h"

namespace EmulatorMirea::Core
{

class IEnvProvider;
class IOutput;
class IVfs;
class UserIdentity;
class WorkingDirectory;
class CommandRegistry;

struct ExecutionContext
{
    UserIdentity identity;
    IEnvProvider& envProvider;
    IOutput& out;
    IVfs& vfs;
    WorkingDirectory& cwd;
    CommandRegistry& commandRegistry;

    void writeLine(const QString& line = {}) const { out.writeLine(line); }
};
} // namespace EmulatorMirea::Core
