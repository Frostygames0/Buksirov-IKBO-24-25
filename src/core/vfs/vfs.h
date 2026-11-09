#pragma once

#include <cstddef>
#include <expected>
#include <vector>

#include "core/vfs/types.h"

namespace EmulatorMirea::Core
{

class VfsPath;

class IVfs
{
public:
    virtual ~IVfs() = default;

    [[nodiscard]] virtual std::expected<VfsEntry, VfsError> stat(const VfsPath& path) const = 0;
    [[nodiscard]] virtual std::expected<std::vector<VfsEntry>, VfsError> list(const VfsPath& path) const = 0;
    [[nodiscard]] virtual std::expected<std::vector<std::byte>, VfsError> readFile(const VfsPath& path) const = 0;

    virtual std::expected<void, VfsError> createFile(const VfsPath& path,
                                                     std::vector<std::byte> data,
                                                     VfsEncoding encoding = VfsEncoding::None) = 0;
    virtual std::expected<void, VfsError> createDir(const VfsPath& path) = 0;
    virtual std::expected<void, VfsError> copy(const VfsPath& from, const VfsPath& to) = 0;
    virtual std::expected<void, VfsError> move(const VfsPath& from, const VfsPath& to) = 0;
    virtual std::expected<void, VfsError> remove(const VfsPath& path, VfsRemoveMode mode) = 0;

    virtual void clear() = 0;
};

} // namespace EmulatorMirea::Core