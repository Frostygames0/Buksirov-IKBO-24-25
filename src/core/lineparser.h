#pragma once

#include <QStringList>
#include <QStringView>

#include "core/envprovider.h"

namespace EmulatorMirea::Core
{

class IEnvProvider;

namespace LineParser
{

[[nodiscard]] QStringList parse(QStringView in, const IEnvProvider& env);

} // namespace LineParser
} // namespace EmulatorMirea::Core