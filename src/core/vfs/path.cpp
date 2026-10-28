#include "path.h"

#include <algorithm>
#include <expected>
#include <optional>

#include <QString>
#include <QStringView>

#include "core/vfs/types.h"

namespace EmulatorMirea::Core
{

namespace
{
bool isValidName(QStringView name)
{
    return !name.isEmpty() && name != u"." && name != u".." && !name.contains(u'/');
}

std::expected<VfsPath, VfsErrorCode> applyString(VfsPath cur, QStringView line)
{
    for (QStringView part : line.split(u'/', Qt::SkipEmptyParts))
    {
        if (part == u".")
        {
            continue;
        }

        if (part == u"..")
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

std::expected<VfsPath, VfsErrorCode> VfsPath::fromString(QStringView line)
{
    if (!line.startsWith(u'/'))
    {
        return std::unexpected(VfsErrorCode::InvalidPath);
    }

    return applyString({}, line);
}

std::expected<VfsPath, VfsErrorCode> VfsPath::resolve(const VfsPath& cwd, QStringView line)
{
    if (line.isEmpty())
    {
        return cwd;
    }

    return applyString(line.startsWith(u'/') ? VfsPath{} : cwd, line);
}

VfsPath VfsPath::parent() const
{
    if (isRoot())
    {
        return *this;
    }

    VfsPath result = *this;
    result.m_parts.removeLast();

    return result;
}

std::optional<VfsPath> VfsPath::child(QStringView name) const
{
    if (!isValidName(name))
    {
        return std::nullopt;
    }

    VfsPath result = *this;
    result.m_parts.append(name.toString());
    return result;
}

VfsPath VfsPath::join(const VfsPath& other) const
{
    VfsPath result = *this;
    result.m_parts += other.m_parts;
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

QString VfsPath::topmostName() const
{
    if (isRoot())
    {
        return QStringLiteral("/");
    }

    return m_parts.last();
}

const QStringList& VfsPath::parts() const
{
    return m_parts;
}

QString VfsPath::toString() const
{
    return u"/" + m_parts.join(u'/');
}

} // namespace EmulatorMirea::Core
