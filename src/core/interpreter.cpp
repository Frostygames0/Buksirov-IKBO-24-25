#include "interpreter.h"

#include <QStringList>
#include <QStringView>

#include "core/commands/command.h"

namespace EmulatorMirea::Core
{

int Interpreter::execute(QStringView cmd, const CommandContext& ctx) const
{
    const QStringList args = m_lineParser.parse(cmd, ctx.envProvider);
    return m_commandExecutor.execute(args, ctx);
}

} // namespace EmulatorMirea::Core