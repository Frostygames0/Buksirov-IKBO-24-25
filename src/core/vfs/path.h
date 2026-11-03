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

    static std::expected<VfsPath, VfsErrorCode> fromString(std::string_view line);
    static std::expected<VfsPath, VfsErrorCode> resolve(const VfsPath& cwd, std::string_view line);

    [[nodiscard]] VfsPath parent() const;

    [[nodiscard]] std::optional<VfsPath> child(std::string_view name) const;
    [[nodiscard]] VfsPath join(const VfsPath& other) const;
    [[nodiscard]] bool isRoot() const;
    [[nodiscard]] bool isDescendantOf(const VfsPath& ancestor) const;

    [[nodiscard]] std::string_view topmostName() const;
    [[nodiscard]] const std::vector<std::string>& parts() const;
    [[nodiscard]] std::string toString() const;

    friend bool operator==(const VfsPath&, const VfsPath&) = default;

private:
    std::vector<std::string> m_parts;
};

}; // namespace EmulatorMirea::Core