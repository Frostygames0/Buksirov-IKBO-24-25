#ifndef LINEPARSER_H
#define LINEPARSER_H

#include <QStringList>
#include <QStringView>

namespace EmulatorMirea::Core
{

class IEnvProvider;

class LineParser
{
public:
    [[nodiscard]] QStringList parse(QStringView in, const IEnvProvider& env) const;
};

} // namespace EmulatorMirea::Core

#endif // LINEPARSER_H