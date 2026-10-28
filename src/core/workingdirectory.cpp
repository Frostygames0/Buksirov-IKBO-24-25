#include "workingdirectory.h"

#include <expected>

#include <QtLogging>

#include "core/vfs/types.h"
#include "core/vfs/vfs.h"

namespace EmulatorMirea::Core
{

std::expected<void, VfsError> WorkingDirectory::set(const VfsPath& absolute)
{
    auto stat = m_vfs.stat(absolute);
    if (!stat)
    {
        return std::unexpected(stat.error());
    }

    if (!stat->isDir())
    {
        return std::unexpected(VfsErrorCode::NotADirectory);
    }

    m_cwd = absolute;
    return {};
}
}; // namespace EmulatorMirea::Core