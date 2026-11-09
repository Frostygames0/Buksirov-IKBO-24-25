#include "path.h"

#include <algorithm>
#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/stringutils.h"
#include "core/vfs/types.h"

namespace EmulatorMirea::Core
{

namespace
{
bool isValidName(std::string_view name)
{
    return !name.empty() && name != "." && name != ".." && !name.contains('/');
}

std::expected<VfsPath, VfsErrorCode> applyString(VfsPath cur, std::string_view line)
{
    for (std::string_view part : StringUtils::split(line, '/'))
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
    return "/" + StringUtils::joinStrings(m_parts, "/");
}

} // namespace EmulatorMirea::Core
