#pragma once

#include <cstddef>
#include <format>
#include <string>
#include <string_view>
#include <utility>

namespace EmulatorMirea::Core
{

enum class VfsEncoding
{
    None,
    Utf8,
    Base64,
};

enum class VfsEntryType
{
    File,
    Dir,
};

inline std::string_view vfsEntryTypeToString(VfsEntryType t)
{
    switch (t)
    {
        case VfsEntryType::File:
            return "file";
        case VfsEntryType::Dir:
            return "dir";
    }

    std::unreachable();
}

struct VfsEntry
{
    VfsEntryType type = VfsEntryType::File;
    VfsEncoding encoding = VfsEncoding::None;

    std::string name;
    std::size_t size = 0;

    friend bool operator==(const VfsEntry&, const VfsEntry&) = default;

    [[nodiscard]] bool isDir() const { return type == VfsEntryType::Dir; }

    [[nodiscard]] bool isFile() const { return type == VfsEntryType::File; }

    [[nodiscard]] std::string toString() const
    {
        return std::format("{}: {} -- {} bytes", vfsEntryTypeToString(type), name, size);
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

inline std::string_view vfsErrorCodeToStr(VfsErrorCode e)
{
    switch (e)
    {
        case VfsErrorCode::NotFound:
            return "not found";
        case VfsErrorCode::NotEmpty:
            return "not empty";
        case VfsErrorCode::NotAFile:
            return "not a file";
        case VfsErrorCode::NotADirectory:
            return "not a directory";
        case VfsErrorCode::InvalidPath:
            return "invalid path";
        case VfsErrorCode::AlreadyExists:
            return "already exists";
        case VfsErrorCode::InvalidArgument:
            return "invalid argument";
        case VfsErrorCode::InvalidMove:
            return "invalid move";
        case VfsErrorCode::InvalidCopy:
            return "invalid copy";
        case VfsErrorCode::CannotRemoveRoot:
            return "cannot remove root";
        case VfsErrorCode::NotSupported:
            return "not supported";
    }

    std::unreachable();
}

enum class VfsOperand
{
    None,
    From,
    To,
};

struct [[nodiscard]] VfsError
{
    VfsErrorCode code;
    VfsOperand operand = VfsOperand::None;

    VfsError(VfsErrorCode c, VfsOperand o = VfsOperand::None) : code(c), operand(o) {}

    [[nodiscard]] std::string toString(std::string_view path = "") const
    {
        std::string_view error = vfsErrorCodeToStr(code);

        std::string_view operandText;
        switch (operand)
        {
            case VfsOperand::From:
                operandText = "source";
            case VfsOperand::To:
                operandText = "destination";
            case VfsOperand::None:
                break;
        }
        return std::format("{} \"{}\": {}", operandText, path, error);
    }
};

enum class VfsRemoveMode
{
    Single,
    Recursive,
};

} // namespace EmulatorMirea::Core