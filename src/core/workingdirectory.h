#pragma once

#include "core/vfs/path.h"

namespace EmulatorMirea::Core
{
class IVfs;

class WorkingDirectory
{
public:
    explicit WorkingDirectory(IVfs& vfs) : m_vfs(vfs) {}

    [[nodiscard]] const VfsPath& currentDir() const { return m_cwd; }

    [[nodiscard]] std::expected<VfsPath, VfsError> resolve(QStringView path) const
    {
        return VfsPath::resolve(m_cwd, path);
    };

    std::expected<void, VfsError> set(const VfsPath& absolute);

private:
    VfsPath m_cwd;
    IVfs& m_vfs;
};
}; // namespace EmulatorMirea::Core