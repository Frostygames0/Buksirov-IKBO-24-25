#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QStringList>

#include "core/envexpander.h"

class QStringView;

namespace EmulatorMirea
{

class Tokenizer
{
public:
    [[nodiscard]] QStringList tokenize(QStringView line, const Session& session) const;

private:
    EnvExpander m_envExpander;
};

} // namespace EmulatorMirea

#endif // TOKENIZER_H
