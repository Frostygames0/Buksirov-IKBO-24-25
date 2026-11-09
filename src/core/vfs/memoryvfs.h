#include <cstddef>
#include <expected>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "core/vfs/path.h"
#include "core/vfs/types.h"
#include "core/vfs/vfs.h"

namespace EmulatorMirea::Core
{

class MemoryVfs final : public IVfs
{
public:
    explicit MemoryVfs();

    [[nodiscard]] std::expected<VfsEntry, VfsError> stat(const VfsPath& path) const override;
    [[nodiscard]] std::expected<std::vector<VfsEntry>, VfsError> list(const VfsPath& path) const override;
    [[nodiscard]] std::expected<std::vector<std::byte>, VfsError> readFile(const VfsPath& path) const override;

    std::expected<void, VfsError> createFile(const VfsPath& path,
                                             std::vector<std::byte> data,
                                             VfsEncoding encoding = VfsEncoding::None) override;
    std::expected<void, VfsError> createDir(const VfsPath& path) override;
    std::expected<void, VfsError> copy(const VfsPath& from, const VfsPath& to) override;
    std::expected<void, VfsError> move(const VfsPath& from, const VfsPath& to) override;
    std::expected<void, VfsError> remove(const VfsPath& path, VfsRemoveMode mode) override;

    void clear() override;

private:
    using NodeId = std::ptrdiff_t;
    static constexpr NodeId NodeIdNone = -1;
    static constexpr NodeId NodeIdRoot = 0;

    using FileData = std::vector<std::byte>;
    using DirData = std::vector<NodeId>; // children
    using NodeData = std::variant<FileData, DirData, std::monostate>;

    struct Node
    {
        std::string name;
        NodeId parent = NodeIdNone;
        NodeData data = std::monostate();
    };

    std::vector<Node> m_nodes;
    std::vector<NodeId> m_freeNodes;

    [[nodiscard]] std::expected<NodeId, VfsErrorCode> lookup(const VfsPath& path) const;
    [[nodiscard]] NodeId findChild(const DirData& dir, std::string_view name) const;
    [[nodiscard]] bool isAncestorOrSelf(NodeId ancestor, NodeId node) const;
    [[nodiscard]] bool hasChildren(NodeId id) const;

    NodeId allocNode(Node node);

    [[nodiscard]] static VfsEntry makeEntryFromNode(const Node& node);

    struct Slot
    {
        NodeId parent;
        std::string name;
    };

    [[nodiscard]] std::expected<Slot, VfsErrorCode> resolveFreeSlot(const VfsPath& path) const;

    struct ResolvedDestination
    {
        NodeId parent;
        std::string name;
    };

    [[nodiscard]] std::expected<ResolvedDestination, VfsError> resolveDestination(const VfsPath& to,
                                                                                  NodeId srcId) const;

    NodeId createNodeUnsafe(NodeId parent, std::string name, NodeData data);
    void detachUnsafe(NodeId id);

    void clearImpl();
};

} // namespace EmulatorMirea::Core