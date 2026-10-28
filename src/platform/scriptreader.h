#pragma once

#include <expected>

#include <QString>
#include <QStringList>

namespace EmulatorMirea::Platform::ScriptReader
{

std::expected<QStringList, QString> readScript(const QString& path);

};