#pragma once

#include <QString>
#include <QStringLiteral>
#include <QtTypes>

namespace EmulatorMirea::Core
{
enum class VfsEntryType
{
    File,
    Dir,
};

struct VfsEntry
{
    VfsEntryType type = VfsEntryType::File;
    QString name;
    qsizetype size = 0;

    friend bool operator==(const VfsEntry&, const VfsEntry&) = default;
};

enum class [[nodiscard]] VfsError
{
    NotFound,
    NotEmpty,
    NotAFile,
    NotADirectory,
    InvalidPath,
    AlreadyExists,
    InvalidArgument,
    InvalidMove,
    InvalidCopy,
    CannotRemoveRoot,
    NotSupported,
};

inline QString vfsErrorToString(VfsError e)
{
    switch (e)
    {
        case VfsError::NotFound:
            return QStringLiteral("NotFound");
        case VfsError::NotEmpty:
            return QStringLiteral("NotEmpty");
        case VfsError::NotAFile:
            return QStringLiteral("NotAFile");
        case VfsError::NotADirectory:
            return QStringLiteral("NotADirectory");
        case VfsError::InvalidPath:
            return QStringLiteral("InvalidPath");
        case VfsError::AlreadyExists:
            return QStringLiteral("AlreadyExists");
        case VfsError::InvalidArgument:
            return QStringLiteral("InvalidArgument");
        case VfsError::InvalidMove:
            return QStringLiteral("InvalidMove");
        case VfsError::InvalidCopy:
            return QStringLiteral("InvalidCopy");
        case VfsError::CannotRemoveRoot:
            return QStringLiteral("CannotRemoveRoot");
        case VfsError::NotSupported:
            return QStringLiteral("NotSupported");
    }
    return QStringLiteral("Unknown");
}
} // namespace EmulatorMirea::Core