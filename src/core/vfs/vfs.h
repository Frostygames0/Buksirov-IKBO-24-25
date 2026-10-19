#ifndef VFS_H
#define VFS_H

#include <expected>
#include <variant>

#include <QByteArray>
#include <QList>
#include <QString>
#include <QtTypes>

#include "core/vfs/path.h"
#include "core/vfs/types.h"

namespace EmulatorMirea::Core
{

class IVfs
{
public:
    enum class RemoveMode
    {
        Single,
        Recursive
    };

    virtual ~IVfs() = default;

    [[nodiscard]] virtual std::expected<VfsEntry, VfsError> stat(const VfsPath& path) const = 0;
    [[nodiscard]] virtual std::expected<QList<VfsEntry>, VfsError> list(const VfsPath& path) const = 0;
    [[nodiscard]] virtual std::expected<QByteArray, VfsError> readFile(const VfsPath& path) const = 0;

    virtual std::expected<void, VfsError> createFile(const VfsPath& path, const QByteArray& data) = 0;
    virtual std::expected<void, VfsError> createDir(const VfsPath& path) = 0;
    virtual std::expected<void, VfsError> copy(const VfsPath& from, const VfsPath& to) = 0;
    virtual std::expected<void, VfsError> move(const VfsPath& from, const VfsPath& to) = 0;
    virtual std::expected<void, VfsError> remove(const VfsPath& path, RemoveMode mode) = 0;

    virtual void clear() = 0;
};

class MemoryVfs final : public IVfs
{
public:
    MemoryVfs();

    [[nodiscard]] std::expected<VfsEntry, VfsError> stat(const VfsPath& path) const override;
    [[nodiscard]] std::expected<QList<VfsEntry>, VfsError> list(const VfsPath& path) const override;
    [[nodiscard]] std::expected<QByteArray, VfsError> readFile(const VfsPath& path) const override;

    std::expected<void, VfsError> createFile(const VfsPath& path, const QByteArray& data) override;
    std::expected<void, VfsError> createDir(const VfsPath& path) override;
    std::expected<void, VfsError> copy(const VfsPath& from, const VfsPath& to) override;
    std::expected<void, VfsError> move(const VfsPath& from, const VfsPath& to) override;
    std::expected<void, VfsError> remove(const VfsPath& path, RemoveMode mode) override;

    void clear() override;

private:
    using NodeId = qsizetype;
    static constexpr NodeId NodeIdNone = -1;
    static constexpr NodeId NodeIdRoot = 0;

    using FileData = QByteArray;
    using DirData = QList<NodeId>; // children
    using NodeData = std::variant<FileData, DirData, std::monostate>;

    struct Node
    {
        QString name;
        NodeId parent = NodeIdNone;
        NodeData data = std::monostate();
    };

    QList<Node> m_nodes;
    QList<NodeId> m_freeNodes;

    [[nodiscard]] std::expected<NodeId, VfsError> lookup(const VfsPath& path) const;
    [[nodiscard]] NodeId findChild(const DirData& dir, const QString& name) const;
    [[nodiscard]] bool isAncestorOrSelf(NodeId ancestor, NodeId node) const;
    [[nodiscard]] bool hasChildren(NodeId id) const;

    NodeId allocNode(Node node);

    [[nodiscard]] static VfsEntry makeEntryFromNode(const Node& node);

    struct Slot
    {
        NodeId parent;
        QString name;
    };

    [[nodiscard]] std::expected<Slot, VfsError> resolveFreeSlot(const VfsPath& path) const;

    NodeId createNodeUnsafe(NodeId parent, QString name, NodeData data);
    void detachUnsafe(NodeId id);

    void clearImpl();
};
} // namespace EmulatorMirea::Core

#endif