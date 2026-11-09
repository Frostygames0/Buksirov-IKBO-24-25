#pragma once

#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/vfs/types.h"

namespace EmulatorMirea::Core
{

class VfsPath
{
public:
    explicit VfsPath() = default;

    [[nodiscard]] static std::expected<VfsPath, VfsErrorCode> fromString(std::string_view line);
    [[nodiscard]] static std::expected<VfsPath, VfsErrorCode> resolve(const VfsPath& cwd, std::string_view line);

    [[nodiscard]] VfsPath parent() const;

    [[nodiscard]] std::optional<VfsPath> child(std::string_view name) const;
    [[nodiscard]] VfsPath join(const VfsPath& other) const;
    [[nodiscard]] bool isRoot() const;
    [[nodiscard]] bool isDescendantOf(const VfsPath& ancestor) const;

    [[nodiscard]] std::string_view topmostName() const;
    [[nodiscard]] const std::vector<std::string>& parts() const;
    [[nodiscard]] std::string toString() const;

    bool operator==(const VfsPath&) const = default;

private:
    std::vector<std::string> m_parts;
};

}; // namespace EmulatorMirea::Core