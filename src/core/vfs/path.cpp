#include "path.h"

#include <optional>

#include <QString>

namespace EmulatorMirea::Core
{

namespace
{
bool isValidName(const QString& name)
{
    return !name.isEmpty() && name != u"." && name != u".." && !name.contains(u'/');
}

std::optional<VfsPath> applyString(VfsPath cur, const QString& line)
{
    for (const QString& part : line.split(u'/', Qt::SkipEmptyParts))
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

        std::optional<VfsPath> next = cur.child(part);
        if (!next)
        {
            return std::nullopt;
        }
        cur = *next;
    }
    return cur;
}

} // namespace

std::optional<VfsPath> VfsPath::fromString(const QString& line)
{
    if (!line.startsWith(u'/'))
    {
        return std::nullopt;
    }

    return applyString({}, line);
}

std::optional<VfsPath> VfsPath::resolve(const VfsPath& cwd, const QString& line)
{
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

std::optional<VfsPath> VfsPath::child(const QString& name) const
{
    if (!isValidName(name))
    {
        return std::nullopt;
    }

    VfsPath result = *this;
    result.m_parts.append(name);
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

QString VfsPath::name() const
{
    if (isRoot())
    {
        return {};
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
