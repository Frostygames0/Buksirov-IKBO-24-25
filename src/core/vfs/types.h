#pragma once

#include <qchar.h>

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

inline QString vfsEntryTypeToString(VfsEntryType t)
{
    switch (t)
    {
        case VfsEntryType::File:
            return QStringLiteral("file");
        case VfsEntryType::Dir:
            return QStringLiteral("dir");
    }

    return QStringLiteral("unknown");
}

struct VfsEntry
{
    VfsEntryType type = VfsEntryType::File;
    QString name;
    qsizetype size = 0;

    friend bool operator==(const VfsEntry&, const VfsEntry&) = default;

    [[nodiscard]] bool isDir() const { return type == VfsEntryType::Dir; }

    [[nodiscard]] bool isFile() const { return type == VfsEntryType::File; }

    [[nodiscard]] QString toString() const
    {
        return QStringLiteral("%1: %2 -- %3 bytes").arg(vfsEntryTypeToString(type), name).arg(size == 0);
    }
};

enum class VfsErrorCode
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

inline QString vfsErrorCodeToStr(VfsErrorCode e)
{
    switch (e)
    {
        case VfsErrorCode::NotFound:
            return QStringLiteral("not found");
        case VfsErrorCode::NotEmpty:
            return QStringLiteral("not empty");
        case VfsErrorCode::NotAFile:
            return QStringLiteral("not a file");
        case VfsErrorCode::NotADirectory:
            return QStringLiteral("not a directory");
        case VfsErrorCode::InvalidPath:
            return QStringLiteral("invalid path");
        case VfsErrorCode::AlreadyExists:
            return QStringLiteral("already exists");
        case VfsErrorCode::InvalidArgument:
            return QStringLiteral("invalid argument");
        case VfsErrorCode::InvalidMove:
            return QStringLiteral("invalid move");
        case VfsErrorCode::InvalidCopy:
            return QStringLiteral("invalid copy");
        case VfsErrorCode::CannotRemoveRoot:
            return QStringLiteral("cannot remove root");
        case VfsErrorCode::NotSupported:
            return QStringLiteral("not supported");
    }

    return QStringLiteral("unknown");
}

enum class VfsOperand
{
    None,
    From,
    To
};

struct [[nodiscard]] VfsError
{
    VfsErrorCode code;
    VfsOperand operand = VfsOperand::None;

    VfsError(VfsErrorCode c, VfsOperand o = VfsOperand::None) : code(c), operand(o) {}

    [[nodiscard]] QString message() const
    {
        QString text = vfsErrorCodeToStr(code);
        switch (operand)
        {
            case VfsOperand::From:
                return QStringLiteral("source: %1").arg(text);
            case VfsOperand::To:
                return QStringLiteral("destination: %1").arg(text);
            case VfsOperand::None:
                break;
        }
        return text;
    }
};

enum class VfsRemoveMode
{
    Single,
    Recursive,
};
} // namespace EmulatorMirea::Core