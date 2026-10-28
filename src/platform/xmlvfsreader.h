#pragma once

#include <expected>

#include <QString>

namespace EmulatorMirea::Core
{

class IVfs;

}

namespace EmulatorMirea::Platform::XmlVfs
{

std::expected<void, QString> readFromFile(const QString& path, Core::IVfs& vfs);
bool writeStub(const QString& filePath);

} // namespace EmulatorMirea::Platform::XmlVfs
