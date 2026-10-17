#include "path.h"

#include <QString>
#include <QLatin1String>
#include <QLatin1Char>

namespace EmulatorMirea::Core
{
bool VfsPath::isValidName(const QString& name)
{
    return !name.isEmpty() && name != QLatin1String(".") && name != QLatin1String("..") &&
           !name.contains(QLatin1Char('/'));
}

std::optional<VfsPath> VfsPath::fromString(const QString& line) {
    
}

VfsPath VfsPath::parent() const
{

}

VfsPath VfsPath::child(const QString& name) const {}

bool VfsPath::isRoot() const
{
    return m_parts.empty();
}

const QStringList& VfsPath::parts() const {}

QString VfsPath::toString() const {}
} // namespace EmulatorMirea::Core
