#include "path.h"

#include <algorithm>
#include <cstddef>
#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/vfs/types.h"

namespace EmulatorMirea::Core
{

namespace
{

[[nodiscard]] std::string joinStrings(const std::vector<std::string>& parts, std::string_view delim)
{
    if (parts.empty())
    {
        return {};
    }

    std::size_t totalSize = 0;
    for (const auto& part : parts)
    {
        totalSize += part.size();
    }
    totalSize += delim.size() * (parts.size() - 1);

    std::string result;
    result.reserve(totalSize);

    result += parts[0];
    for (std::size_t i = 1; i < parts.size(); ++i)
    {
        result.append(delim);
        result.append(parts[i]);
    }

    return result;
}

[[nodiscard]] std::vector<std::string_view> split(std::string_view str, char delim)
{
    std::vector<std::string_view> result;
    std::size_t start = 0;
    std::size_t end = str.find(delim);

    while (end != std::string_view::npos)
    {
        auto part = str.substr(start, end - start);
        if (!part.empty())
        {
            result.push_back(str.substr(start, end - start));
        }

        start = end + 1;
        end = str.find(delim, start);
    }

    auto lastPart = str.substr(start);
    if (!lastPart.empty())
    {
        result.push_back(str.substr(start));
    }

    return result;
}

bool isValidName(std::string_view name)
{
    return !name.empty() && name != "." && name != ".." && !name.contains('/');
}

std::expected<VfsPath, VfsErrorCode> applyString(VfsPath cur, std::string_view line)
{
    for (std::string_view part : split(line, '/'))
    {
        if (part == ".")
        {
            continue;
        }

        if (part == "..")
        {
            cur = cur.parent();
            continue;
        }

        auto next = cur.child(part);
        if (!next)
        {
            return std::unexpected(VfsErrorCode::InvalidPath);
        }
        cur = *next;
    }
    return cur;
}

} // namespace

std::expected<VfsPath, VfsErrorCode> VfsPath::fromString(std::string_view line)
{
    if (!line.starts_with('/'))
    {
        return std::unexpected(VfsErrorCode::InvalidPath);
    }

    return applyString(VfsPath{}, line);
}

std::expected<VfsPath, VfsErrorCode> VfsPath::resolve(const VfsPath& cwd, std::string_view line)
{
    if (line.empty())
    {
        return cwd;
    }

    return applyString(line.starts_with('/') ? VfsPath{} : cwd, line);
}

VfsPath VfsPath::parent() const
{
    if (isRoot())
    {
        return *this;
    }

    VfsPath result = *this;
    result.m_parts.pop_back();

    return result;
}

std::optional<VfsPath> VfsPath::child(std::string_view name) const
{
    if (!isValidName(name))
    {
        return std::nullopt;
    }

    VfsPath result = *this;

    result.m_parts.emplace_back(name);
    return result;
}

VfsPath VfsPath::join(const VfsPath& other) const
{
    VfsPath result = *this;
    result.m_parts.append_range(other.parts());
    return result;
}

bool VfsPath::isRoot() const
{
    return m_parts.empty();
}

bool VfsPath::isDescendantOf(const VfsPath& ancestor) const
{
    return m_parts.size() > ancestor.m_parts.size() &&
           std::equal(ancestor.m_parts.begin(), ancestor.m_parts.end(), m_parts.begin());
}

std::string_view VfsPath::topmostName() const
{
    if (isRoot())
    {
        return "/";
    }

    return m_parts.back();
}

const std::vector<std::string>& VfsPath::parts() const
{
    return m_parts;
}

std::string VfsPath::toString() const
{
    return "/" + joinStrings(m_parts, "/");
}

} // namespace EmulatorMirea::Core
