#ifndef ENVEXPANDER_H
#define ENVEXPANDER_H

#include <QString>

namespace EmulatorMirea
{

class Session;

class EnvExpander
{
public:
    QString Expand(const QString& line, Session& session) const;
};

}

#endif // ENVEXPANDER_H
