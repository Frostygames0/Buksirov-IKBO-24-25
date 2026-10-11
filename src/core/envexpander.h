#ifndef ENVEXPANDER_H
#define ENVEXPANDER_H

#include <QString>

namespace EmulatorMirea
{

class Session;

class EnvExpander
{
public:
    [[nodiscard]] QString expand(const QString& line, const Session& session) const;
};

} // namespace EmulatorMirea

#endif // ENVEXPANDER_H
