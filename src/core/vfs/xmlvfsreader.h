#ifndef XMLVFSREADER_H
#define XMLVFSREADER_H

#include <expected>

#include <QString>

namespace EmulatorMirea::Core
{

class IVfs;

namespace XmlVfs
{

std::expected<void, QString> readFromFile(const QString& path, IVfs& vfs);
bool writeStub(const QString& filePath);

} // namespace XmlVfs
} // namespace EmulatorMirea::Core

#endif