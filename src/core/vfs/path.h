#ifndef VFSPATH_H
#define VFSPATH_H

#include <optional>

#include <QString>
#include <QStringList>

namespace EmulatorMirea::Core
{

class VfsPath
{
public:
    VfsPath() = default;
    static std::optional<VfsPath> fromString(const QString& line);
    static std::optional<VfsPath> resolve(const VfsPath& cwd, const QString& line);

    [[nodiscard]] VfsPath parent() const;

    [[nodiscard]] std::optional<VfsPath> child(const QString& name) const;
    [[nodiscard]] VfsPath join(const VfsPath& other) const;
    [[nodiscard]] bool isRoot() const;
    [[nodiscard]] bool isDescendantOf(const VfsPath& ancestor) const;

    [[nodiscard]] QString name() const;
    [[nodiscard]] const QStringList& parts() const;
    [[nodiscard]] QString toString() const;

    friend bool operator==(const VfsPath&, const VfsPath&) = default;

private:
    QStringList m_parts;
};

}; // namespace EmulatorMirea::Core

#endif