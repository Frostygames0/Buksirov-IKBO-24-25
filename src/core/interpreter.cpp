#include "interpreter.h"

#include <QStringList>
#include <QStringView>

#include "core/commands/command.h"
#include "core/commands/executor.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"

namespace EmulatorMirea::Core
{

ExecutionResult Interpreter::execute(QStringView cmd, const ExecutionContext& ctx) const
{
    const QStringList args = m_lineParser.parse(cmd, ctx.envProvider);
    return m_commandExecutor.execute(args, ctx);
}

} // namespace EmulatorMirea::Core