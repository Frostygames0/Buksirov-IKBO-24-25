#ifndef STREAM_H
#define STREAM_H

#include <optional>

#include <QString>

namespace EmulatorMirea::Core
{

class IOutputStream
{
public:
    virtual ~IOutputStream() noexcept = default;
    virtual void write(const QString& text) = 0;
};

class IInputStream
{
public:
    virtual ~IInputStream() noexcept = default;
    virtual std::optional<QString> readLine() = 0;
};

} // namespace EmulatorMirea::Core

#endif // STREAM_H
