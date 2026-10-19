#ifndef XMLVFSREADER_H
#define XMLVFSREADER_H

#include <expected>

#include <QString>

#include "core/vfs/vfs.h"

namespace EmulatorMirea::Core::XmlVfs
{

std::expected<MemoryVfs, QString> readFromFile(const QString& path);
bool writeStub(const QString& filePath);

} // namespace EmulatorMirea::Core::XmlVfs

#endif