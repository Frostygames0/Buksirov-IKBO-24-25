#ifndef ENVEXPANDER_H
#define ENVEXPANDER_H

#include <QString>

namespace EmulatorMirea
{

class Session;

class EnvExpander
{
public:
    QString expand(const QString& line, const Session& session) const;
};

}

#endif // ENVEXPANDER_H
