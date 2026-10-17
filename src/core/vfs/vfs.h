#ifndef VFS_H
#define VFS_H

#include <qlist.h>
#include <qobject.h>

#include "core/vfs/types.h"
#include "core/vfs/path.h"

namespace EmulatorMirea::Core
{
class IVfs
{
public:
    virtual ~IVfs() = default;

    [[nodiscard]] bool exists(const VfsPath& path) const;

    [[nodiscard]] std::optional<QList<VfsEntry>> list(const VfsPath& path) const;
    [[nodiscard]] std::optional<QByteArray> readFile(const VfsPath& path) const;

    VfsError move(const VfsPath& from, const VfsPath& to);
    VfsError remove(const VfsPath& from, bool recursive);

    void reset();
    
    // exists

    // list
    // readFile
    // move
    // remove

    // reset
private:

    using NodeId = int;
    static constexpr NodeId NodeIdNone = -1;
    static constexpr NodeId NodeIdRoot = 0;

    using FileData = QByteArray;
    using DirData = QList<NodeId>;

    struct Node
    {
        QString name;
        NodeId parent = NodeIdNone;
        std::variant<FileData, DirData> data;
    };

    QList<Node> m_nodes;
};
} // namespace EmulatorMirea::Core

#endif