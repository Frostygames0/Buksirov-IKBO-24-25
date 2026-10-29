#pragma once

#include <expected>
#include <optional>

#include <QString>
#include <QStringList>
#include <QStringView>

#include "core/vfs/types.h"

namespace EmulatorMirea::Core
{

class VfsPath
{
public:
    explicit VfsPath() = default;

    static std::expected<VfsPath, VfsErrorCode> fromString(QStringView line);
    static std::expected<VfsPath, VfsErrorCode> resolve(const VfsPath& cwd, QStringView line);

    [[nodiscard]] VfsPath parent() const;

    [[nodiscard]] std::optional<VfsPath> child(QStringView name) const;
    [[nodiscard]] VfsPath join(const VfsPath& other) const;
    [[nodiscard]] bool isRoot() const;
    [[nodiscard]] bool isDescendantOf(const VfsPath& ancestor) const;

    [[nodiscard]] QString topmostName() const;
    [[nodiscard]] const QStringList& parts() const;
    [[nodiscard]] QString toString() const;

    friend bool operator==(const VfsPath&, const VfsPath&) = default;

private:
    QStringList m_parts;
};

}; // namespace EmulatorMirea::Core