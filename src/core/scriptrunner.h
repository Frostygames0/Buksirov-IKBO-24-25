#ifndef SCRIPTRUNNER_H
#define SCRIPTRUNNER_H

#include <QString>

#include "core/executionresult.h"

namespace EmulatorMirea::Core
{

class Shell;
class ITerminalView;

class ScriptRunner
{
public:
    ScriptRunner(Shell& shell, ITerminalView& view);

    ExecutionResult run(const QString& path);

private:
    Shell* m_shell;
    ITerminalView* m_view;
};

} // namespace EmulatorMirea::Core

#endif // SCRIPTRUNNER_H