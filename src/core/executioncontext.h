#pragma once

#include <string>

#include "core/output.h"
#include "core/useridentity.h"

namespace EmulatorMirea::Core
{

class IEnvProvider;
class IOutput;
class IVfs;
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

    void writeLine(const std::string& line = "") const { out.writeLine(line); }

    void write(const std::string& line) const { out.write(line); }
};

} // namespace EmulatorMirea::Core
