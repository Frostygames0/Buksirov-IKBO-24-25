#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QStringList>
#include <QStringView>

#include "core/envexpander.h"

namespace EmulatorMirea
{

class Tokenizer
{
public:
    QStringList tokenize(QStringView line, const Session& session) const;
private:
    EnvExpander m_envExpander;
};

}

#endif // TOKENIZER_H
