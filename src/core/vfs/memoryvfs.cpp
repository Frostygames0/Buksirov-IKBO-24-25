#include "memoryvfs.h"

#include <expected>
#include <queue>
#include <utility>
#include <variant>

#include <QByteArray>
#include <QList>
#include <QString>
#include <QtTypes>

#include "core/vfs/path.h"
#include "core/vfs/types.h"

namespace EmulatorMirea::Core
{

MemoryVfs::MemoryVfs()
{
    clearImpl();
}

std::expected<VfsEntry, VfsError> MemoryVfs::stat(const VfsPath& path) const
{
    const std::expected<NodeId, VfsErrorCode> id = lookup(path);
    if (!id)
    {
        return std::unexpected(id.error());
    }

    return makeEntryFromNode(m_nodes[*id]);
}

std::expected<QList<VfsEntry>, VfsError> MemoryVfs::list(const VfsPath& path) const
{
    const std::expected<NodeId, VfsErrorCode> id = lookup(path);
    if (!id)
    {
        return std::unexpected(id.error());
    }

    const auto* dir = std::get_if<DirData>(&m_nodes[*id].data);
    if (dir == nullptr)
    {
        return std::unexpected(VfsErrorCode::NotADirectory);
    }

    QList<VfsEntry> result;
    result.reserve(dir->size());
    for (const NodeId child : *dir)
    {
        result.append(makeEntryFromNode(m_nodes[child]));
    }
    return result;
}

std::expected<QByteArray, VfsError> MemoryVfs::readFile(const VfsPath& path) const
{
    const std::expected<NodeId, VfsErrorCode> id = lookup(path);
    if (!id)
    {
        return std::unexpected(id.error());
    }

    const auto* file = std::get_if<FileData>(&m_nodes[*id].data);
    if (file == nullptr)
    {
        return std::unexpected(VfsErrorCode::NotAFile);
    }

    return *file;
}

std::expected<void, VfsError> MemoryVfs::createFile(const VfsPath& path, const QByteArray& data)
{
    const std::expected<Slot, VfsErrorCode> slot = resolveFreeSlot(path);
    if (!slot)
    {
        return std::unexpected(slot.error());
    }

    createNodeUnsafe(slot->parent, slot->name, data);
    return {};
}

std::expected<void, VfsError> MemoryVfs::createDir(const VfsPath& path)
{
    const std::expected<Slot, VfsErrorCode> slot = resolveFreeSlot(path);
    if (!slot)
    {
        return std::unexpected(slot.error());
    }

    createNodeUnsafe(slot->parent, slot->name, DirData{});
    return {};
}

std::expected<MemoryVfs::ResolvedDestination, VfsError> MemoryVfs::resolveDestination(const VfsPath& to,
                                                                                      NodeId srcId) const
{
    const std::expected<NodeId, VfsErrorCode> existingDst = lookup(to);
    if (existingDst && std::holds_alternative<DirData>(m_nodes[*existingDst].data))
    {
        return ResolvedDestination{.parent = *existingDst, .name = m_nodes[srcId].name};
    }

    const std::expected<Slot, VfsErrorCode> slot = resolveFreeSlot(to);
    if (!slot)
    {
        return std::unexpected(VfsError{slot.error(), VfsOperand::To});
    }

    return ResolvedDestination{.parent = slot->parent, .name = slot->name};
}

std::expected<void, VfsError> MemoryVfs::copy(const VfsPath& from, const VfsPath& to)
{
    const std::expected<NodeId, VfsErrorCode> src = lookup(from);
    if (!src)
    {
        return std::unexpected(VfsError{src.error(), VfsOperand::From});
    }

    const auto dest = resolveDestination(to, *src);
    if (!dest)
    {
        return std::unexpected(dest.error());
    }

    if (isAncestorOrSelf(*src, dest->parent))
    {
        return std::unexpected(VfsErrorCode::InvalidCopy);
    }

    struct Task
    {
        NodeId src;
        NodeId dstParent;
        QString name;
    };

    std::queue<Task> queue;
    queue.push({.src = *src, .dstParent = dest->parent, .name = dest->name});

    while (!queue.empty())
    {
        Task t = std::move(queue.front());
        queue.pop();

        const NodeData data = m_nodes[t.src].data;

        if (const auto* dir = std::get_if<DirData>(&data))
        {
            const NodeId dst = createNodeUnsafe(t.dstParent, std::move(t.name), DirData{});
            for (const NodeId child : *dir)
            {
                queue.push({.src = child, .dstParent = dst, .name = m_nodes[child].name});
            }
        }
        else
        {
            createNodeUnsafe(t.dstParent, std::move(t.name), data);
        }
    }

    return {};
}

std::expected<void, VfsError> MemoryVfs::move(const VfsPath& from, const VfsPath& to)
{
    const std::expected<NodeId, VfsErrorCode> src = lookup(from);
    if (!src)
    {
        return std::unexpected(VfsError{src.error(), VfsOperand::From});
    }

    if (*src == NodeIdRoot)
    {
        return std::unexpected(VfsErrorCode::InvalidPath);
    }

    const auto dest = resolveDestination(to, *src);
    if (!dest)
    {
        return std::unexpected(dest.error());
    }

    if (isAncestorOrSelf(*src, dest->parent))
    {
        return std::unexpected(VfsErrorCode::InvalidMove);
    }

    detachUnsafe(*src);

    m_nodes[*src].name = dest->name;
    m_nodes[*src].parent = dest->parent;
    std::get<DirData>(m_nodes[dest->parent].data).append(*src);

    return {};
}

std::expected<void, VfsError> MemoryVfs::remove(const VfsPath& path, VfsRemoveMode mode)
{
    const std::expected<NodeId, VfsErrorCode> id = lookup(path);
    if (!id)
    {
        return std::unexpected(id.error());
    }

    if (*id == NodeIdRoot)
    {
        return std::unexpected(VfsErrorCode::CannotRemoveRoot);
    }

    if (mode == VfsRemoveMode::Single && hasChildren(*id))
    {
        return std::unexpected(VfsErrorCode::NotEmpty);
    }

    detachUnsafe(*id);

    QList<NodeId> stack{*id};
    while (!stack.empty())
    {
        NodeId stackId = stack.takeLast();

        if (const auto* dir = std::get_if<DirData>(&m_nodes[stackId].data); dir)
        {
            stack.append(*dir);
        }

        m_nodes[stackId] = Node{};
        m_freeNodes.append(stackId);
    }

    return {};
}

void MemoryVfs::clear()
{
    clearImpl();
}

void MemoryVfs::clearImpl()
{
    m_nodes.clear();
    m_freeNodes.clear();
    m_nodes.push_back(Node{
        .name = {},
        .parent = NodeIdNone,
        .data = DirData{},
    });
}

VfsEntry MemoryVfs::makeEntryFromNode(const Node& n)
{
    if (const auto* f = std::get_if<FileData>(&n.data))
    {
        return {.type = VfsEntryType::File, .name = n.name, .size = f->size()};
    }

    return {.type = VfsEntryType::Dir, .name = n.name, .size = 0};
}

std::expected<MemoryVfs::NodeId, VfsErrorCode> MemoryVfs::lookup(const VfsPath& path) const
{
    NodeId current = NodeIdRoot;

    for (const QString& name : path.parts())
    {
        const auto* dir = std::get_if<DirData>(&m_nodes[current].data);
        if (dir == nullptr)
        {
            return std::unexpected(VfsErrorCode::NotADirectory);
        }

        const NodeId next = findChild(*dir, name);
        if (next == NodeIdNone)
        {
            return std::unexpected(VfsErrorCode::NotFound);
        }

        current = next;
    }

    return current;
}

MemoryVfs::NodeId MemoryVfs::findChild(const DirData& dir, const QString& name) const
{
    for (const NodeId child : dir)
    {
        if (m_nodes[child].name == name)
        {
            return child;
        }
    }
    return NodeIdNone;
}

/*
 * This function checks whether it's possible to create something at path, be it a dir or a file.
 * It checks whether all parts of the path before the object itself are directories
 */
std::expected<MemoryVfs::Slot, VfsErrorCode> MemoryVfs::resolveFreeSlot(const VfsPath& path) const
{
    if (path.isRoot())
    {
        return std::unexpected(VfsErrorCode::AlreadyExists);
    }

    const std::expected<NodeId, VfsErrorCode> parentId = lookup(path.parent());
    if (!parentId)
    {
        return std::unexpected(parentId.error());
    }

    const auto* dir = std::get_if<DirData>(&m_nodes[*parentId].data);
    if (dir == nullptr)
    {
        return std::unexpected(VfsErrorCode::NotADirectory);
    }

    if (findChild(*dir, path.topmostName()) != NodeIdNone)
    {
        return std::unexpected(VfsErrorCode::AlreadyExists);
    }

    return MemoryVfs::Slot{.parent = *parentId, .name = path.topmostName()};
}

/*
 * Creates node at the parent, assumes that the parent is a dir, therefore it's UNSAFE
 */
MemoryVfs::NodeId MemoryVfs::createNodeUnsafe(NodeId parent, QString name, NodeData data)
{
    const NodeId id = allocNode(Node{
        .name = std::move(name),
        .parent = parent,
        .data = std::move(data),
    });

    // Index AFTER allocNode: it may reallocate m_nodes.
    std::get<DirData>(m_nodes[parent].data).append(id);
    return id;
}

/*
 * Detaches the part of the VFS tree by node ID.
 * NOT SAFE! DOESN'T VALIDATE ANYTHING, ASSUMES THAT THE CALLER VALIDATES IT
 */
void MemoryVfs::detachUnsafe(NodeId id)
{
    auto& siblings = std::get<DirData>(m_nodes[m_nodes[id].parent].data);
    siblings.removeOne(id);
}

bool MemoryVfs::isAncestorOrSelf(NodeId ancestor, NodeId node) const
{
    for (NodeId cur = node; cur != NodeIdNone; cur = m_nodes[cur].parent)
    {
        if (cur == ancestor)
        {
            return true;
        }
    }
    return false;
}

bool MemoryVfs::hasChildren(NodeId id) const
{
    const auto* dir = std::get_if<DirData>(&m_nodes[id].data);
    return dir != nullptr && !dir->isEmpty();
}

MemoryVfs::NodeId MemoryVfs::allocNode(Node node)
{
    // If we have a free node that can be used
    if (!m_freeNodes.isEmpty())
    {
        const NodeId id = m_freeNodes.takeLast();
        m_nodes[id] = std::move(node);
        return id;
    }

    // otherwise allocate new one
    m_nodes.append(std::move(node));
    return m_nodes.size() - 1;
}

} // namespace EmulatorMirea::Core