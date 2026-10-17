#ifndef VFSPATH_H
#define VFSPATH_H

#include <qchar.h>
namespace EmulatorMirea::Core
{

class VfsPath
{
public:
    VfsPath() = default;
    static std::optional<VfsPath> fromString(const QString& line);

    [[nodiscard]] VfsPath parent() const;
    [[nodiscard]] VfsPath child(const QString& name) const;
    [[nodiscard]] bool isRoot() const;

    [[nodiscard]] const QStringList& parts() const;
    [[nodiscard]] QString toString() const;

    bool operator==(const VfsPath& other) const = default;
    bool operator!=(const VfsPath& other) const = default;

private:
    QStringList m_parts;

    static bool isValidName(const QString& name);
};

}; // namespace EmulatorMirea::Core

#endif